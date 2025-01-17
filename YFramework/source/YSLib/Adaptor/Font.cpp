﻿/*
	© 2009-2016, 2019-2021 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file Font.cpp
\ingroup Adaptor
\brief 平台无关的字体库。
\version r3727
\author FrankHB <frankhb1989@gmail.com>
\since build 296
\par 创建时间:
	2009-11-12 22:06:13 +0800
\par 修改时间:
	2022-08-01 19:12 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::Adaptor::Font
*/


#include "Helper/YModules.h"
#include YFM_YSLib_Adaptor_Font
#include YFM_YSLib_Core_YException // for LoggedEvent;
#include YFM_YSLib_Service_FileSystem
#include YFM_Helper_Initialization
#include YFM_YCLib_Debug
#include <ystdex/deref_op.hpp> // for ystdex::call_value_or;
#include <ft2build.h>
#include FT_FREETYPE_H // for ::FT_Matrix, ::FT_Memory;
#include FT_SIZES_H // for ::FT_New_Size, FT_Done::Size;
#include FT_BITMAP_H // for ::FT_GlyphSlot_Own_Bitmap, ::FT_Bitmap_Embolden;
//! \since build 952
#define YF_Impl_Adaptor_Font_FreeTypeVer \
	(FREETYPE_MAJOR * 10000 + FREETYPE_MINOR * 100 + FREETYPE_PATCH)
// XXX: This is only used when the modified version in "3rdparty" is enabled,
//	current only for platform %DS and %MinGW32.
#if defined(FT_CONFIG_OPTION_OLD_INTERNALS) \
	&& YF_Impl_Adaptor_Font_FreeTypeVer >= 20500
#	define YF_Impl_Use_FT_Internal true
// XXX: The internal header <freetype/internal/internal.h> was removed since
//	FreeType 2.10.3. (Before that, the macro for the internal header is only
//	available when %FT2_BUILD_LIBRARY is defined, so it would still rely on the
//	layout which was once changed in FreeType 2.5; although the modified version
//	in "3rdparty" always has the known layout and fixed header names.) Thus, the
//	header is not used.
#	include <freetype/internal/tttypes.h> // for ::TT_Face;
#	include <ystdex/type_pun.hpp> // for ystdex::aligned_cast;
#endif
#include <cstdlib> // for std::malloc, std::free, std::realloc;
#include FT_MODULE_H // for ::FT_New_Library, ::FT_Add_Default_Modules,
//	::FT_Done_Library;
#include <ystdex/string.hpp> // for ystdex::sfmt;

using namespace ystdex;
using namespace platform;

namespace YSLib
{

using namespace IO;

namespace Drawing
{

//! \since build 562
//@{
static_assert(std::is_same<FontException::FontError, ::FT_Error>(),
	"Invalid type found.");
static_assert(std::is_same<CharBitmap::BufferType, ::FT_Byte*>(),
	"Invalid type found.");
static_assert(int(CharBitmap::None) == ::FT_PIXEL_MODE_NONE && int(
	CharBitmap::Mono) == ::FT_PIXEL_MODE_MONO && int(CharBitmap::Gray)
	== ::FT_PIXEL_MODE_GRAY && int(CharBitmap::Gray2) == ::FT_PIXEL_MODE_GRAY2
	&& int(CharBitmap::Gray4) == ::FT_PIXEL_MODE_GRAY4 && int(CharBitmap::LCD)
	== ::FT_PIXEL_MODE_LCD && int(CharBitmap::LCD_V) == ::FT_PIXEL_MODE_LCD_V,
	"Incompatible format found.");
static_assert(std::is_same<CharBitmap::ScaleType, ::FT_Byte>(),
	"Invalid type found.");
static_assert(std::is_same<CharBitmap::SignedScaleType, ::FT_Char>(),
	"Invalid type found.");
//@}


ImplDeDtor(FontException)


//! \since build 420
namespace
{

//! \brief 替代 ::FT_Set_Pixel_Sizes 。
YB_ATTR_nodiscard ::FT_Error
N_SetPixelSizes(::FT_FaceRec& face, ::FT_UInt s) ynothrow
{
	::FT_Size_RequestRec req{FT_SIZE_REQUEST_TYPE_NOMINAL, ::FT_Long(s << 6),
		::FT_Long(s << 6), 0, 0};

	return ::FT_Request_Size(&face, &req);
}

//! \since build 849
YB_ATTR_nodiscard YB_STATELESS std::int8_t
CvtFixed26_6ToI8(::FT_Pos v)
{
	// XXX: The internal value is is in 26.6 format in FreeType2.
	// FIXME: Possible truncation?
	// XXX: No exception specification is specified here currently.
	return v >> 6;
}

// XXX: This should be 'const', but not allowed by the signature of
//	%::FT_Set_Transform.
/*!
\since build 421
\see ::FT_GlyphSlot_Oblique 实现。
*/
::FT_Matrix italic_matrix{0x10000L, 0x0366AL, 0x0000L, 0x10000L};

} // unnamed namespace;


NativeFontSize::NativeFontSize(::FT_FaceRec& face, FontSize s)
	: size()
{
	if(const auto err = ::FT_New_Size(&face, &size))
		throw FontException(err, "Native font size creation failed.");
	Activate();
	if(const auto err = N_SetPixelSizes(face, s))
		throw FontException(err, "Native font setting size failed.");
}
NativeFontSize::NativeFontSize(NativeFontSize&& ns) ynothrow
	: size(ns.size)
{
	ns.size = {};
}
NativeFontSize::~NativeFontSize()
{
	::FT_Done_Size(size);
}

::FT_SizeRec&
NativeFontSize::GetSizeRec() const
{
	if(YB_UNLIKELY(!size))
		throw LoggedEvent("Invalid native size found.", Critical);
	return *size;
}

void
NativeFontSize::Activate() const
{
	Deref(Deref(size).face).size = size;
}


FontFamily::FontFamily(const FamilyName& name)
	: family_name(name), mFaces()
{}

void
FontFamily::operator+=(Typeface& face)
{
	mFaces.emplace(face.GetStyleName(), face);
}

bool
FontFamily::operator-=(Typeface& face) ynothrow
{
	return mFaces.erase(face.GetStyleName()) != 0;
}

observer_ptr<Typeface>
FontFamily::GetTypefacePtr(FontStyle fs) const
{
	const auto p(GetTypefacePtr(FetchName(fs)));

	return p ? p : (fs == FontStyle::Regular ? nullptr
		: GetTypefacePtr("Regular"));
}
observer_ptr<Typeface>
FontFamily::GetTypefacePtr(const StyleName& style_name) const
{
	// TODO: Blocked. Use %string_view as argument using C++14 heterogeneous
	//	%find template.
	const auto i(mFaces.find(style_name));

	return (i == mFaces.cend()) ? nullptr : make_observer(&i->second.get());
}
Typeface&
FontFamily::GetTypefaceRef(FontStyle fs) const
{
	if(const auto p = GetTypefacePtr(fs))
		return *p;
	throw LoggedEvent("No matching face found.", Critical);
}
Typeface&
FontFamily::GetTypefaceRef(const StyleName& style_name) const
{
	if(const auto p = GetTypefacePtr(style_name))
		return *p;
	throw LoggedEvent("No matching face found.", Critical);
}


Typeface::SmallBitmapData::SmallBitmapData(::FT_GlyphSlot slot, FontStyle style)
{
	if(slot && slot->format == FT_GLYPH_FORMAT_BITMAP)
	{
		auto& bitmap(slot->bitmap);

		if(bool(style & FontStyle::Bold))
		{
			const auto library(slot->library);
			const auto face(slot->face);
			::FT_Pos xstr(FT_MulFix(face->units_per_EM,
				face->size->metrics.y_scale) / 24 & ~63), ystr(xstr);

			if(xstr == 0)
				xstr = 1 << 6;
			if(::FT_GlyphSlot_Own_Bitmap(slot) == FT_Err_Ok
				&& ::FT_Bitmap_Embolden(library, &bitmap, xstr, ystr)
				== FT_Err_Ok)
			{
				if(slot->advance.x)
					slot->advance.x += xstr;
				if(slot->advance.y)
					slot->advance.y += ystr;
				{
					auto& metrics(slot->metrics);

					yunseq(metrics.width += xstr, metrics.height += ystr,
						metrics.horiAdvance += xstr,
						metrics.vertAdvance += ystr);
				}
				slot->bitmap_top += ::FT_Int(ystr >> 6);
			}
		}

		const ::FT_Pos xadv((slot->advance.x + 32) >> 6),
			yadv((slot->advance.y + 32) >> 6);

#define YSL_Impl_SB_CheckChar(d) (::FT_Int(::FT_Char(d)) == ::FT_Int(d))
#define YSL_Impl_SB_CheckByte(d) (::FT_Int(::FT_Byte(d)) == ::FT_Int(d))
		if(YSL_Impl_SB_CheckByte(bitmap.rows)
			&& YSL_Impl_SB_CheckByte(bitmap.width)
			&& YSL_Impl_SB_CheckChar(bitmap.pitch)
			&& YSL_Impl_SB_CheckChar(slot->bitmap_left)
			&& YSL_Impl_SB_CheckChar(slot->bitmap_top)
			&& YSL_Impl_SB_CheckChar(xadv) && YSL_Impl_SB_CheckChar(yadv))
		{
			yunseq(
			width = std::uint8_t(bitmap.width),
			height = std::uint8_t(bitmap.rows),
			left = static_cast<signed char>(slot->bitmap_left),
			top = static_cast<signed char>(slot->bitmap_top),
			format = std::uint8_t(bitmap.pixel_mode),
			max_grays = std::uint8_t(bitmap.num_grays - 1),
			pitch = static_cast<signed char>(bitmap.pitch),
			xadvance = static_cast<signed char>(xadv),
			yadvance = static_cast<signed char>(yadv),
			buffer = bitmap.buffer
			);
			bitmap.buffer = {};
			// XXX: Moving instead of copying should be safe if the library
			//	memory handlers are not customized.
			// NOTE: Be cautious for DLLs. For documented default behavior, see:
			//	http://www.freetype.org/freetype2/docs/design/design-4.html.
			//	This is now implemented in the constructor of %FontCache.
			// NOTE: See also $2022-08 @ %Documentation::Workflow.
			return;
		}
#undef YSL_Impl_SB_CheckChar
#undef YSL_Impl_SB_CheckByte
	}
}
Typeface::SmallBitmapData::SmallBitmapData(SmallBitmapData&& sbit_dat)
	ynothrow
	: width(sbit_dat.width), height(sbit_dat.height), left(sbit_dat.left),
	top(sbit_dat.top), format(sbit_dat.format), max_grays(sbit_dat.width),
	pitch(sbit_dat.pitch), xadvance(sbit_dat.xadvance),
	yadvance(sbit_dat.yadvance), buffer(sbit_dat.buffer)
{
	sbit_dat.buffer = {};
}
Typeface::SmallBitmapData::~SmallBitmapData()
{
	// NOTE: See the notes in the constructor.
	std::free(buffer);
}


Typeface::Typeface(FontCache& cache, const FontPath& path, std::uint32_t i)
	// XXX: Conversion to 'long' might be implementation-defined.
	: face_index(long(i)), cmap_index(-1), style_name(),
	ref([&, this]{
		if(YB_UNLIKELY(ystdex::exists(cache.mFaces, path)))
			throw LoggedEvent("Duplicate typeface found.", Critical);

		::FT_Face face;
		auto error(::FT_New_Face(cache.library, path.c_str(), face_index,
			&face));

		if(YB_LIKELY(!error))
			if(YB_LIKELY(!(error = ::FT_Select_Charmap(face,
				FT_ENCODING_UNICODE)) && face))
				cmap_index = face->charmap
					? ::FT_Get_Charmap_Index(face->charmap) : 0;
		if(YB_UNLIKELY(error))
			throw LoggedEvent(ystdex::sfmt("Face loading failed"
				" with face request error: %08x\n.", error), Critical);
		return pair<lref<FontFamily>, lref<::FT_FaceRec_>>(
			cache.LookupFamily(face->family_name), *face);
	}()), bitmap_cache(2047U), glyph_index_cache()
{
	// FIXME: This should be exception, but not assertion for malformed fonts.
	YAssert(::FT_UInt(cmap_index) < ::FT_UInt(ref.second.get().num_charmaps),
		"Invalid CMap index found.");
	style_name = ref.second.get().style_name;
	ref.first.get() += *this;
}
Typeface::~Typeface()
{
	size_cache.clear();
	glyph_index_cache.clear();
	bitmap_cache.clear();
	ref.first.get() -= *this;

	const auto face(&ref.second.get());

#if YF_Impl_Use_FT_Internal
	YAssert(Deref(face).internal->refcount == 1,
		"Invalid face reference count found.");
	// XXX: Hack for using %ttmtx.c and %sfobjs.c of FreeType 2.4.11.
	if(FT_IS_SFNT(face))
	{
		// XXX: Downcast.
		const auto ttface(ystdex::aligned_cast<::TT_Face>(face));

		// NOTE: See %Typeface::SmallBitmapData::SmallBitmapData.
		// NOTE: %sfnt_done_face in "sfobjs.c" still releases vertical metrics.
		std::free(ttface->horizontal.long_metrics),
		std::free(ttface->horizontal.short_metrics);
	}
#endif
	::FT_Done_Face(face);
}

Typeface::SmallBitmapData&
Typeface::LookupBitmap(const Typeface::BitmapKey& key) const
{
	return ystdex::cache_lookup(bitmap_cache, key, [&]{
		LookupSize(key.Size).Activate();
		::FT_Set_Transform(&ref.second.get(),
			bool(key.Style & FontStyle::Italic) ? &italic_matrix : nullptr, {});

		return SmallBitmapData(::FT_Load_Glyph(&ref.second.get(),
			key.GlyphIndex, std::int32_t(key.Flags | FT_LOAD_RENDER)) == 0
			? ref.second.get().glyph : nullptr, key.Style);
	});
}

::FT_UInt
Typeface::LookupGlyphIndex(char32_t c) const
{
	auto i(glyph_index_cache.find(c));

	if(i == glyph_index_cache.end())
	{
		if(cmap_index > 0)
			::FT_Set_Charmap(&ref.second.get(),
				ref.second.get().charmaps[cmap_index]);

		const auto pr(glyph_index_cache.emplace(c, ::FT_Get_Char_Index(
			&ref.second.get(), ::FT_ULong(c))));

		if(YB_UNLIKELY(!pr.second))
			throw LoggedEvent("Glyph index cache insertion failed.", Alert);
		i = pr.first;
	}
	return i->second;
}

NativeFontSize&
Typeface::LookupSize(FontSize s) const
{
	auto i(size_cache.find(s));

	if(i == size_cache.end())
	{
		const auto pr(size_cache.emplace(s, NativeFontSize(ref.second, s)));

		if(YB_UNLIKELY(!pr.second))
			throw LoggedEvent("Bitmap cache insertion failed.", Alert);
		i = pr.first;
	}
	return i->second;
}


const Typeface&
FetchDefaultTypeface()
{
	if(const auto p = FetchDefaultFontCache().GetDefaultTypefacePtr())
		return *p;
	throw LoggedEvent("Null default font face pointer found.", Critical);
}


FontCache::FontCache(size_t /*cache_size*/)
	: pDefaultFace()
{
	::FT_Error error;

	if(YB_LIKELY((error = [&]{
		// XXX: This should be probably 'const', but not allowed by the
		//	signature of %::FT_New_Library. Note that exception specifications
		//	are not allowed by FreeType.
		static ::FT_MemoryRec_ mrec{yimpl({}), [](::FT_Memory, long size){
			return std::malloc(size_t(size));
		}, [](::FT_Memory, void* block){
			std::free(block);
		},
			[](::FT_Memory, long, long new_size, void* block){
			return ::realloc(block, size_t(new_size));
		}};

		return ::FT_New_Library(&mrec, &library);
	}()) == 0))
	{
		::FT_Add_Default_Modules(library);
		YTraceDe(Informative, "FreeType library instance initialized.");
	}
	else
		// TODO: More precise error message.
		throw LoggedEvent(
			ystdex::sfmt("Font init failed: %08x\n;", error).c_str(), Alert);
}
FontCache::~FontCache()
{
	mFaces.clear();
	mFamilies.clear();
	::FT_Done_Library(library);
}

observer_ptr<const FontFamily>
FontCache::GetFontFamilyPtr(const FamilyName& family_name) const
{
	// TODO: Blocked. Use %string_view as argument using C++14 heterogeneous
	//	%find template.
	const auto i(mFamilies.find(family_name));

	return (i == mFamilies.cend()) ? nullptr : make_observer(i->second.get());
}

observer_ptr<const Typeface>
FontCache::GetDefaultTypefacePtr() const
{
	// NOTE: Guaranteed to be not null for default typeface in default cache.
	// XXX: Exceptions during initialization?
	// XXX: Use %common_type?
	return pDefaultFace ? observer_ptr<const Typeface>(pDefaultFace)
		: FetchDefaultFontCache().GetDefaultTypefacePtr();
}
observer_ptr<const Typeface>
FontCache::GetTypefacePtr(const FamilyName& family_name,
	const StyleName& style_name) const
{
	return ystdex::call_value_or([&](const FontFamily& f){
		return f.GetTypefacePtr(style_name);
	}, GetFontFamilyPtr(family_name));
}

void
FontCache::Add(const FontPath& path, unique_ptr<Typeface> face)
{
	mFaces.emplace(path, std::move(face));
}

size_t
FontCache::LoadTypefaces(const FontPath& path)
{
	if(ufexists(path.c_str()) && !IO::VerifyDirectory(path))
	{
		::FT_Face face(nullptr);

		YTraceDe(Debug, "Trying loading font path using ::FT_New_Face: %s.",
			path.c_str());
		// TODO: Filter out more files earilier?
		if(::FT_New_Face(library, path.c_str(), -1, &face) != 0)
		{
			YTraceDe(Debug, "Failed loading font path: %s.", path.c_str());
			return 0;
		}

		const auto face_num(face->num_faces);

		::FT_Done_Face(face);
		YTraceDe(Informative, "Loaded faces num '%ld' from path '%s'.",
			face_num, path.c_str());
		if(face_num < 0)
			return 0;
		for(long i(0); i < face_num; ++i)
			// XXX: Conversion to 'long' might be implementation-defined.
			TryExpr(
				Add(path, make_unique<Typeface>(*this, path, std::uint32_t(i))))
			CatchExpr(..., YTraceDe(Warning, "Failed loading face of path"
				" '%s', index '%ld'.", path.c_str(), i))
		return size_t(face_num);
	}
	return 0;
}

FontFamily&
FontCache::LookupFamily(const FamilyName& name)
{
	auto& p_ff(mFamilies[name]);

	if(!p_ff)
		TryExpr(p_ff.reset(new FontFamily(name)))
		catch(...)
		{
			mFamilies.erase(name);
			throw;
		}
	return *p_ff;
}

void
FontCache::InitializeDefaultTypeface()
{
	if(YB_LIKELY(!(pDefaultFace || mFaces.empty())))
		pDefaultFace = make_observer(mFaces.cbegin()->second.get());
}


Font::Font(const FontFamily& family, const FontSize size, FontStyle fs)
	: typeface(family.GetTypefaceRef(fs)), font_size(size), style(fs)
{}

std::int8_t
Font::GetAdvance(char32_t c, CharBitmap sbit) const
{
	if(!sbit)
		sbit = GetGlyph(c, FT_LOAD_DEFAULT);
	return YB_LIKELY(sbit) ? sbit.GetXAdvance() : 0;
}
std::int8_t
Font::GetAscender() const
{
	return CvtFixed26_6ToI8(GetInternalInfo().ascender);
}
std::int8_t
Font::GetDescender() const
{
	return CvtFixed26_6ToI8(GetInternalInfo().descender);
}
CharBitmap
Font::GetGlyph(char32_t c, unsigned flags) const
{
	static_assert((FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL) == 4L,
		"Invalid default argument found.");
	const auto& face(GetTypeface());

	return &face.LookupBitmap(Typeface::BitmapKey{flags,
		face.LookupGlyphIndex(c), font_size, style});
}
FontSize
Font::GetHeight() const ynothrow
{
	return GetInternalInfo().height >> 6;
}
::FT_Size_Metrics
Font::GetInternalInfo() const
{
	// XXX: Similar to %CvtFixed26_6ToI8.
	return GetTypeface().LookupSize(GetSize()).GetSizeRec().metrics;
}

void
Font::SetSize(FontSize s)
{
	if(IsInClosedInterval(s, MinimalSize, MaximalSize))
		font_size = s;
}
bool
Font::SetStyle(FontStyle fs)
{
	auto p(GetFontFamily().GetTypefacePtr(fs));

	if(p)
	{
		yunseq(typeface = ystdex::ref(*p), style = fs);
		return true;
	}
	return {};
}

} // namespace Drawing;

#undef YF_Impl_Adaptor_Font_FreeTypeVer

} // namespace YSLib;

