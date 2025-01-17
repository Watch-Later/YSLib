﻿/*
	© 2015-2016, 2018-2022 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file range.hpp
\ingroup YStandardEx
\brief 范围操作。
\version r1225
\author FrankHB <frankhb1989@gmail.com>
\since build 624
\par 创建时间:
	2015-08-18 22:33:54 +0800
\par 修改时间:
	2022-03-21 12:08 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YStandardEx::Range
*/


#ifndef YB_INC_ystdex_range_hpp_
#define YB_INC_ystdex_range_hpp_ 1

#include "addressof.hpp" // for "addressof.hpp", <initializer_list>,
//	ystdex::addressof, enable_if_t, std::initializer_list;
#include <iterator> // for <iteartor>, std::reverse_iterator, std::begin,
//	std::end;
#include <algorithm> // for internal <algorithm>, std::distance;
// XXX: For efficiency, <valarray> is not supported here. Use %std names
//	instead.

/*!
\brief \c \<iterator> 特性测试宏。
\see ISO C++20 [version.syn] 。
\see WG21 P0941R2 2.2 。
*/
//@{
/*!
\see WG21 P0031R0 。
\see https://docs.microsoft.com/cpp/preprocessor/predefined-macros 。
\since build 834
*/
//@{
#ifndef __cpp_lib_array_constexpr
#	if (YB_IMPL_MSCPP >= 1911 && _MSVC_LANG >= 201603L) \
	|| __cplusplus >= 201603L
#		define __cpp_lib_array_constexpr 201603L
#	endif
#endif
//@}
/*!
\see LWG 2285 。
\see https://blogs.msdn.microsoft.com/vcblog/2016/01/22/vs-2015-update-2s-stl-is-c17-so-far-feature-complete/ 。
\since build 832
*/
//@{
#ifndef __cpp_lib_make_reverse_iterator
#	if YB_IMPL_MSCPP >= 1800 || __cplusplus >= 201402L
#		define __cpp_lib_make_reverse_iterator 201210L
#	endif
#endif
//@}
/*!
\see WG21 N4280 。
\see https://docs.microsoft.com/cpp/visual-cpp-language-conformance 。
\since build 834
*/
//@{
#ifndef __cpp_lib_nonmember_container_access
#	if YB_IMPL_MSCPP >= 1800 || __cplusplus >= 201411L
#		define __cpp_lib_nonmember_container_access 201411L
#	endif
#endif
//@}
//@}

namespace ystdex
{

/*!	\defgroup iterators Iterators
\brief 迭代器。
*/

/*!	\defgroup iterator_adaptors Iterator Adaptors
\ingroup iterators
\brief 迭代器适配器。
*/

//! \since build 833
inline namespace cpp2017
{

// XXX: If resolution from LWG 1052 is adopted, Microsoft VC++ 15.7 would be
//	conforming.
// NOTE: '__cpp_lib_array_constexpr >= 201603L' is not enough as LWG issues are
//	not respected, including (at least) LWG 2188 and LWG 2438. Note LWG 2188 is
//	adopted by WG21 N3936, it is effective since ISO C++14.
// NOTE: See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=86734.
// XXX: Assume non-GCC would not use libstdc++ old branch earlier for GCC 6.
#if __cplusplus >= 201703L && !YB_IMPL_MSCPP \
	&& !(__GLIBCXX__ && (__GLIBCXX__ <= 20180808 || YB_IMPL_GNUCPP < 60800))
#	define YB_Use_std_reverse_iterator true
#endif
#if YB_Use_std_reverse_iterator
using std::reverse_iterator;
#else
/*!
\note nodiscard 标记是类似 P0600R1 理由的兼容扩展。
\see WG21 P0031R0 。
*/
template<typename _tIter>
class reverse_iterator
{
private:
	using traits_type = std::iterator_traits<_tIter>;

public:
	using iterator_type = _tIter;
	using iterator_category = typename traits_type::iterator_category;
	using value_type = typename traits_type::value_type;
	using difference_type = typename traits_type::difference_type;
	using pointer = typename traits_type::pointer;
	using reference = typename traits_type::reference;

protected:
	_tIter current;

public:
	/*!
	\see LWG 235 。
	\see LWG 1012 。
	*/
	yconstfn
	reverse_iterator()
		: current()
	{}

	explicit yconstfn
	reverse_iterator(_tIter x)
		: current(x)
	{}
	template<class _tOther>
	yconstfn
	reverse_iterator(const reverse_iterator<_tOther>& u)
		: current(u.current)
	{}

	template<class _tOther>
	yconstfn_relaxed reverse_iterator&
	operator=(const reverse_iterator<_tOther>& u)
	{
		current = u.base();
		return *this;
	}

	YB_ATTR_nodiscard YB_PURE yconstfn _tIter
	base() const
	{
		return current;
	}

	YB_ATTR_nodiscard YB_PURE yconstfn_relaxed reference
	operator*() const
	{
		_tIter tmp = current;

		return *--tmp;
	}

	/*!
	\see LWG 1052 。
	\see LWG 2188 。
	*/
	YB_ATTR_nodiscard YB_PURE yconstfn pointer
	operator->() const
	{
		return ystdex::addressof(operator*());
	}

	yconstfn_relaxed reverse_iterator&
	operator++()
	{
		--current;
		return *this;
	}
	yconstfn_relaxed reverse_iterator
	operator++(int)
	{
		reverse_iterator tmp = *this;

		--current;
		return tmp;
	}

	yconstfn_relaxed reverse_iterator&
	operator--()
	{
		++current;
		return *this;
	}
	yconstfn_relaxed reverse_iterator
	operator--(int)
	{
		reverse_iterator tmp = *this;

		++current;
		return tmp;
	}

	yconstfn_relaxed reverse_iterator&
	operator+=(difference_type n)
	{
		current -= n;
		return *this;
	}

	YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator
	operator+(difference_type n) const
	{
		return reverse_iterator(current - n);
	}

	yconstfn_relaxed reverse_iterator&
	operator-=(difference_type n)
	{
		current += n;
		return *this;
	}

	YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator
	operator-(difference_type n) const
	{
		return reverse_iterator(current + n);
	}

	//! \see LWG 386 。
	YB_ATTR_nodiscard YB_PURE yconstfn yimpl(reference)
	operator[](difference_type n) const
	{
		return *(*this + n);
	}

	/*!
	\since build 834
	\see LWG 280 。
	*/
	//@{
	template<typename _tIter2>
	YB_ATTR_nodiscard YB_PURE friend yconstfn bool
	operator==(const reverse_iterator& x, const reverse_iterator<_tIter2>& y)
	{
		return x.current == y.current;
	}

	template<typename _tIter2>
	YB_ATTR_nodiscard YB_PURE friend yconstfn bool
	operator<(const reverse_iterator& x, const reverse_iterator<_tIter2>& y)
	{
		return x.current > y.current;
	}

	template<typename _tIter2>
	YB_ATTR_nodiscard YB_PURE friend yconstfn bool
	operator!=(const reverse_iterator& x, const reverse_iterator<_tIter2>& y)
	{
		return x.current != y.current;
	}

	template<typename _tIter2>
	YB_ATTR_nodiscard YB_PURE friend yconstfn bool
	operator>(const reverse_iterator& x, const reverse_iterator<_tIter2>& y)
	{
		return x.current < y.current;
	}

	template<typename _tIter2>
	YB_ATTR_nodiscard YB_PURE friend yconstfn bool
	operator>=(const reverse_iterator& x, const reverse_iterator<_tIter2>& y)
	{
		return x.current <= y.current;
	}

	template<typename _tIter2>
	YB_ATTR_nodiscard YB_PURE friend yconstfn bool
	operator<=(const reverse_iterator& x, const reverse_iterator<_tIter2>& y)
	{
		return x.current >= y.current;
	}
	//@}

	YB_ATTR_nodiscard YB_PURE friend yconstfn reverse_iterator
	operator+(difference_type n, const reverse_iterator& x)
	{
		return reverse_iterator(x.current - n);
	}
};

/*!
\relates reverse_iterator
\since build 844
*/
template<typename _tIter, typename _tIter2>
YB_ATTR_nodiscard YB_PURE yconstfn auto
operator-(const reverse_iterator<_tIter>& x, const reverse_iterator<_tIter2>& y)
	-> decltype(y.base() - x.base())
{
	return y.base() - x.base();
}
#endif


#define YB_Impl_Range_DisambiguousParams \
	yimpl(typename... _tParams, \
	typename = enable_if_t<sizeof...(_tParams) == 0>)
#define YB_Impl_Range_DisambiguousArgs yimpl(_tParams&&...)

#if __cpp_lib_array_constexpr >= 201603L
//! \since build 624
using std::begin;
//! \since build 624
using std::end;
#else
template<class _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
begin(_tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(c.begin())
{
	return c.begin();
}
template<class _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
begin(const _tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(c.begin())
{
	return c.begin();
}
// NOTE: To avoid ambiguity, overloads for arrays are reimplemented here even
//	they do not change in P0031R0.
//! \since build 834
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn _type*
begin(_type(&array)[_vN]) ynothrow
{
	return array;
}

//! \since build 936
template<class _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
end(_tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(c.end())
{
	return c.end();
}
//! \since build 936
template<class _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
end(const _tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(c.end())
{
	return c.end();
}
// NOTE: Ditto.
//! \since build 834
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn _type*
end(_type(&array)[_vN]) ynothrow
{
	return array + _vN;
}
#endif

} // inline namespace cpp2017;

/*!
\ingroup YBase_replacement_features
\since build 833
*/
//@{
//! \since build 664
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn _type*
begin(_type(&&array)[_vN]) ynothrow
{
	return array;
}

//! \since build 664
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn _type*
end(_type(&&array)[_vN]) ynothrow
{
	return array + _vN;
}

template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator<_type*>
rbegin(_type(&&array)[_vN])
{
	return reverse_iterator<_type*>(array + _vN);
}

template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator<_type*>
rend(_type(&&array)[_vN])
{
	return reverse_iterator<_type*>(array);
}
//@}

//! \since build 834
inline namespace cpp2017
{

//! \since build 624
//@{
// XXX: To prevent introduce non-'constexpr' version of %std::(begin, end),
//	%__cpp_lib_array_constexpr is also checked, even the direct interface is not
//	changed from the original version in ISO C++14.
#if __cpp_lib_array_constexpr >= 201603L \
	&& ((__cpp_lib_nonmember_container_access >= 201411L \
	|| (__cplusplus >= 201402L && (!defined(__GLIBCXX__) \
	|| __GLIBCXX__ > 20150119))) || (_LIBCPP_VERSION >= 1101 \
	&& _LIBCPP_STD_VER > 11) || YB_IMPL_MSCPP >= 1800)
using std::cbegin;
using std::cend;
using std::rbegin;
using std::rend;
using std::crbegin;
using std::crend;
#else
//! \since build 938
//@{
template<typename _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
cbegin(const _tRange& c, YB_Impl_Range_DisambiguousArgs)
	ynoexcept_spec(ystdex::begin(c)) -> decltype(ystdex::begin(c))
{
	return ystdex::begin(c);
}

template<typename _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
cend(const _tRange& c, YB_Impl_Range_DisambiguousArgs)
	ynoexcept_spec(ystdex::end(c)) -> decltype(ystdex::end(c))
{
	return ystdex::end(c);
}

template<class _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
rbegin(_tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(c.rbegin())
{
	return c.rbegin();
}
template<class _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
rbegin(const _tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(c.rbegin())
{
	return c.rbegin();
}
//@}
//! \since build 833
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator<_type*>
rbegin(_type(&array)[_vN])
{
	return reverse_iterator<_type*>(array + _vN);
}
//! \since build 833
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator<const _tElem*>
rbegin(std::initializer_list<_tElem> il)
{
	return reverse_iterator<const _tElem*>(il.end());
}

//! \since build 938
template<class _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
rend(_tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(c.rend())
{
	return c.rend();
}
//! \since build 938
template<class _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
rend(const _tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(c.rend())
{
	return c.rend();
}
//! \since build 833
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator<_type*>
rend(_type(&array)[_vN])
{
	return reverse_iterator<_type*>(array);
}
//! \since build 833
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator<const _tElem*>
rend(std::initializer_list<_tElem> il)
{
	return reverse_iterator<const _tElem*>(il.begin());
}

//! \since build 938
template<typename _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
crbegin(const _tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(ystdex::rbegin(c))
{
	return ystdex::rbegin(c);
}

//! \since build 938
template<typename _tRange, YB_Impl_Range_DisambiguousParams>
YB_ATTR_nodiscard YB_PURE yconstfn auto
crend(const _tRange& c, YB_Impl_Range_DisambiguousArgs) -> decltype(ystdex::rend(c))
{
	return ystdex::rend(c);
}
#endif
//@}

#undef YB_Impl_Range_DisambiguousArgs
#undef YB_Impl_Range_DisambiguousParams

} // inline namespace cpp2017;

/*!
\ingroup YBase_replacement_extensions
\brief 初值符列表常量迭代器访问重载扩展。
\since build 664

访问 std::initializer_list 的实例是特殊的，因为它们只有成员 begin 和 end ，
	而没有其它的访问。逆向和常量访问都依赖非成员函数。
通过 CWG 1591 的解决可以在存在数组类型参数的重载时推导 std::initializer_list 实例，
	但标准库中，只有非常量的访问有数组类型的重载（且非常量的访问同时对
	std::initializer_list 已有针对性的重载而不需要依赖 CWG 1591 的解决），
	所以不能帮助推导需要常量访问初值符列表的情形。
因此，在此提供扩展，以允许和其它方式相同的方式直接以初值符调用这些访问函数模板。
*/
//@{
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn const _tElem*
cbegin(std::initializer_list<_tElem> il) ynothrow
{
	return il.begin();
}

template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn const _tElem*
cend(std::initializer_list<_tElem> il) ynothrow
{
	return il.end();
}

//! \since build 833
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator<const _tElem*>
crbegin(std::initializer_list<_tElem> il) ynothrow
{
	return ystdex::rbegin(il);
}

//! \since build 833
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn reverse_iterator<const _tElem*>
crend(std::initializer_list<_tElem> il) ynothrow
{
	return ystdex::rend(il);
}
//@}

/*!
\brief 类容器访问。
\see WG21 N4280 。
\see ISO C++17 [iterator.container] 。
\since build 663
*/
//@{
//! \since build 834
inline namespace cpp2017
{

#if __cpp_lib_nonmember_container_access >= 201411L
using std::size;
using std::empty;
using std::data;
#else
template<class _tRange>
yconstfn auto
size(const _tRange& c) -> decltype(c.size())
{
	return c.size();
}
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn size_t
size(const _type(&)[_vN]) ynothrow
{
	return _vN;
}
#if __cplusplus <= 201402L
//! \see CWG 1591 。
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn size_t
size(std::initializer_list<_tElem> il) ynothrow
{
	return il.size();
}
#endif

template<class _tRange>
YB_ATTR_nodiscard YB_PURE yconstfn auto
empty(const _tRange& c) -> decltype(c.empty())
{
	return c.empty();
}
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn bool
empty(const _type(&)[_vN]) ynothrow
{
	return {};
}
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn bool
empty(std::initializer_list<_tElem> il) ynothrow
{
	return il.size() == 0;
}

template<typename _tRange>
YB_ATTR_nodiscard YB_PURE yconstfn auto
data(_tRange& c) -> decltype(c.data())
{
	return c.data();
}
template<typename _tRange>
YB_ATTR_nodiscard YB_PURE yconstfn auto
data(const _tRange& c) -> decltype(c.data())
{
	return c.data();
}
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn _type*
data(_type(&array)[_vN]) ynothrow
{
	return array;
}
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE yconstfn const _tElem*
data(std::initializer_list<_tElem> il) ynothrow
{
	return il.begin();
}
#endif

} // inline namespace cpp2017;

//! \ingroup YBase_replacement_features
template<typename _type, size_t _vN>
YB_ATTR_nodiscard YB_PURE yconstfn _type*
data(_type(&&array)[_vN]) ynothrow
{
	return array;
}
//@}

/*!
\ingroup metafunctions
\since build 899
*/
//@{
template<typename _tRange>
using range_iterator_t = decltype(ystdex::begin(std::declval<_tRange&>()));

template<typename _tRange>
using range_const_iterator_t
	= decltype(ystdex::cbegin(std::declval<const _tRange&>()));
//@}

//! \since build 546
namespace details
{

//! \since build 663
template<class _type>
using range_size_t = decltype(size(std::declval<_type>()));

//! \since build 663
template<class _type>
using has_range_size = is_detected_convertible<size_t, range_size_t, _type>;

//! \since buld 730
template<typename _tRange>
yconstfn auto
range_size(const _tRange& c, true_) -> decltype(size(c))
{
	return size(c);
}
//! \since buld 730
template<typename _tRange>
inline auto
range_size(const _tRange& c, false_)
	-> decltype(std::distance(ystdex::begin(c), ystdex::end(c)))
{
	return std::distance(ystdex::begin(c), ystdex::end(c));
}

} // namespace details;

/*!
\ingroup algorithms
\pre 参数指定的迭代器范围（若存在）有效。
\note 参数 \c first 和 \c last 指定迭代器范围。
\note 对不以迭代器指定的范围，使用 ystdex::begin 和 ystdex::end 取迭代器。
\note 确定为 const 迭代器时使用 ystdex::cbegin 和 ystdex::cend 代替。
*/
//@{
/*!
\brief 取范围大小。
\since build 546

对 std::initializer_list 的实例直接返回大小，否则：
若可调用结果可转换为 size_t 的 ADL 函数 size 则使用 ADL size ；
否则使用 std::distance 计算范围迭代器确定范围大小。
*/
//@{
template<typename _tRange>
yconstfn auto
range_size(const _tRange& c)
	-> decltype(details::range_size(c, details::has_range_size<_tRange>()))
{
	return details::range_size(c, details::has_range_size<_tRange>());
}
template<typename _tElem>
yconstfn size_t
range_size(std::initializer_list<_tElem> il)
{
	return size(il);
}
//@}

} // namespace ystdex;

#endif

