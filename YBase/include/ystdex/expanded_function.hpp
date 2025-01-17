﻿/*
	© 2012-2013, 2015-2017, 2019-2022 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file expanded_function.hpp
\ingroup YStandardEx
\brief 展开调用的函数对象。
\version r4985
\author FrankHB <frankhb1989@gmail.com>
\since build 939
\par 创建时间:
	2022-02-14 06:11:55 +0800
\par 修改时间:
	2022-02-15 18:04 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YStandardEx::ExpandedFunction
*/


#ifndef YB_INC_ystdex_expanded_function_hpp_
#define YB_INC_ystdex_expanded_function_hpp_ 1

#include "function.hpp" // for internal "function.hpp", paramlist_size,
//	make_index_sequence, call_projection, index_sequence, exclude_self_t,
//	std::move, is_constructible, as_function_type_t, equality_comparable,
//	enable_if_t, is_nothrow_copy_constructible, is_bitwise_swappable,
//	make_parameter_list, return_of, is_covariant, is_contravariant,
//	make_function_type, invoke_result_t, object_result_t, invoke_nonvoid;
#include "swap.hpp" // for std::allocator_arg_t, std::allocator_arg,
//	trivial_swap_t, trivial_swap, ystdex::swap_dependent;

namespace ystdex
{

//! \since build 634
//@{
template<typename _fCallable, size_t _vLen = paramlist_size<_fCallable>::value>
struct expand_proxy : private call_projection<_fCallable,
	make_index_sequence<_vLen>>, private expand_proxy<_fCallable, _vLen - 1>
{
	/*!
	\note 为避免歧义，不直接使用 using 声明。
	\see CWG 1393 。
	\see EWG 102 。
	*/
	//@{
	//! \since build 810
	//@{
	using call_projection<_fCallable, make_index_sequence<_vLen>>::apply_call;
	template<typename... _tParams>
	static auto
	apply_call(_tParams&&... args) -> decltype(
		expand_proxy<_fCallable, _vLen - 1>::apply_call(yforward(args)...))
	{
		return expand_proxy<_fCallable, _vLen - 1>::apply_call(
			yforward(args)...);
	}

	using call_projection<_fCallable, make_index_sequence<_vLen>>::apply_invoke;
	template<typename... _tParams>
	static auto
	apply_invoke(_tParams&&... args) -> decltype(
		expand_proxy<_fCallable, _vLen - 1>::apply_invoke(yforward(args)...))
	{
		return expand_proxy<_fCallable, _vLen - 1>::apply_invoke(
			yforward(args)...);
	}
	//@}

	using call_projection<_fCallable, make_index_sequence<_vLen>>::call;
	//! \since build 657
	template<typename... _tParams>
	static auto
	call(_tParams&&... args) -> decltype(
		expand_proxy<_fCallable, _vLen - 1>::call(yforward(args)...))
	{
		return expand_proxy<_fCallable, _vLen - 1>::call(yforward(args)...);
	}

	//! \since build 810
	//@{
	using call_projection<_fCallable, make_index_sequence<_vLen>>::invoke;
	template<typename... _tParams>
	static auto
	invoke(_tParams&&... args) -> decltype(
		expand_proxy<_fCallable, _vLen - 1>::invoke(yforward(args)...))
	{
		return expand_proxy<_fCallable, _vLen - 1>::invoke(yforward(args)...);
	}
	//@}
	//@}
};

template<typename _fCallable>
struct expand_proxy<_fCallable, 0>
	: private call_projection<_fCallable, index_sequence<>>
{
	//! \since build 865
	using call_projection<_fCallable, index_sequence<>>::apply_call;

	//! \since build 865
	using call_projection<_fCallable, index_sequence<>>::apply_invoke;

	using call_projection<_fCallable, index_sequence<>>::call;

	//! \since build 865
	using call_projection<_fCallable, index_sequence<>>::invoke;
};
//@}


/*!
\ingroup functors
\brief 接受冗余参数的可调用对象。
\since build 447
\todo 支持 const 以外的限定符和 ref-qualifier 。
*/
template<typename _fHandler, typename _fCaller>
struct expanded_caller
{
	//! \since build 448
	static_assert(is_object<_fCaller>(), "Callable object type is needed.");

	//! \since build 525
	_fCaller caller;

	//! \since build 849
	//@{
	expanded_caller() = default;
	template<typename _func,
		yimpl(typename = exclude_self_t<expanded_caller, _fCaller>)>
	expanded_caller(_func f)
		: caller(yforward(f))
	{}
	expanded_caller(const expanded_caller&) = default;
	expanded_caller(expanded_caller&&) = default;

	expanded_caller&
	operator=(const expanded_caller&) = default;
	expanded_caller&
	operator=(expanded_caller&&) = default;
	//@}

	//! \since build 640
	template<typename... _tParams>
	inline auto
	operator()(_tParams&&... args) const
		-> decltype(expand_proxy<_fHandler>::call(caller, yforward(args)...))
	{
		return expand_proxy<_fHandler>::call(caller, yforward(args)...);
	}
};

/*!
\ingroup helper_functions
\brief 构造接受冗余参数的可调用对象。
\relates expanded_caller
\since build 849
*/
template<typename _fHandler, typename _fCallable>
YB_ATTR_nodiscard YB_PURE inline expanded_caller<_fHandler, _fCallable>
make_expanded(_fCallable f)
{
	return expanded_caller<_fHandler, _fCallable>(std::move(f));
}


/*!
\ingroup binary_type_traits
\brief 判断可调用类型是可展开到指定函数对象的类型。
\since build 850
*/
template<typename _func, typename _fCallable>
using is_expandable = is_constructible<_func,
	expanded_caller<as_function_type_t<_func>, _fCallable>>;


//! \since build 878
//@{
/*!
\ingroup functors
\brief 扩展函数包装类模板。
\pre 第二模板参数指定和 function_base 的对应签名的实例的操作兼容的类类型。

接受冗余参数的可调用对象的函数包装类模板。
函数模板签名和对应的基类。
基类和 function_base 实例的操作兼容，当且仅当实例化的操作具有相同的语义，
	包括使用特定类型的参数构造对象时和 std::function 实例类似的不抛出异常的要求。
*/
template<typename _fSig, class = function<_fSig>>
class expanded_function;

/*!
\brief 扩展函数包装类模板特化。
\sa function
\sa function_base
*/
template<typename _tRet, typename... _tParams, class _tBase>
class expanded_function<_tRet(_tParams...), _tBase>
	: protected _tBase, private equality_comparable<expanded_function<
	_tRet(_tParams...), _tBase>>, private equality_comparable<
	expanded_function<_tRet(_tParams...), _tBase>, nullptr_t>
{
public:
	using base_type = _tBase;
	using result_type = typename base_type::result_type;

private:
	template<typename _fCallable>
	using enable_if_expandable_t = enable_if_t<
		!is_constructible<base_type, _fCallable>::value
		&& is_expandable<base_type, _fCallable>::value>;

public:
	//! \note 对函数指针为参数的初始化允许区分重载函数并提供异常规范。
	//@{
	expanded_function() ynothrow
		: base_type()
	{}
	//! \brief 构造：使用函数指针。
	expanded_function(_tRet(*f)(_tParams...)) ynothrow
		: base_type(f)
	{}
	//! \brief 构造：使用分配器。
	template<class _tAlloc>
	inline
	expanded_function(std::allocator_arg_t, const _tAlloc& a) ynothrow
		: base_type(std::allocator_arg, a)
	{}
	//! \brief 构造：使用分配器和函数指针。
	template<class _tAlloc>
	inline
	expanded_function(std::allocator_arg_t, const _tAlloc& a,
		_tRet(*f)(_tParams...)) ynothrow
		: base_type(std::allocator_arg, a, f)
	{}
	//@}
	// XXX: Reference of '_Callable&&' is kept for performance here.
	/*!
	\brief 使用函数对象。
	\since build 926
	*/
	//@{
	template<class _fCallable, yimpl(typename = exclude_self_t<
		expanded_function, _fCallable>, typename = enable_if_t<is_constructible<
		base_type, _fCallable>::value>)>
	inline
	expanded_function(_fCallable&& f)
		: base_type(yforward(f))
	{}
	template<class _fCallable, yimpl(typename = exclude_self_t<
		expanded_function, _fCallable>, typename = enable_if_t<is_constructible<
		base_type, _fCallable>::value>)>
	inline
	expanded_function(trivial_swap_t, _fCallable&& f)
		: base_type(trivial_swap, yforward(f))
	{}
	//@}
	//! \brief 使用分配器和函数对象。
	//@{
	template<class _fCallable, class _tAlloc,
		yimpl(typename = exclude_self_t<expanded_function, _fCallable>,
		typename = enable_if_t<
		std::is_constructible<base_type, _fCallable>::value>)>
	inline
	expanded_function(std::allocator_arg_t, const _tAlloc& a, _fCallable&& f)
		: base_type(std::allocator_arg, a, yforward(f))
	{}
	//! \since build 926
	template<class _fCallable, class _tAlloc,
		yimpl(typename = exclude_self_t<expanded_function, _fCallable>,
		typename = enable_if_t<
		std::is_constructible<base_type, _fCallable>::value>)>
	inline
	expanded_function(std::allocator_arg_t, const _tAlloc& a, trivial_swap_t,
		_fCallable&& f)
		: base_type(std::allocator_arg, a, trivial_swap, yforward(f))
	{}
	//@}
	//! \brief 使用扩展函数对象。
	//@{
	template<typename _fCallable,
		yimpl(typename = enable_if_expandable_t<_fCallable>)>
	expanded_function(_fCallable f)
		: base_type(ystdex::make_expanded<_tRet(_tParams...)>(std::move(f)))
	{}
	//! \since build 926
	template<typename _fCallable,
		yimpl(typename = enable_if_expandable_t<_fCallable>)>
	expanded_function(trivial_swap_t, _fCallable f)
		: base_type(trivial_swap,
		ystdex::make_expanded<_tRet(_tParams...)>(std::move(f)))
	{}
	//@}
	//! \brief 使用分配器和扩展函数对象。
	//@{
	template<typename _fCallable, class _tAlloc,
		yimpl(typename = enable_if_expandable_t<_fCallable>)>
	expanded_function(std::allocator_arg_t, const _tAlloc& a, _fCallable f)
		: base_type(std::allocator_arg, a,
		ystdex::make_expanded<_tRet(_tParams...)>(std::move(f)))
	{}
	//! \since build 926
	template<typename _fCallable, class _tAlloc,
		yimpl(typename = enable_if_expandable_t<_fCallable>)>
	expanded_function(std::allocator_arg_t, const _tAlloc& a, trivial_swap_t,
		_fCallable f)
		: base_type(std::allocator_arg, a, trivial_swap,
		ystdex::make_expanded<_tRet(_tParams...)>(std::move(f)))
	{}
	//@}
	/*!
	\brief 构造：使用对象引用和成员函数指针。
	\warning 使用空成员指针构造的函数对象调用引起未定义行为。
	\todo 支持相等构造。
	*/
	//@{
	template<class _type>
	yconstfn
	expanded_function(_type& obj, _tRet(_type::*pm)(_tParams...))
		: expanded_function([&, pm](_tParams... args) ynoexcept(
			noexcept((obj.*pm)(yforward(args)...))
			&& is_nothrow_copy_constructible<_tRet>::value){
			return (obj.*pm)(yforward(args)...);
		})
	{}
	template<class _type, class _tAlloc>
	yconstfn
	expanded_function(std::allocator_arg_t, const _tAlloc& a, _type& obj,
		_tRet(_type::*pm)(_tParams...))
		: expanded_function(std::allocator_arg, a,
			[&, pm](_tParams... args) ynoexcept(
			noexcept((obj.*pm)(yforward(args)...))
			&& is_nothrow_copy_constructible<_tRet>::value){
			return (obj.*pm)(yforward(args)...);
		})
	{}
	//@}
	//! \exception allocator_mismatch_error 分配器不兼容。
	//@{
	template<class _tAlloc>
	expanded_function(std::allocator_arg_t, const _tAlloc& a,
		const expanded_function& x)
		: base_type(std::allocator_arg, a, x)
	{}
	template<class _tAlloc>
	expanded_function(std::allocator_arg_t, const _tAlloc& a,
		expanded_function&& x)
		: base_type(std::allocator_arg, a, std::move(x))
	{}
	//@}
	expanded_function(const expanded_function&) = default;
	expanded_function(expanded_function&&) = default;

	expanded_function&
	operator=(nullptr_t) ynothrow
	{
		base_type::operator=(nullptr);
		return *this;
	}
	template<typename _fCallable, yimpl(typename
		= exclude_self_t<expanded_function, _fCallable>), yimpl(typename
		= enable_if_t<is_invocable_r<_tRet, _fCallable&, _tParams...>::value>)>
	expanded_function&
	operator=(_fCallable&& f)
	{
		return *this = expanded_function(std::move(f));
	}
	template<typename _fCallable>
	expanded_function&
	operator=(std::reference_wrapper<_fCallable> f) ynothrow
	{
		return *this = expanded_function(f);
	}
	expanded_function&
	operator=(const expanded_function&) = default;
	expanded_function&
	operator=(expanded_function&&) = default;

	using base_type::operator();

	using base_type::operator bool;

	friend void
	swap(expanded_function& x, expanded_function& y) ynothrow
	{
		ystdex::swap_dependent(static_cast<base_type&>(x),
			static_cast<base_type&>(y));
	}

	using base_type::target;

	using base_type::target_type;
};

//! \relates expanded_function
//@{
//! \since build 926
template<typename _tRet, typename... _tParams, class _tBase>
struct is_bitwise_swappable<expanded_function<_tRet(_tParams...), _tBase>>
	: is_bitwise_swappable<_tBase>
{};

template<typename _fSig, class _tBase>
struct make_parameter_list<expanded_function<_fSig, _tBase>>
	: make_parameter_list<_fSig>
{};

template<typename _fSig, class _tBase>
struct return_of<expanded_function<_fSig, _tBase>> : return_of<_fSig>
{};

template<typename _tFrom, typename _tTo,
	typename... _tFromParams, typename... _tToParams, class _tBase>
struct is_covariant<expanded_function<_tFrom(_tFromParams...), _tBase>,
	expanded_function<_tTo(_tToParams...), _tBase>>
	: is_covariant<_tFrom(_tFromParams...), _tTo(_tToParams...)>
{};

template<typename _tResFrom, typename _tResTo,
	typename... _tFromParams, typename... _tToParams, class _tBase>
struct is_contravariant<expanded_function<_tResFrom(_tFromParams...), _tBase>,
	expanded_function<_tResTo(_tToParams...), _tBase>>
	: is_contravariant<_tResFrom(_tFromParams...), _tResTo(_tToParams...)>
{};

template<typename _tRet, typename _tUnused, typename... _tParams, class _tBase>
struct make_function_type<_tRet, expanded_function<_tUnused(_tParams...),
	_tBase>>
	: make_function_type<_tRet, empty_base<_tParams...>>
{};

template<typename _tRet, typename... _tParams, class _tBase, size_t... _vSeq>
struct call_projection<expanded_function<_tRet(_tParams...), _tBase>,
	index_sequence<_vSeq...>>
	: call_projection<_tRet(_tParams...), index_sequence<_vSeq...>>
{};
//@}
//@}


/*!
\brief 循环重复调用：代替直接使用 do-while 语句以避免过多引入作用域外的变量。
\tparam _fCond 判断条件。
\tparam _fCallable 可调用对象类型。
\tparam _tParams 参数类型。
\note 条件接受调用结果或没有参数。
\since build 832
\sa object_result_t
*/
template<typename _fCond, typename _fCallable, typename... _tParams>
invoke_result_t<_fCallable, _tParams...>
retry_on_cond(_fCond cond, _fCallable&& f, _tParams&&... args)
{
	using res_t = invoke_result_t<_fCallable, _tParams...>;
	using obj_t = object_result_t<res_t>;
	obj_t res;

	do
		res = ystdex::invoke_nonvoid(yforward(f), yforward(args)...);
	while(expand_proxy<bool(obj_t&)>::call(cond, res));
	return res_t(res);
}

} // namespace ystdex;

#endif

