﻿/*
	© 2012-2015 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file NativeAPI.cpp
\ingroup YCLib
\brief 通用平台应用程序接口描述。
\version r932
\author FrankHB <frankhb1989@gmail.com>
\since build 296
\par 创建时间:
	2012-03-26 13:36:28 +0800
\par 修改时间:
	2015-06-29 15:32 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YCLib::NativeAPI
*/


#include "YCLib/YModules.h"
#include YFM_YCLib_NativeAPI
#if YCL_DS
#	include YFM_DS_YCLib_DSIO // for ::DISC_INTERFACE, Disc,
//	platform_ex::FAT::Mount;
#	include <arm9/dldi.h> // for ::_io_dldi_stub;


//! \since build 602
extern "C" ::DLDI_INTERFACE _io_dldi_stub;
#elif YCL_MinGW
#	include <ctime> // for std::gmtime;
#endif

namespace
{

}

#if YCL_DS
namespace platform_ex
{

//! \since build 602
//@{
using namespace platform::Descriptions;

namespace
{

enum class DSFATRoot
{
	Null,
	SD,
	FAT
};
//@}

//! \since build 611;
const char* init_dev;

} // unnamed namespace;

bool
InitializeFileSystem() ynothrow
{
	if(init_dev)
		return {};

	// NOTE: %DEFAULT_CACHE_PAGES is 16 in "common.h" in libfat source.
	const auto res([](size_t pages) ynothrow -> DSFATRoot{
		const auto init([=](const char* name, const ::DISC_INTERFACE& disc_io)
			ynothrow -> bool{
			// NOTE: %DEFAULT_SECTORS_PAGE is 8 in "common.h" in libfat source.
			//	This is also the minimal value used by the cache. So the shift
			//	is 3.
			TryRet(platform_ex::FAT::Mount(name, disc_io, 0, pages, 3))
			CatchExpr(std::exception& e, YCL_TraceRaw(Warning,
				"FATMount failure[%s]: %s", typeid(e).name(), e.what()))
			CatchExpr(..., YCL_TraceRaw(Emergent,
				"Unknown exception found @ InitializeFileSystem."))
			return {};
		});

		if(init("sd", ::__io_dsisd))
			return DSFATRoot::SD;
		// NOTE: As %::dldiGetInternal.
		if((::_io_dldi_stub.ioInterface.features
			& (FEATURE_SLOT_GBA | FEATURE_SLOT_NDS)) != 0)
			// NOTE: As %::sysSetCardOwner(BUS_OWNER_ARM9), with one less access
			//	to the volatile lvalue.
			REG_EXMEMCNT &= ~ARM7_OWNS_CARD;
		if(init("fat", ::_io_dldi_stub.ioInterface))
			return DSFATRoot::FAT;
		return DSFATRoot::Null;
	}(16));

	if(res != DSFATRoot::Null)
	{
		init_dev = res == DSFATRoot::FAT ? "fat" : "sd";
		// NOTE: No %ARGV_MAGIC here as libnds does.
		::chdir(res == DSFATRoot::FAT ? "fat:/" : "sd:/");
		return true;
	}
	return {};
}

bool
UninitializeFileSystem() ynothrow
{
	const bool res(init_dev ? FAT::Unmount(init_dev) : false);

	init_dev = {};
	return res;
}

} // namespace platform_ex;
#elif YCL_MinGW
extern "C"
{

#	if defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
// See FileSystem.cpp.
//! \since build 465
struct ::tm* __cdecl __MINGW_NOTHROW
_gmtime32(const ::__time32_t* p)
{
	return std::gmtime(p);
}
#	endif

} // extern "C";

#endif

namespace platform
{
}

