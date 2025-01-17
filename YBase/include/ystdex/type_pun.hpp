﻿/*
	© 2014-2016, 2018, 2021 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file type_pun.hpp
\ingroup YStandardEx
\brief 共享存储和直接转换。
\version r578
\author FrankHB <frankhb1989@gmail.com>
\since build 629
\par 创建时间:
	2015-09-04 12:16:27 +0800
\par 修改时间:
	2021-11-22 04:03 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YStandardEx::TypePun
*/


#ifndef YB_INC_ystdex_type_pun_hpp_
#define YB_INC_ystdex_type_pun_hpp_ 1

#include "placement.hpp" // for internal "placement.hpp", is_trivially_copyable,
//	bool_, yalignof, and_, is_trivial, enable_if_t, is_object_pointer,
//	remove_pointer_t, is_object, aligned_storage_t, is_reference,
//	remove_reference_t, placement ::operator new from standard library, decay_t,
//	exclude_self_t, ystdex::addressof;

namespace ystdex
{

/*!	\defgroup aligned_type_traits Aligned Type Traits
\ingroup binary_type_traits
\brief 对齐类型特征。
\note 第一参数类型比第二参数类型对齐要求更严格或相同。
\since build 630
*/
//@{
/*!
\brief 判断是否对齐兼容。
\since build 610
*/
template<typename _type, typename _type2>
struct is_aligned_compatible : bool_<!(yalignof(_type) < yalignof(_type2))
	&& yalignof(_type) % yalignof(_type2) == 0>
{};


/*!
\brief 判断是否可放置对齐存储。
\since build 610
*/
template<typename _type, typename _tDst>
struct is_aligned_placeable : bool_<sizeof(_type) == sizeof(_tDst)
	&& is_aligned_compatible<_type, _tDst>::value>
{};


/*!
\brief 判断是否可对齐存储。
\since build 503
*/
template<typename _type, typename _tDst>
struct is_aligned_storable : bool_<!(sizeof(_type) < sizeof(_tDst))
	&& is_aligned_compatible<_type, _tDst>::value>
{};


/*!
\brief 判断是否可替换对齐存储。
\since build 610
*/
template<typename _type, typename _tDst>
struct is_aligned_replaceable : and_<is_aligned_storable<_type, _tDst>,
	is_aligned_storable<_tDst, _type>>
{};


/*!
\brief 判断是否可平凡替换存储。
\todo 使用 \c is_trivially_copyable 代替 is_trivial 。
*/
template<typename _type, typename _tDst>
struct is_trivially_replaceable : and_<is_trivial<_type>, is_trivial<_tDst>,
	is_aligned_replaceable<_type, _tDst>>
{};
//@}


//! \since build 630
//@{
/*!
\ingroup metafunctions
\brief 选择和特定类型可替换字符类类型的特定重载以避免冲突。
*/
template<typename _tSrc, typename _tDst, typename _type = void>
using enable_if_replaceable_t
	= enable_if_t<is_trivially_replaceable<_tSrc, _tDst>::value, _type>;


/*!
\ingroup transformation_traits
\brief 显式转换的替代对齐存储对齐存储类型。
*/
template<typename _type, size_t _vAlign = yalignof(_type)>
using pun_storage_t = aligned_storage_t<sizeof(_type), _vAlign>;


/*!
\ingroup cast
\pre 目标类型是对象指针或引用。
\note 使用 \c reinterpret_cast 且保证目标类型的对齐要求不比源类型更严格以保证可逆。
*/
//@{
/*!
\brief 保证对齐兼容的显式转换。
\note 用于替代针对满足 is_aligned_compatible 要求对象的 \c reinterpret_cast 。
*/
//@{
template<typename _pDst, typename _tSrc>
YB_ATTR_always_inline inline yimpl(enable_if_t)<and_<is_object_pointer<_pDst>,
	is_aligned_compatible<_tSrc, remove_pointer_t<_pDst>>>::value, _pDst>
aligned_cast(_tSrc* v) ynothrow
{
	return reinterpret_cast<_pDst>(v);
}
template<typename _rDst, typename _tSrc, yimpl(typename = ystdex::enable_if_t<
	and_<is_reference<_rDst>, is_aligned_compatible<remove_reference_t<_tSrc>,
	remove_reference_t<_rDst>>>::value>)>
YB_ATTR_always_inline inline auto
aligned_cast(_tSrc&& v) ynothrow
	-> decltype(yforward(reinterpret_cast<_rDst>(v)))
{
	return yforward(reinterpret_cast<_rDst>(v));
}
//@}


/*!
\brief 保证对齐存储的显式转换。
\note 用于替代针对满足 is_aligned_storable 要求对象的 \c reinterpret_cast 。
*/
//@{
template<typename _pDst, typename _tSrc>
YB_ATTR_always_inline inline yimpl(enable_if_t)<and_<is_object_pointer<_pDst>,
	is_aligned_storable<_tSrc, remove_pointer_t<_pDst>>>::value, _pDst>
aligned_store_cast(_tSrc* v) ynothrow
{
	return reinterpret_cast<_pDst>(v);
}
template<typename _rDst, typename _tSrc, yimpl(typename = ystdex::enable_if_t<
	and_<is_reference<_rDst>, is_aligned_storable<remove_reference_t<_tSrc>,
	remove_reference_t<_rDst>>>::value>)>
YB_ATTR_always_inline inline auto
aligned_store_cast(_tSrc&& v) ynothrow
	-> decltype(yforward(reinterpret_cast<_rDst>(v)))
{
	return yforward(reinterpret_cast<_rDst>(v));
}
//@}


/*!
\brief 保证对齐替换存储的显式转换。
\note 用于替代针对满足 is_aligned_replaceable 要求对象的 \c reinterpret_cast 。
*/
//@{
template<typename _pDst, typename _tSrc>
YB_ATTR_always_inline inline yimpl(enable_if_t)<and_<is_object_pointer<_pDst>,
	is_aligned_replaceable<_tSrc, remove_pointer_t<_pDst>>>::value, _pDst>
aligned_replace_cast(_tSrc* v) ynothrow
{
	return reinterpret_cast<_pDst>(v);
}
template<typename _rDst, typename _tSrc, yimpl(typename = ystdex::enable_if_t<
	and_<is_reference<_rDst>, is_aligned_replaceable<remove_reference_t<_tSrc>,
	remove_reference_t<_rDst>>>::value>)>
YB_ATTR_always_inline inline auto
aligned_replace_cast(_tSrc&& v) ynothrow
	-> decltype(yforward(reinterpret_cast<_rDst>(v)))
{
	return yforward(reinterpret_cast<_rDst>(v));
}
//@}


/*!
\brief 保证平凡替换存储的显式转换。
\note 用于替代针对满足 is_trivially_replaceable 要求对象的 \c reinterpret_cast 。
\since build 629
*/
//@{
template<typename _pDst, typename _tSrc>
YB_ATTR_always_inline inline yimpl(enable_if_t)<and_<is_object_pointer<_pDst>,
	is_trivially_replaceable<_tSrc, remove_pointer_t<_pDst>>>::value, _pDst>
replace_cast(_tSrc* v) ynothrow
{
	return reinterpret_cast<_pDst>(v);
}
template<typename _rDst, typename _tSrc, yimpl(typename = ystdex::enable_if_t<
	and_<is_reference<_rDst>, is_trivially_replaceable<remove_reference_t<_tSrc>,
	remove_reference_t<_rDst>>>::value>)>
YB_ATTR_always_inline inline auto
replace_cast(_tSrc&& v) ynothrow
	-> decltype(yforward(reinterpret_cast<_rDst>(v)))
{
	return yforward(reinterpret_cast<_rDst>(v));
}
//@}
//@}
//@}


/*!
\brief 避免严格别名分析限制的缓冲引用。
\tparam _type 完整或不完整的对象类型。
\since build 705
*/
template<typename _type>
class pun_ref
{
	static_assert(is_object<_type>(), "Invalid type found.");

private:
	//! \invariant \c alias 。
	_type& alias;

public:
	//! \pre 实例化时对象类型完整。
	//@{
	/*!
	\pre 指针参数非空。
	\since build 716
	*/
	//@{
	YB_NONNULL(3)
	pun_ref(default_init_t, void* p)
		: alias(*::new(p) _type)
	{}
	template<typename... _tParams>
	YB_NONNULL(2)
	pun_ref(void* p, _tParams&&... args)
		: alias(*::new(p) _type(yforward(args)...))
	{}
	//@}
	~pun_ref()
	{
		alias.~_type();
	}
	//@}

	YB_ATTR_nodiscard YB_ATTR_always_inline _type&
	get() const ynothrow
	{
		return alias;
	}
};



//! \since build 716
//@{
//! \brief 避免严格别名分析限制的缓冲独占所有权指针。
template<typename _type>
using pun_ptr = placement_ptr<decay_t<_type>>;


//! \brief 构造避免严格别名分析限制的缓冲独占所有权指针指定的值初始化对象。
template<typename _type, typename _tObj, typename... _tParams>
YB_ATTR_nodiscard inline pun_ptr<_type>
make_pun(_tObj& obj, _tParams&&... args)
{
	return pun_ptr<_type>(
		ystdex::construct_within<decay_t<_type>>(obj, yforward(args)...));
}

/*!
\brief 构造避免严格别名分析限制的缓冲独占所有权指针指定的默认初始化对象。
\since build 717
*/
template<typename _type, typename _tObj>
YB_ATTR_nodiscard inline pun_ptr<_type>
make_pun_default(_tObj& obj)
{
	return
		pun_ptr<_type>(ystdex::construct_default_within<decay_t<_type>>(obj));
}
//@}


/*!
\brief 任意标准布局类型对象提供空间的存储。
\note 可能储存类型非标准布局类型对象。
\see WG21 N4606 3.8[basic.life] 。
\warning 应注意避免违反对象生存期规则。
\since build 692
*/
template<typename _tUnderlying = aligned_storage_t<sizeof(void*)>>
struct standard_layout_storage
{
	static_assert(is_standard_layout<_tUnderlying>(),
		"Invalid underlying type found.");

	using underlying = _tUnderlying;

	underlying object;

	standard_layout_storage() = default;
	standard_layout_storage(const standard_layout_storage&) = default;

	standard_layout_storage&
	operator=(const standard_layout_storage&) = default;
	/*!
	\note 为避免类型错误，需要确定类型时应使用显式使用 access 指定类型赋值。
	\since build 454
	*/
	template<typename _type,
		yimpl(typename = exclude_self_t<standard_layout_storage, _type>)>
	inline standard_layout_storage&
	operator=(_type&& x)
	{
		assign(yforward(x));
		return *this;
	}

	YB_ATTR_nodiscard YB_ATTR_always_inline YB_PURE yconstfn_relaxed void*
	access() ynothrow
	{
		return ystdex::addressof(object);
	}
	YB_ATTR_nodiscard YB_ATTR_always_inline YB_PURE yconstfn const void*
	access() const ynothrow
	{
		return ystdex::addressof(object);
	}
	//! \warning 应注意避免违反严格别名分析规则。
	//@{
	template<typename _type>
	YB_ATTR_nodiscard YB_ATTR_always_inline YB_PURE yconstfn_relaxed _type&
	access() ynothrow
	{
		static_assert(is_aligned_storable<standard_layout_storage, _type>(),
			"Invalid type found.");

		return *static_cast<_type*>(access());
	}
	template<typename _type>
	YB_ATTR_nodiscard YB_ATTR_always_inline YB_PURE yconstfn const _type&
	access() const ynothrow
	{
		static_assert(is_aligned_storable<standard_layout_storage, _type>(),
			"Invalid type found.");

		return *static_cast<const _type*>(access());
	}
	//@}

	//! \since build 716
	//@{
	//! \pre 对象未通过 construct 或构造模板创建。
	template<typename _type, typename... _tParams>
	inline _type*
	construct(_tParams&&... args)
	{
		return
			ystdex::construct_within<decay_t<_type>>(object, yforward(args)...);
	}

	//! \pre 指定类型的对象已通过 construct 或构造模板创建。
	template<typename _type>
	inline void
	destroy()
	{
		ystdex::destruct_in(access<decay_t<_type>>());
	}

	//! \pre 对象未通过 construct 或构造模板创建。
	//@{
	template<typename _type, typename... _tParams>
	YB_ATTR_nodiscard inline pun_ptr<_type>
	pun(_tParams&&... args)
	{
		return ystdex::make_pun<_type>(object, yforward(args)...);
	}

	//! \since build 717
	template<typename _type, typename... _tParams>
	YB_ATTR_nodiscard inline pun_ptr<_type>
	pun_default()
	{
		return ystdex::make_pun_default<_type>(object);
	}
	//@}
	//@}

	YB_ATTR_nodiscard YB_ATTR_always_inline YB_PURE yconstfn_relaxed byte*
	data() ynothrow
	{
		return static_cast<byte*>(access());
	}
	YB_ATTR_nodiscard YB_ATTR_always_inline YB_PURE yconstfn const byte*
	data() const ynothrow
	{
		return static_cast<const byte*>(access());
	}

	/*!
	\pre 指定类型的对象已通过 construct 或构造模板创建。
	\since build 503
	*/
	template<typename _type>
	inline void
	assign(_type&& x)
	{
		access<decay_t<_type>>() = yforward(x);
	}
};


/*!
\ingroup transformation_traits
\brief 显式转换的替代标准布局替代存储类型。
\since build 693
*/
template<typename _type, size_t _vAlign = yalignof(_type)>
using replace_storage_t
	= standard_layout_storage<pun_storage_t<_type, _vAlign>>;

} // namespace ystdex;

#endif

