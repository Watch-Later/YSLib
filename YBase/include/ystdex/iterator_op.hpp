﻿/*
	© 2013-2016, 2018-2022 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file iterator_op.hpp
\ingroup YStandardEx
\brief 迭代器操作。
\version r280
\author FrankHB <frankhb1989@gmail.com>
\since build 576
\par 创建时间:
	2015-02-09 11:28:52 +0800
\par 修改时间:
	2022-06-05 01:38 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YStandardEx::IteratorOperation
*/


#ifndef YB_INC_ystdex_iterator_op_hpp_
#define YB_INC_ystdex_iterator_op_hpp_ 1

#include "range.hpp" // for "range.hpp", std::next, std::prev,
//	std::make_move_iterator, std::reverse_iterator, std::make_pair, begin, end;
#include "deref_op.hpp" // for "deref_op.hpp", yconstraint, YB_VerifyIterator;
#include "operators.hpp" // for input_iteratable, output_iteratable,
//	forward_iteratable, bidirectional_iteratable, random_access_iteratable, _t;

namespace ystdex
{

/*!	\defgroup iterator_operations Iterator Operations
\ingroup iterators
\brief 迭代器操作。
\since build 375
*/
//@{
/*!
\brief 迭代器指向的值满足条件时取邻接迭代器，否则取原值。
\param i 指定的迭代器。
\pre 迭代器可解引用，蕴含断言：满足 YB_VerifyIterator 。
*/
//@{
template<typename _tIn, typename _fPred>
_tIn
next_if(_tIn i, _fPred f,
	typename std::iterator_traits<_tIn>::difference_type n = 1)
{
	YB_VerifyIterator(i);
	return f(*i) ? std::next(i, n) : i;
}
template<typename _tIn, typename _type>
_tIn
next_if_eq(_tIn i, const _type& val,
	typename std::iterator_traits<_tIn>::difference_type n = 1)
{
	YB_VerifyIterator(i);
	return *i == val ? std::next(i, n) : i;
}
//@}

/*!
\brief 迭代器指向的值满足条件时取反向邻接迭代器，否则取原值。
\pre 迭代器可解引用。
*/
//@{
template<typename _tBi, typename _fPred>
_tBi
prev_if(_tBi i, _fPred f,
	typename std::iterator_traits<_tBi>::difference_type n = 1)
{
	return f(*i) ? std::prev(i, n) : i;
}
template<typename _tBi, typename _type>
_tBi
prev_if_eq(_tBi i, const _type& val,
	typename std::iterator_traits<_tBi>::difference_type n = 1)
{
	return *i == val ? std::prev(i, n) : i;
}
//@}
//@}


//! \since build 833
inline namespace cpp2014
{

/*!
\brief 构造反向迭代器。
\see WG21 N3936 24.5.1.3.21[reverse.iter.make] 。
\see LWG 2285 。
\since build 531
*/
#if __cpp_lib_make_reverse_iterator >= 201402L && YB_Use_std_reverse_iterator
using std::make_reverse_iterator;
#else
//! \since build 833
template<typename _tIter>
YB_ATTR_nodiscard YB_STATELESS reverse_iterator<_tIter>
make_reverse_iterator(_tIter i)
{
	return reverse_iterator<_tIter>(i);
}
#endif

} // inline namespace cpp2014;

//! \ingroup helper_functions
//@{
/*!
\brief 按 std::move_if_noexcept 的条件构造转移迭代器或非转移迭代器。
\since build 866
*/
template<typename _tIter, typename _tRet = cond_t<is_throwing_move_copyable<
	typename std::iterator_traits<_tIter>::value_type>, _tIter,
	std::move_iterator<_tIter>>>
YB_ATTR_nodiscard YB_STATELESS yconstfn _tRet
make_move_if_noexcept_iterator(_tIter it)
{
	return _tRet(it);
}

/*!
\brief 构造转移迭代器对。
\since build 337
*/
template<typename _tIter1, typename _tIter2>
YB_ATTR_nodiscard YB_STATELESS inline auto
make_move_iterator_pair(_tIter1 it1, _tIter2 it2) -> decltype(
	std::make_pair(std::make_move_iterator(it1), std::make_move_iterator(it2)))
{
	return std::make_pair(std::make_move_iterator(it1),
		std::make_move_iterator(it2));
}
/*!
\brief 构造指定序列范围（包含序列容器及内建数组等）的转移迭代器对。
\note 使用 ADL begin 和 end 指定范围迭代器。
\since build 337
*/
template<typename _tRange>
YB_ATTR_nodiscard YB_STATELESS inline auto
make_move_iterator_pair(_tRange& c)
	-> decltype(ystdex::make_move_iterator_pair(begin(c), end(c)))
{
	return ystdex::make_move_iterator_pair(begin(c), end(c));
}
//@}


//! \since build 576
namespace details
{

template<typename _tIter, typename _tTraits,
	typename _tTag = typename _tTraits::iterator_category>
struct iterator_operators;

template<typename _tIter, typename _tTraits>
struct iterator_operators<_tIter, _tTraits, std::input_iterator_tag>
{
	using type = input_iteratable<_tIter, typename _tTraits::reference>;
};

template<typename _tIter, typename _tTraits>
struct iterator_operators<_tIter, _tTraits, std::output_iterator_tag>
{
	using type = output_iteratable<_tIter, typename _tTraits::reference>;
};

template<typename _tIter, typename _tTraits>
struct iterator_operators<_tIter, _tTraits, std::forward_iterator_tag>
{
	using type = forward_iteratable<_tIter, typename _tTraits::reference>;
};

template<typename _tIter, typename _tTraits>
struct iterator_operators<_tIter, _tTraits, std::bidirectional_iterator_tag>
{
	using type = bidirectional_iteratable<_tIter, typename _tTraits::reference>;
};

template<typename _tIter, typename _tTraits>
struct iterator_operators<_tIter, _tTraits, std::random_access_iterator_tag>
{
	using type = random_access_iteratable<_tIter,
		typename _tTraits::difference_type, typename _tTraits::reference>;
};

} // namespace details;


/*!
\ingroup metafunctions
\brief 按迭代器类别取可实现迭代器的重载操作符集合的实现。
\note 仅使用第二参数的特定成员，可以是兼容 std::iterator_tratis 的实例的类型。
\warning 实例作为实现迭代器的基类时不应使用默认参数，因为此时无法访问成员类型。
\since build 576
*/
template<typename _tIter, typename _tTraits = std::iterator_traits<_tIter>>
using iterator_operators_t = _t<details::iterator_operators<_tIter, _tTraits>>;

} // namespace ystdex;

#endif

