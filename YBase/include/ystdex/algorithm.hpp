﻿/*
	© 2010-2021 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file algorithm.hpp
\ingroup YStandardEx
\brief 泛型算法。
\version r1237
\author FrankHB <frankhb1989@gmail.com>
\since build 254
\par 创建时间:
	2010-05-23 06:10:59 +0800
\par 修改时间:
	2021-12-28 18:51 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YStandardEx::Algorithm
*/


#ifndef YB_INC_ystdex_algorithm_hpp_
#define YB_INC_ystdex_algorithm_hpp_ 1

#include "iterator_trait.hpp" // for have_same_iterator_category;
#include <numeric> // for std::accumulate;
#include <algorithm> // for <algorithm>, __cpp_lib_clamp,
//	__cpp_lib_robust_nonmodifying_seq_ops;
#include "functor.hpp" // for is_equal, less, std::bind, std::placeholders::_1;
#include "deref_op.hpp" // for YB_VerifyIterator, yconstraint;
#include <cstring> // for std::memcpy, std::memmove;

/*!
\brief \c \<algorithm> 特性测试宏。
\see ISO C++20 [version.syn] 。
\see WG21 P0941R2 2.2 。
\see https://blogs.msdn.microsoft.com/vcblog/2016/10/11/c1417-features-and-stl-fixes-in-vs-15-preview-5/ 。
*/
//@{
/*!
\see https://dev.to/yumetodo/list-of-mscver-and-mscfullver-8nd 。
\see https://docs.microsoft.com/cpp/preprocessor/predefined-macros 。
\since build 835
*/
//@{
#ifndef __cpp_lib_clamp
#	if (_MSC_FULL_VER >= 190024210L && _MSVC_LANG >= 201603L) \
	|| __cplusplus >= 201603L
#		define __cpp_lib_clamp 201603L
#	endif
#endif
//@}
//! \since build 628
//@{
#ifndef __cpp_lib_robust_nonmodifying_seq_ops
#	if YB_IMPL_MSCPP >= 1900 || __cplusplus >= 201304L
#		define __cpp_lib_robust_nonmodifying_seq_ops 201304L
#	endif
#endif
//@}
//@}

namespace ystdex
{

/*!	\defgroup algorithms Gerneral Algorithms
\brief 算法。
\note 范围包含序列容器及内建数组等。容器概念和容器要求参见 ISO C++ Clause 23 。
\see WG21 N3936 25.1[algorithms.general] 。
\since build 189
*/
//@{
/*!	\defgroup nonmodifying_algorithms Non-modifying Sequence Operations
\brief 非修改序列操作。
\see WG21 N3936 25.2[alg.nonmodifying] 。
\since build 531
*/
//@{
/*!
\brief 快速序列判断操作。
\note 保证从序列起始判断，尽早确定返回值，保证若干个值被判断一次，之后的值不被判断。
\since build 675
*/
//@{
//! \note 若返回值，同 std::all_of 的返回结果。
template<typename _tIn, typename _fPred>
bool
fast_all_of(_tIn first, _tIn last, _fPred pred)
{
	while(first != last && bool(pred(*first)))
		++first;
	return first == last;
}

//! \note 若返回值，同 std::any_of 的返回结果。
template<typename _tIn, typename _fPred>
bool
fast_any_of(_tIn first, _tIn last, _fPred pred)
{
	while(first != last && !bool(pred(*first)))
		++first;
	return first != last;
}

//! \note 若返回值，同 std::none_of 的返回结果。
template<typename _tIn, typename _fPred>
bool
fast_none_of(_tIn first, _tIn last, _fPred pred)
{
	return !ystdex::fast_any_of(first, last, pred);
}
//@}


/*!
\brief 严格序列判断操作。
\note 保证从序列起始判断，返回时序列中的每个值被判断一次。
\since build 675
*/
//@{
//! \note 若返回值，同 std::all_of 的返回结果。
template<typename _tIn, typename _fPred>
bool
strict_all_of(_tIn first, _tIn last, _fPred pred)
{
	return std::accumulate(first, last, true,
		[pred](bool b, decltype(*first) val){
		return b && bool(pred(yforward(val)));
	});
}

//! \note 若返回值，同 std::any_of 的返回结果。
template<typename _tIn, typename _fPred>
bool
strict_any_of(_tIn first, _tIn last, _fPred pred)
{
	return std::accumulate(first, last, false,
		[pred](bool b, decltype(*first) val){
		return b || bool(pred(yforward(val)));
	});
}

//! \note 若返回值，同 std::none_of 的返回结果。
template<typename _tIn, typename _fPred>
inline bool
strict_none_of(_tIn first, _tIn last, _fPred pred)
{
	return !ystdex::strict_any_of(first, last, pred);
}
//@}


/*!
\tparam _func 用于遍历范围的操作的可调用类型。
\param first 输入范围起始迭代器。
\param last 输入范围终止迭代器。
\param f 遍历操作。
\pre _func 满足 MoveConstructible 要求。
\pre _type 满足 EqualityComparable 要求。
\return 转移的 f 。
\note 时间复杂度： 不大于 <tt>last - first</tt> 次 \c f 的调用。
\see WG21 N3936 25.2.4[alg.foreach] 。
\see 其它参考实现： http://stackoverflow.com/questions/234482/using-stl-to-find-all-elements-in-a-vector 。

分别应用操作至范围 <tt>[first, last)</tt> 内的解应用的迭代器 \c i 满足以下条件：
\li <tt>*i == value</tt>
\li <tt>pred(*i) != false</tt>
*/
//@{
template<typename _tIn, typename _func, typename _type>
_func
for_each_equal(_tIn first, _tIn last, const _type& value, _func f)
{
	for(; first != last; ++first)
	{
		first = std::find(first, last, value);
		f(*first);
	}
	return f;
}

template<typename _tIn, typename _func, typename _fPred>
_func
for_each_if(_tIn first, _tIn last, _fPred pred, _func f)
{
	for(; first != last; ++first)
	{
		first = std::find_if(first, last, pred);
		f(*first);
	}
	return f;
}
//@}


//! \since build 627
inline namespace cpp2014
{

using std::equal;
#if !(__cpp_lib_robust_nonmodifying_seq_ops >= 201304L)
template<typename _tIn1, typename _tIn2, typename _fBiPred>
inline bool
equal(_tIn1 first1, _tIn1 last1, _tIn2 first2, _tIn2 last2,
	_fBiPred binary_pred)
{
	if(have_same_iterator_category<std::random_access_iterator_tag, _tIn1,
		_tIn2>())
	{
		if(std::distance(first1, last1) != std::distance(first2, last2))
			return {};
		return std::equal(first1, last1, first2, binary_pred);
	}
	for(; first1 != last1 && first2 != last2; ++first1, ++first2)
		if(!bool(binary_pred(*first1, *first2)))
			return {};
	return first1 == last1 && first2 == last2;
}
template<typename _tIn1, typename _tIn2>
inline bool
equal(_tIn1 first1, _tIn1 last1, _tIn2 first2, _tIn2 last2)
{
	return ystdex::equal(first1, last1, first2, last2, is_equal());
}
#endif

} // inline namespace cpp2014;


/*!
\note 起始范围距离可较迭代器类别指定的复杂度更优化（如指定 list 的整个范围时）。
\since build 865
*/
//@{
/*
\brief 取有序范围内的迭代器下界。
\note 同 std::lower_bound 但使用起始迭代器和距离指定范围。
*/
template<typename _tFwd, typename _type, typename _fComp = less<_type>>
YB_ATTR_nodiscard YB_PURE _tFwd
lower_bound_n(_tFwd first, typename std::iterator_traits<
	_tFwd>::difference_type n, const _type& value, _fComp comp = less<_type>())
{
	while(n > 0)
	{
		const auto half(n >> 1);
		auto middle(first);

		YB_VerifyIterator(middle);
		std::advance(middle, half);
		YB_VerifyIterator(middle);
		if(comp(*middle, value))
		{
			first = middle;
			++first;
			n = n - half - 1;
		}
		else
			n = half;
	}
	return first;
}


/*!
\brief 取有序范围内的迭代器上界。
\note 同 std::upper_bound 但使用起始迭代器和距离指定范围。
*/
template<typename _tFwd, typename _type, typename _fComp = less<_type>>
YB_ATTR_nodiscard YB_PURE _tFwd
upper_bound_n(_tFwd first, typename std::iterator_traits<
	_tFwd>::difference_type n, const _type& value, _fComp comp = less<_type>())
{
	return ystdex::lower_bound_n(first, n, value, [&](const _type& x,
		const typename std::iterator_traits<_tFwd>::value_type& y){
		return !comp(y, x);
	});
}
//@}


//! \note 只保留非空结果，不保留分隔符。
//@{
/*!
\brief 以指定分隔符分割序列。
\since build 304
*/
template<typename _fPred, typename _fInsert, typename _tFwd>
void
split(_tFwd first, _tFwd last, _fPred is_delim, _fInsert insert)
{
	while(first != last)
	{
		_tFwd i(std::find_if_not(first, last, is_delim));

		first = std::find_if(i, last, is_delim);
		if(i != first)
			insert(i, first);
		else
			break;
	}
}

/*!
\brief 以满足迭代器谓词的指定分隔符分割序列。
\since build 668
*/
template<typename _fPred, typename _fInsert, typename _func, typename _tFwd>
void
split_if(_tFwd first, _tFwd last, _fPred is_delim, _fInsert insert, _func pred)
{
	while(first != last)
	{
		_tFwd i(first);

		while(i != last && is_delim(*i) && pred(i))
			++i;
		for(first = i; first != last; ++first)
		{
			first = std::find_if(first, last, is_delim);
			if(first == last || pred(first))
				break;
		}
		if(i != first)
			insert(i, first);
		else
			break;
	}
}

/*!
\brief 以指定分隔符分割保留起始分隔符的序列。
\note 除非无法匹配，保留起始分隔符。
\since build 408
*/
template<typename _fPred, typename _fInsert, typename _tFwd>
_tFwd
split_l(_tFwd first, _tFwd last, _fPred is_delim, _fInsert insert)
{
	_tFwd i(first);

	while(first != last)
	{
		if(is_delim(*first) && i != first)
		{
			insert(i, first);
			i = first;
		}
		++first;
	}
	if(i != first)
		insert(i, first);
	return i;
}
//@}
//@}


/*!	\defgroup mutating_algorithms Mutating Sequence Operations
\brief 可变序列操作。
\see WG21 N3936 25.3[alg.modifying.operations] 。
\since build 531
*/
//@{
/*!
\tparam _tIn 输入迭代器类型。
\tparam _tOut 输出迭代器类型。
\tparam _fPred 谓词类型。
\param first 被复制序列的起始输入迭代器。
\param result 输出迭代器。
\param pred 一元谓词。
\pre 迭代器可解引用。
\since build 594
*/
//@{
/*!
\brief 变换满足谓词的序列的连续元素。
\tparam _fOp 变换操作类型。
\param op 一元变换操作。
*/
template<typename _tIn, typename _tOut, typename _fPred, typename _fOp>
_tOut
transform_when(_tIn first, _tOut result, _fPred pred, _fOp op)
{
	yunseq((YB_VerifyIterator(first), 0), (YB_VerifyIterator(result), 0));

	for(; pred(*first); yunseq((++first, 0), (++result, 0)))
		*result = op(*first);
	return result;
}

//! \brief 复制满足谓词的序列的连续元素。
template<typename _tIn, typename _tOut, typename _fPred>
_tOut
copy_when(_tIn first, _tOut result, _fPred pred)
{
	return ystdex::transform_when(first, result, pred,
		[](decltype(*first)& v) ynothrow{
		return static_cast<decltype(*first)&>(v);
	});
}
//@}

/*!
\brief 指定数量的序列变换。
\tparam _fOp 序列变换操作类型。
\tparam _tOut 表示结果的输出迭代器类型。
\tparam _tIns 输入迭代器类型。
\pre 迭代器可解引用。
\warning 不检查越界。
\since build 517
*/
template<typename _fOp, typename _tOut, typename... _tIns>
void
transform_n(_fOp op, _tOut result, size_t n, _tIns... iters)
{
	while(n-- != 0)
	{
		yunseq((YB_VerifyIterator(result), 0),
			(YB_VerifyIterator(iters), void(iters), 0)...);
		*result = op((*iters)...);
		yunseq(++result, ++iters...);
	}
}

/*!	\defgroup trivial_type_operations Trivial Type Operations
\brief 平凡类型操作。
\tparam _type 指定对象类型。
\pre 静态断言： <tt>is_trivial<_type>()</tt> 。
\pre 范围中的指针有效。
\pre 断言：范围起始指针满足 YB_VerifyIterator 。
\since build 603
\todo 使用 \c is_trivially_copyable 代替 \c is_trivial 。
*/
//@{
/*!
\brief 填充字节序列。
\pre 指针输入范围要求同 std::memset 。
*/
//@{
template<typename _type>
inline _type*
trivially_fill_n(_type* first, size_t n = 1, byte value = {}) ynothrowv
{
	static_assert(is_trivial<_type>(), "Non-trivial type found.");

	YB_VerifyIterator(first);
	std::memset(first, int(value), sizeof(_type) * n);
	return first + n;
}

template<typename _type>
inline void
trivially_fill(_type* first, _type* last, byte value = {}) ynothrowv
{
	ystdex::trivially_fill_n(first, last - first, value);
}
//@}

/*!
\brief 复制不覆盖的序列。
\pre 静态断言： <tt>is_copy_assignment<_type>()</tt> 。
\pre 指针输入范围要求同 std::memcpy 。
*/
//@{
template<typename _type>
inline _type*
trivially_copy_n(const _type* first, size_t n, _type* result) ynothrowv
{
	static_assert(is_trivial<_type>(), "Non-trivial type found.");
	static_assert(is_copy_assignable<_type>(),
		"Type shall meet CopyAssignable.");

	yunseq((YB_VerifyIterator(result), 0),
		(YB_VerifyIterator(first), 0));
	std::memcpy(result, first, sizeof(_type) * n);
	return result + n;
}

template<typename _type>
inline _type*
trivially_copy(const _type* first, const _type* last, _type* result) ynothrowv
{
	return ystdex::trivially_copy_n(first, last - first, result);
}
//@}

/*!
\brief 复制可能覆盖的序列。
\pre 静态断言： <tt>is_copy_assignment<_type>()</tt> 。
\pre 指针输入范围要求同 std::memmove 。
*/
//@{
template<class _type>
inline _type*
trivially_move_n(const _type* first, size_t n, _type* result)
{
	static_assert(is_trivial<_type>(), "Non-trivial type found.");
	static_assert(is_copy_assignable<_type>(),
		"Type shall meet CopyAssignable.");

	yunseq((YB_VerifyIterator(result), 0),
		(YB_VerifyIterator(first), 0));
	std::memmove(result, first, sizeof(_type) * n);
	return result + n;
}

template<class _type>
inline _type*
trivially_move(const _type* first, const _type* last, _type* result)
{
	return ystdex::trivially_move_n(first, last - first, result);
}
//@}
//@}


/*!
\brief 去除迭代器指定的范围中的重复元素，且不改变元素之间的相对顺序。
\param first 输入范围起始迭代器。
\param last 输入范围终止迭代器。
\return 输出范围终止迭代器。
\note 输入和输出范围的起始迭代器相同。
\note 输出范围元素之间的相对顺序和输入的范围保持一致。
\note 时间复杂度： O(n^2) ，其中 n 满足 <tt>std::advance(first, n) == last</tt> 。
\note 使用 ADL 交换。
\see WG21 N3936 25.3.9[alg.unique] 。
\since build 531
*/
//@{
template<typename _tFwd>
_tFwd
stable_unique(_tFwd first, _tFwd last)
{
	_tFwd result(first);

	for(_tFwd i(first); i != last; ++i)
		if(std::find(first, result, *i) == result)
		{
			std::iter_swap(i, result);
			++result;
		}
	return result;
}
//! \param pred 比较等价的二元谓词。
template<typename _tFwd, typename _fPred>
_tFwd
stable_unique(_tFwd first, _tFwd last, _fPred pred)
{
	_tFwd result(first);

	for(_tFwd i(first); i != last; ++i)
		if(std::find_if(first, result,
			std::bind(pred, std::ref(*i), std::placeholders::_1)) == result)
		{
			std::iter_swap(i, result);
			++result;
		}
	return result;
}
//@}
//@}


/*!	\defgroup sorting_and_related_algorithms Sorting and Related Operations
\brief 排序相关操作。
\see WG21 N3936 25.4[alg.sorting] 。
\since build 531
*/
//@{
/*!
\brief 排序指定序列范围，保留不重复元素的区间。
\tparam _tRandom 随机迭代器类型。
\param first 输入范围起始迭代器。
\param last 输入范围终止迭代器。
\note 时间复杂度：令 N = last - first ， O(N log(N)) + N - 1 次比较。
\return 不重复元素的区间末尾。
\since build 414
*/
template<typename _tRandom>
inline _tRandom
sort_unique(_tRandom first, _tRandom last)
{
	std::sort(first, last);
	return std::unique(first, last);
}


/*!
\see LWG 2350 。
\since build 836
*/
inline namespace cpp2014
{

// XXX: No feature testing macro available yet.
#if __cplupslus >= 201402L
using std::min;
using std::max;
#else
/*!
\ingroup YBase_replacement_features
\since build 578
*/
//@{
/*!
\brief 取较小的元素。
\note 语义同 ISO C++14 std::min 的带 constexpr 的重载。
*/
//@{
template<typename _type, typename _fComp = less<_type>>
YB_ATTR_nodiscard YB_PURE yconstfn const _type&
min(const _type& a, const _type& b, _fComp comp = less<_type>())
{
	return comp(b, a) ? b : a;
}
template<typename _type, typename _fComp = less<_type>>
YB_ATTR_nodiscard YB_PURE yconstfn const _type&
min(std::initializer_list<_type> t, _fComp comp = less<_type>(),
	yimpl(size_t n = 0))
{
	return n + 1 < t.size() ? ystdex::min(*(t.begin() + n),
		ystdex::min(t, comp, n + 1)) : *(t.begin() + n);
}
//@}

/*!
\brief 取较大的元素。
\note 语义同 ISO C++14 std::max 的带 constexpr 的重载。
*/
//@{
template<typename _type, typename _fComp = less<_type>>
YB_ATTR_nodiscard YB_PURE yconstfn const _type&
max(const _type& a, const _type& b, _fComp comp = less<_type>())
{
	return comp(a, b) ? b : a;
}
template<typename _type, typename _fComp = less<_type>>
YB_ATTR_nodiscard YB_PURE yconstfn const _type&
max(std::initializer_list<_type> t, _fComp comp = less<_type>(),
	yimpl(size_t n = 0))
{
	return n + 1 < t.size() ? ystdex::max(*(t.begin() + n),
		ystdex::max(t, comp, n + 1)) : *(t.begin() + n);
}
//@}
//@}
#endif

} // inline namespace cpp2014;

/*!
\brief 取约束范围的值。
\see WG21 N4536 。
\since build 612
*/
//@{
/*!
\see ISO C++17 [alg.clamp] 。
\see WG21 P0025R1 。
*/
//@{
inline namespace cpp2017
{

#if __cpp_lib_clamp >= 201603L
using std::clamp;
#else
//! \ingroup YBase_replacement_features
//@{
template<typename _type, typename _fComp>
YB_ATTR_nodiscard YB_PURE yconstfn const _type&
clamp(const _type& v, const _type& lo, const _type& hi, _fComp comp)
{
	return yconstraint(!comp(hi, lo)), comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}
template<typename _type>
YB_ATTR_nodiscard YB_PURE yconstfn const _type&
clamp(const _type& v, const _type& lo, const _type& hi)
{
    return ystdex::clamp(v, lo, hi, less<_type>());
}
//@}
#endif

} // inline namespace cpp2017;
//@}

//! \ingroup YBase_replacement_extensions
template<typename _tIn, typename _tOut, typename _fComp = less<>>
yconstfn _tOut
clamp_range(_tIn first, _tIn last, _tOut result, const typename
	std::iterator_traits<_tIn>::value_type& lo, const typename
	std::iterator_traits<_tIn>::value_type& hi, _fComp comp = _fComp())
{
	using arg_type = decltype(lo);

	return std::transform(first, last, result, [&](arg_type val) -> arg_type{
		return ystdex::clamp(val, lo, hi, comp);
	});
}
//@}
//@}
//@}

} // namespace ystdex;

#endif

