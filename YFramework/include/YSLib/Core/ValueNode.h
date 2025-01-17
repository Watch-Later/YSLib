﻿/*
	© 2012-2022 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file ValueNode.h
\ingroup Core
\brief 值类型节点。
\version r4291
\author FrankHB <frankhb1989@gmail.com>
\since build 338
\par 创建时间:
	2012-08-03 23:03:44 +0800
\par 修改时间:
	2022-09-09 00:08 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::Core::ValueNode
*/


#ifndef YSL_INC_Core_ValueNode_h_
#define YSL_INC_Core_ValueNode_h_ 1

#include "YModules.h"
#include YFM_YSLib_Core_YObject // for pmr, ystdex::is_interoperable,
//	ystdex::enable_if_t, std::allocator_arg_t, std::allocator_arg, ystdex::and_,
//	ystdex::remove_cvref_t, in_place_type, ystdex::end, to_string,
//	to_pmr_string;
#include <ystdex/operators.hpp> // for ystdex::totally_ordered;
#include <ystdex/set.hpp> // for ystdex::mapped_set;
#include <ystdex/tuple.hpp> // for ystdex::invoke, ystdex::make_from_tuple;
#include <ystdex/swap.hpp> // for ystdex::copy_and_swap;
#include <ystdex/path.hpp> // for ystdex::path;
#include <ystdex/utility.hpp> // for ystdex::forward_like;
#include <ystdex/algorithm.hpp> // for std::for_each_if;
#include <numeric> // for std::accumulate;
#include <ystdex/deref_op.hpp> // for ystdex::call_value_or;
#include <ystdex/functor.hpp> // for ystdex::addrof;

namespace YSLib
{

//! \since build 674
//@{
//! \brief 标记列表构造容器。
yconstexpr const struct ListContainerTag{} ListContainer{};

//! \brief 标记不使用容器。
yconstexpr const struct NoContainerTag{} NoContainer{};
//@}


/*!
\brief 值类型节点。
\warning 非虚析构。
\warning 作为子节点时应保证修改操作不影响键（名称）的等价性，否则访问容器行为未定义。
\warning 若操作引入循环引用，可能造成资源泄漏，且不保证容器行为满足 C++ 容器要求。
\since build 330

包含名称字符串和值类型对象的对象节点。
使用字符串作为外部访问的键，但容器类型直接保存的键是自身。
内部包含一个 Container 类型的关联容器，保持被删除以外的其它元素不失效。
因为 Container 对实现无其它要求，构造开销不保证可忽略，不使用合一构造。
*/
class YF_API ValueNode : private ystdex::totally_ordered<ValueNode>,
	private ystdex::totally_ordered<ValueNode, string>
{
public:
	//! \since build 844
	//@{
	/*!
	\brief 分配器类型。
	\note 支持 uses-allocator 构造并适合直接传递对象的值。
	*/
	using allocator_type = default_allocator<ValueNode>;
	template<typename _type>
	using is_key_t = ystdex::is_interoperable<const _type&, const string&>;
	template<typename _tKey>
	using enable_if_key_t = ystdex::enable_if_t<is_key_t<_tKey>::value>;
	template<typename _tOther>
	using enable_not_key_t = ystdex::enable_if_t<!is_key_t<_tOther>::value>;
	//@}

private:
	//! \since build 844
	template<typename... _tParams>
	using enable_value_constructible_t = ystdex::enable_if_t<
		std::is_constructible<ValueObject, _tParams...>::value>;

public:
	/*!
	\sa ystdex::mapped_set
	\since build 852
	*/
	struct YF_API MappedSetTraits
	{
		//! \since build 844
		//@{
		template<typename _tKey, class _tCon>
		YB_ATTR_nodiscard YB_PURE static inline auto
		extend_key(_tKey&& k, _tCon& con) ynothrow -> decltype(
			ValueNode(std::allocator_arg, con.get_allocator(), NoContainer,
			yforward(k)))
		{
			return {std::allocator_arg, con.get_allocator(), NoContainer,
				yforward(k)};
		}

		//! \note 这些重载和构造函数中可能由参数确定键的值的情形匹配。
		//@{
		YB_ATTR_nodiscard YB_PURE static inline
			PDefH(string, get_value_key, allocator_type = {}) ynothrow
			ImplRet({})
		YB_ATTR_nodiscard YB_PURE static PDefH(const string&, get_value_key,
			const ValueNode& nd, allocator_type = {}) ynothrow
			ImplRet(nd.name)
		YB_ATTR_nodiscard YB_PURE static PDefH(string&&, get_value_key,
			ValueNode&& nd, allocator_type = {}) ynothrow
			ImplRet(std::move(nd.name))
		//! \since build 845
		//@{
		template<typename _tParam, typename _tOther, typename... _tParams,
			yimpl(typename = enable_not_key_t<_tOther>)>
		static PDefH(string, get_value_key, _tOther&&, allocator_type = {})
			ynothrow
			ImplRet({})
		template<typename _tString, typename _tOther, typename... _tParams,
			yimpl(typename = enable_value_constructible_t<_tParams...>),
			yimpl(typename = enable_not_key_t<_tOther>)>
		YB_ATTR_nodiscard YB_PURE static inline _tString&&
		get_value_key(_tOther&&, _tString&& str, _tParams&&...)
		{
			return yforward(str);
		}
		template<typename _tString, typename _tOther, typename... _tParams,
			yimpl(typename = enable_value_constructible_t<_tParams...>),
			yimpl(typename = enable_not_key_t<_tOther>)>
		YB_ATTR_nodiscard YB_PURE static inline _tString&&
		get_value_key(std::allocator_arg_t, allocator_type, _tOther&&,
			_tString&& str, _tParams&&...)
		{
			return yforward(str);
		}
		template<typename _tIn>
		YB_ATTR_nodiscard YB_PURE static inline string
		get_value_key(const pair<_tIn, _tIn>&, allocator_type = {})
		{
			return {};
		}
		template<typename _tIn, typename _tString>
		YB_ATTR_nodiscard YB_PURE static inline _tString&&
		get_value_key(const pair<_tIn, _tIn>&, _tString&& str,
			allocator_type = {})
		{
			return yforward(str);
		}
		template<typename... _tParams1>
		YB_ATTR_nodiscard YB_PURE static inline string
		get_value_key(tuple<_tParams1...>, allocator_type = {})
		{
			return {};
		}
		template<typename... _tParams1, typename... _tParams2>
		YB_ATTR_nodiscard YB_PURE static inline string
		get_value_key(tuple<_tParams1...>, tuple<_tParams2...> args2,
			allocator_type = {})
		{
			return ystdex::make_from_tuple<string>(args2);
		}
		template<typename... _tParams1, typename... _tParams2,
			typename... _tParams3>
		YB_ATTR_nodiscard YB_PURE static inline string
		get_value_key(tuple<_tParams1...>, tuple<_tParams2...> args2,
			tuple<_tParams3...>, allocator_type = {})
		{
			return ystdex::make_from_tuple<string>(args2);
		}
		//@}
		//@}

		/*!
		\sa ystdex::restore_key
		\since build 845
		*/
		static PDefH(void, restore_key, ValueNode& nd, ValueNode&& ek)
			ImplExpr(nd.name = std::move(ek.name))

		//! \sa ystdex::set_value_move
		static PDefH(ValueNode, set_value_move, ValueNode& nd)
			ImplRet({std::move(nd.GetContainerRef()),
				nd.GetName(), std::move(nd.Value)})
		//@}
	};
	using Container = ystdex::mapped_set<ValueNode, ystdex::less<>,
		MappedSetTraits, allocator_type>;
	//! \since build 678
	using key_type = Container::key_type;
	//! \since build 460
	using iterator = Container::iterator;
	//! \since build 460
	using const_iterator = Container::const_iterator;
	//! \since build 696
	using reverse_iterator = Container::reverse_iterator;
	//! \since build 696
	using const_reverse_iterator = Container::const_reverse_iterator;

private:
	string name{};
	/*!
	\brief 子节点容器。
	\since build 667
	*/
	Container container{};

public:
	//! \since build 667
	ValueObject Value{};

	//! \note 因为 Container 对实现无要求，不显式保证无异常抛出。
	DefDeCtor(ValueNode)
	//! \since build 844
	//@{
	//! \brief 构造：使用分配器。
	explicit
	ValueNode(allocator_type a)
		: container(a)
	{}
	/*!
	\brief 构造：使用容器对象引用和可选的分配器。
	\since build 845

	复制或转移第一参数指定的容器对象作为节点的内容。
	若指定了分配器参数，使用指定的分配器；否则，复制或转移容器中的分配器。
	*/
	//@{
	ValueNode(const Container& con)
		: ValueNode(con, con.get_allocator())
	{}
	ValueNode(const Container& con, allocator_type a)
		: container(con, a)
	{}
	ValueNode(Container&& con)
		: container(std::move(con))
	{}
	ValueNode(Container&& con, allocator_type a)
		: container(std::move(con), a)
	{}
	//@}
	/*!
	\brief 构造：使用字符串引用和值类型对象构造参数。
	\note 不使用容器。
	*/
	template<typename _tString, typename... _tParams,
		yimpl(typename = enable_value_constructible_t<_tParams...>)>
	inline
	ValueNode(NoContainerTag, _tString&& str, _tParams&&... args)
		: name(yforward(str)), Value(yforward(args)...)
	{}
	/*!
	\brief 构造：使用字符串引用、值类型对象构造参数和分配器。
	\note 不使用容器。
	*/
	template<typename _tString, typename... _tParams,
		yimpl(typename = enable_value_constructible_t<_tParams...>)>
	inline
	ValueNode(std::allocator_arg_t, allocator_type a, NoContainerTag,
		_tString&& str, _tParams&&... args)
		: name(yforward(str)), container(a), Value(yforward(args)...)
	{}
	//! \brief 构造：使用容器对象引用、字符串引用和值类型对象构造参数。
	//@{
	template<typename _tString, typename... _tParams,
		yimpl(typename = enable_value_constructible_t<_tParams...>)>
	ValueNode(const Container& con, _tString&& str, _tParams&&... args)
		: ValueNode(std::allocator_arg, con.get_allocator(), con, yforward(str),
		yforward(args)...)
	{}
	template<typename _tString, typename... _tParams,
		yimpl(typename = enable_value_constructible_t<_tParams...>)>
	ValueNode(Container&& con, _tString&& str, _tParams&&... args)
		: name(yforward(str)), container(std::move(con)),
		Value(yforward(args)...)
	{}
	//@}
	//! \brief 构造：使用容器对象引用、字符串引用、值类型对象构造参数和分配器。
	//@{
	template<typename _tString, typename... _tParams,
		yimpl(typename = enable_value_constructible_t<_tParams...>)>
	inline
	ValueNode(std::allocator_arg_t, allocator_type a, const Container& con,
		_tString&& str, _tParams&&... args)
		: name(yforward(str)), container(con, a), Value(yforward(args)...)
	{}
	template<typename _tString, typename... _tParams,
		yimpl(typename = enable_value_constructible_t<_tParams...>)>
	inline
	ValueNode(std::allocator_arg_t, allocator_type a, Container&& con,
		_tString&& str, _tParams&&... args)
		: name(yforward(str)), container(std::move(con), a),
		Value(yforward(args)...)
	{}
	//@}
	//! \brief 构造：使用输入迭代器对和可选的分配器。
	template<typename _tIn>
	inline
	ValueNode(const pair<_tIn, _tIn>& pr, allocator_type a = {})
		: container(pr.first, pr.second, a)
	{}
	//! \brief 构造：使用输入迭代器对、字符串引用、值参数和可选的分配器。
	template<typename _tIn, typename _tString>
	inline
	ValueNode(const pair<_tIn, _tIn>& pr, _tString&& str,
		allocator_type a = {})
		: name(yforward(str)), container(pr.first, pr.second, a)
	{}
	//! \brief 原地构造：使用容器、名称、值的参数元组和可选的分配器。
	//@{
	template<typename... _tParams1>
	inline
	ValueNode(tuple<_tParams1...> args1, allocator_type a = {})
		: container(ystdex::make_from_tuple<Container>(args1), a)
	{}
	template<typename... _tParams1, typename... _tParams2>
	inline
	ValueNode(tuple<_tParams1...> args1, tuple<_tParams2...> args2,
		allocator_type a = {})
		: name(ystdex::make_from_tuple<string>(args2)),
		container(ystdex::make_from_tuple<Container>(args1), a)
	{}
	template<typename... _tParams1, typename... _tParams2,
		typename... _tParams3>
	inline
	ValueNode(tuple<_tParams1...> args1, tuple<_tParams2...> args2,
		tuple<_tParams3...> args3, allocator_type a = {})
		: name(ystdex::make_from_tuple<string>(args2)),
		container(ystdex::make_from_tuple<Container>(args1), a),
		Value(ystdex::make_from_tuple<ValueObject>(args3))
	{}
	//@}
	/*!
	\brief 复制构造：使用参数和参数的分配器。
	\since build 879
	*/
	ValueNode(const ValueNode& nd)
		: ValueNode(nd, nd.get_allocator())
	{}
	ValueNode(const ValueNode& nd, allocator_type a)
		: name(nd.name), container(nd.container, a), Value(nd.Value)
	{}
	//! \since build 330
	DefDeMoveCtor(ValueNode)
	ValueNode(ValueNode&& nd, allocator_type a) ynothrowv
		: name(std::move(nd.name)), container(std::move(nd.container), a),
		Value(std::move(nd.Value))
	{}
	//@}

	//! \since build 768
	//@{
	//! \brief 复制赋值：使用参数副本和交换操作。
	PDefHOp(ValueNode&, =, const ValueNode& nd)
		ImplRet(ystdex::copy_and_swap(*this, nd))
	/*!
	\pre 被转移的参数不是被子节点容器直接或间接所有的其它节点。
	\warning 违反前置条件的转移可能引起循环引用。
	*/
	DefDeMoveAssignment(ValueNode)
	//@}

	//! \since build 730
	//@{
	PDefHOp(const ValueNode&, +=, const ValueNode& nd)
		ImplRet(Add(nd), *this)
	PDefHOp(const ValueNode&, +=, ValueNode&& nd)
		ImplRet(Add(std::move(nd)), *this)

	PDefHOp(const ValueNode&, -=, const ValueNode& nd)
		ImplRet(Remove(nd), *this)
	PDefHOp(const ValueNode&, -=, const string& str)
		ImplRet(Remove(str), *this)
	/*!
	\brief 替换同名子节点。
	\return 自身引用。
	*/
	//@{
	PDefHOp(ValueNode&, /=, const ValueNode& nd)
		ImplRet(*this %= nd, *this)
	PDefHOp(ValueNode&, /=, ValueNode&& nd)
		ImplRet(*this %= std::move(nd), *this)
	//@}
	/*!
	\brief 替换同名子节点。
	\return 子节点引用。
	\since build 792
	*/
	//@{
	ValueNode&
	operator%=(const ValueNode&);
	ValueNode&
	operator%=(ValueNode&&);
	//@}
	//@}

	//! \since build 336
	DefBoolNeg(explicit, bool(Value) || !container.empty())

	//! \since build 673
	friend PDefHOp(bool, ==, const ValueNode& x, const ValueNode& y) ynothrow
		ImplRet(x.name == y.name)
	//! \since build 679
	friend PDefHOp(bool, ==, const ValueNode& x, const string& str) ynothrow
		ImplRet(x.name == str)
	//! \since build 730
	template<typename _tKey>
	YB_ATTR_nodiscard YB_PURE friend bool
	operator==(const ValueNode& x, const _tKey& k) ynothrow
	{
		return x.name == k;
	}

	//! \since build 673
	YB_ATTR_nodiscard YB_PURE friend
		PDefHOp(bool, <, const ValueNode& x, const ValueNode& y) ynothrow
		ImplRet(x.name < y.name)
	//! \since build 678
	YB_ATTR_nodiscard YB_PURE friend
		PDefHOp(bool, <, const ValueNode& x, const string& str) ynothrow
		ImplRet(x.name < str)
	//! \since build 730
	template<typename _tKey>
	YB_ATTR_nodiscard YB_PURE friend bool
	operator<(const ValueNode& x, const _tKey& k) ynothrow
	{
		return x.name < k;
	}
	//! \since build 730
	template<typename _tKey>
	YB_ATTR_nodiscard YB_PURE friend bool
	operator<(const _tKey& k, const ValueNode& y) ynothrow
	{
		return k < y.name;
	}
	//! \since build 679
	YB_ATTR_nodiscard YB_PURE friend
		PDefHOp(bool, >, const ValueNode& x, const string& str) ynothrow
		ImplRet(x.name > str)
	//! \since build 730
	template<typename _tKey>
	YB_ATTR_nodiscard YB_PURE friend bool
	operator>(const ValueNode& x, const _tKey& k) ynothrow
	{
		return x.name > k;
	}
	//! \since build 730
	template<typename _tKey>
	YB_ATTR_nodiscard YB_PURE friend bool
	operator>(const _tKey& k, const ValueNode& y) ynothrow
	{
		return k > y.name;
	}

	//! \since build 680
	template<typename _tKey>
	ValueNode&
	operator[](_tKey&& k)
	{
		return Deref(try_emplace(k, NoContainer, yforward(k)).first);
	}
	//! \since build 792
	template<class _tCon>
	ValueNode&
	operator[](const ystdex::path<_tCon>& pth)
	{
		auto p(this);

		for(const auto& n : pth)
			p = &(*p)[n];
		return *p;
	}

	/*!
	\brief 取子节点容器引用。
	\since build 664
	*/
	DefGetter(const ynothrow, const Container&, Container, container)
	/*!
	\brief 取子节点容器引用。
	\since build 667
	*/
	DefGetter(ynothrow, Container&, ContainerRef, container)
	DefGetter(const ynothrow, const string&, Name, name)

	/*!
	\pre 被转移的参数不是被子节点容器直接或间接所有的其它节点。
	\warning 违反前置条件的转移可能引起循环引用。
	*/
	//@{
	/*!
	\brief 设置子节点容器内容。
	\since build 774
	*/
	//@{
	PDefH(void, SetChildren, const Container& con)
		ImplExpr(container = con)
	PDefH(void, SetChildren, Container&& con)
		ImplExpr(container = std::move(con))
	//! \since build 776
	PDefH(void, SetChildren, ValueNode&& nd)
		ImplExpr(container = std::move(nd.container))
	//@}
	/*!
	\pre 断言：分配器相等。
	\note 设置子节点容器和值的内容。
	\note 除转移外非强异常安全。
	\since build 734
	*/
	//@{
	//! \since build 776
	template<class _tCon, class _type>
	yimpl(ystdex::enable_if_t)<
		ystdex::and_<std::is_assignable<Container, _tCon&&>,
		std::is_assignable<ValueObject, _type&&>>::value>
	SetContent(_tCon&& con, _type&& val) ynoexcept(ystdex::and_<
		std::is_nothrow_assignable<Container, _tCon&&>,
		std::is_nothrow_assignable<ValueObject, _type&&>>())
	{
		container = yforward(con);
		Value = yforward(val);
	}
	PDefH(void, SetContent, const ValueNode& nd)
		ImplExpr(SetContent(nd.GetContainer(), nd.Value))
	//! \pre 间接断言：容器分配器和参数的容器分配器相等。
	PDefH(void, SetContent, ValueNode&& nd)
		ImplExpr(SwapContainer(nd), Value = std::move(nd.Value))
	//@}
	//@}

	//! \since build 667
	PDefH(bool, Add, const ValueNode& nd)
		ImplRet(insert(nd).second)
	//! \since build 667
	PDefH(bool, Add, ValueNode&& nd)
		ImplRet(insert(std::move(nd)).second)

	/*!
	\brief 添加参数节点指定容器和值的子节点。
	\since build 845
	*/
	//@{
	//! \sa ystdex::try_emplace
	template<typename _tKey, typename _tNode,
		yimpl(typename = ystdex::enable_if_t<
		std::is_same<ValueNode&, ystdex::remove_cvref_t<_tNode>&>::value>)>
	yimpl(ystdex::enable_if_inconvertible_t)<_tKey&&, const_iterator, bool>
	AddChild(_tKey&& k, _tNode&& nd)
	{
		return try_emplace(k, yforward(nd).container, yforward(k),
			yforward(nd).Value).second;
	}
	//! \sa ystdex::try_emplace_hint
	//@{
	template<typename _tKey, typename _tNode,
		yimpl(typename = ystdex::enable_if_t<
		std::is_same<ValueNode&, ystdex::remove_cvref_t<_tNode>&>::value>)>
	bool
	AddChild(const_iterator hint, _tKey&& k, _tNode&& nd)
	{
		return try_emplace_hint(hint, k, yforward(nd).container, yforward(k),
			yforward(nd).Value);
	}
	//@}
	//@}

	/*!
	\brief 添加参数指定值的子节点。
	\sa AddValueTo
	\since build 757
	*/
	//@{
	template<typename _tKey, typename... _tParams>
	inline
		yimpl(ystdex::enable_if_inconvertible_t)<_tKey&&, const_iterator, bool>
	AddValue(_tKey&& k, _tParams&&... args)
	{
		return AddValueTo(container, k, yforward(args)...);
	}
	template<typename _tKey, typename... _tParams>
	inline bool
	AddValue(const_iterator hint, _tKey&& k, _tParams&&... args)
	{
		return AddValueTo(hint, container, k, yforward(args)...);
	}
	//@}

	//! \brief 向容器添加参数指定值的子节点。
	//@{
	/*!
	\sa ystdex::try_emplace
	\since build 674
	*/
	template<typename _tKey, typename... _tParams>
	static inline bool
	AddValueTo(Container& con, _tKey&& k, _tParams&&... args)
	{
		return ystdex::try_emplace(con, k, NoContainer, yforward(k),
			yforward(args)...).second;
	}
	/*!
	\sa ystdex::try_emplace_hint
	\since build 845
	*/
	template<typename _tKey, typename... _tParams>
	static inline bool
	AddValueTo(const_iterator hint, Container& con, _tKey&& k,
		_tParams&&... args)
	{
		return ystdex::try_emplace_hint(con, hint, k, NoContainer, yforward(k),
			yforward(args)...).second;
	}
	//@}

	//! \note 清理容器和修改值的操作之间的顺序未指定。
	//@{
	/*!
	\brief 清除节点。
	\post <tt>!Value && empty()</tt> 。
	\since build 666
	*/
	PDefH(void, Clear, ) ynothrow
		ImplExpr(Value.Clear(), ClearContainer())
	//@}

	/*!
	\brief 清除节点容器。
	\post \c empty() 。
	\since build 667
	*/
	PDefH(void, ClearContainer, ) ynothrow
		ImplExpr(container.clear())

	/*!
	\note 允许被参数中被复制的对象直接或间接地被目标引用。
	\since build 913
	*/
	//@{
	PDefH(void, CopyContainer, const ValueNode& nd)
		ImplExpr(GetContainerRef() = Container(nd.GetContainer()))

	PDefH(void, CopyContent, const ValueNode& nd)
		ImplExpr(SetContent(ValueNode(nd)))

	PDefH(void, CopyValue, const ValueNode& nd)
		ImplExpr(Value = ValueObject(nd.Value))
	//@}

	//! \brief 递归创建容器副本。
	//@{
	//! \since build 767
	static Container
	CreateRecursively(const Container&, IValueHolder::Creation);
	//! \since build 845
	template<class _tCon, typename _fCallable,
		yimpl(typename = ystdex::enable_if_t<
		std::is_same<Container&, ystdex::remove_cvref_t<_tCon>&>::value>)>
	static Container
	CreateRecursively(_tCon&& con, _fCallable f)
	{
		Container res(con.get_allocator());

		for(auto&& nd : con)
			res.emplace(CreateRecursively(
				ystdex::forward_like<_tCon>(nd.container), f), nd.GetName(),
				ystdex::invoke(f, ystdex::forward_like<_tCon>(nd.Value)));
		return res;
	}

	//! \since build 767
	PDefH(Container, CreateWith, IValueHolder::Creation c) const
		ImplRet(CreateRecursively(container, c))
	//! \since build 834
	//@{
	template<typename _fCallable>
	Container
	CreateWith(_fCallable f) &
	{
		return CreateRecursively(container, f);
	}
	template<typename _fCallable>
	Container
	CreateWith(_fCallable f) const&
	{
		return CreateRecursively(container, f);
	}
	template<typename _fCallable>
	Container
	CreateWith(_fCallable f) &&
	{
		return CreateRecursively(std::move(container), f);
	}
	template<typename _fCallable>
	Container
	CreateWith(_fCallable f) const&&
	{
		return CreateRecursively(std::move(container), f);
	}
	//@}
	//@}

	/*!
	\pre 断言：参数不是 \c *this 。
	\note 允许被参数中被转移的对象直接或间接地被目标引用。
	\since build 913
	*/
	//@{
	/*!
	\brief 转移容器。
	\pre 间接断言：容器分配器和参数的容器分配器相等。

	转移参数指定的节点的容器到对象。
	转移后的节点的容器是转移前的参数的容器。
	*/
	void
	MoveContainer(ValueNode&&);

	/*!
	\brief 转移内容。
	\pre 间接断言：容器分配器和参数的容器分配器相等。
	\since build 844

	转移参数指定的节点的内容到对象。
	转移后的节点的内容是转移前的参数的内容。
	*/
	void
	MoveContent(ValueNode&&);

	/*!
	\brief 转移值数据成员。

	转移参数指定的节点的值数据成员到对象。
	转移后的节点的值数据成员是转移前的参数内容。
	*/
	void
	MoveValue(ValueNode&&);
	//@}

	/*!
	\brief 若指定名称子节点不存在则按指定值初始化。
	\since build 781
	*/
	//@{
	//! \return 按指定名称查找的指定类型的子节点的值的引用。
	//@{
	/*!
	\sa ValueObject::Access
	\since build 681
	*/
	template<typename _type, typename _tString, typename... _tParams>
	inline _type&
	Place(_tString&& str, _tParams&&... args)
	{
		return PlaceValue<_type>(yforward(str), yforward(args)...).template
			Access<_type>();
	}

	/*!
	\pre 间接断言：存储对象已存在时类型和访问的类型一致。
	\sa ValueObject::GetObject
	*/
	template<typename _type, typename _tString, typename... _tParams>
	inline _type&
	PlaceUnchecked(_tString&& str, _tParams&&... args)
	{
		return PlaceValue<_type>(yforward(str), yforward(args)...).template
			GetObject<_type>();
	}
	//@}

	//! \brief 初始化的值对象引用。
	template<typename _type, typename _tString, typename... _tParams>
	inline ValueObject&
	PlaceValue(_tString&& str, _tParams&&... args)
	{
		return try_emplace(str, NoContainer, yforward(str),
			in_place_type<_type>, yforward(args)...).first->Value;
	}
	//@}

	PDefH(bool, Remove, const ValueNode& nd)
		ImplRet(erase(nd) != 0)
	//! \since build 754
	PDefH(iterator, Remove, const_iterator i)
		ImplRet(erase(i))
	//! \since build 680
	template<typename _tKey, yimpl(typename = enable_if_key_t<_tKey>)>
	inline bool
	Remove(const _tKey& k)
	{
		return ystdex::erase_first(container, k);
	}

	/*!
	\brief 复制满足条件的子节点。
	\since build 664
	*/
	template<typename _func>
	Container
	SelectChildren(_func f) const
	{
		Container res(get_allocator());

		ystdex::for_each_if(begin(), end(), f, [&](const ValueNode& nd){
			res.insert(nd);
		});
		return res;
	}

	//! \since build 667
	//@{
	//! \brief 转移满足条件的子节点。
	template<typename _func>
	Container
	SplitChildren(_func f)
	{
		Container res(get_allocator());

		std::for_each(begin(), end(), [&](const ValueNode& nd){
			res.emplace(NoContainer, nd.GetName());
		});
		ystdex::for_each_if(begin(), end(), f, [&, this](const ValueNode& nd){
			const auto& child_name(nd.GetName());

			Deref(res.find(child_name)).Value = std::move(nd.Value);
			Remove(child_name);
		});
		return res;
	}

	/*!
	\pre 断言：分配器相等。
	\warning 不检查容器之间的所有权关系，可能引起循环引用。
	\since build 844
	*/
	//@{
	//! \brief 交换容器。
	PDefH(void, SwapContainer, ValueNode& nd) ynothrowv
		ImplExpr(YAssert(get_allocator() == nd.get_allocator(),
			"Invalid allocator found."), container.swap(nd.container))

	//! \brief 交换容器和值。
	void
	SwapContent(ValueNode&) ynothrowv;
	//@}
	//@}

	/*!
	\brief 抛出索引越界异常。
	\throw std::out_of_range 索引越界。
	\since build 849
	*/
	YB_NORETURN static void
	ThrowIndexOutOfRange(size_t);

	/*!
	\brief 抛出名称错误异常。
	\pre 间接断言：间接参数的数据指针非空。
	\throw std::out_of_range 名称错误。
	\since build 849
	*/
	YB_NORETURN static void
	ThrowWrongNameFound(string_view);

	//! \since build 460
	//@{
	PDefH(iterator, begin, )
		ImplRet(GetContainerRef().begin())
	PDefH(const_iterator, begin, ) const
		ImplRet(GetContainer().begin())

	//! \since build 767
	DefFwdTmpl(, pair<iterator YPP_Comma bool>, emplace,
		container.emplace(yforward(args)...))

	//! \since build 667
	DefFwdTmpl(, iterator, emplace_hint,
		container.emplace_hint(yforward(args)...))

	//! \since build 598
	YB_ATTR_nodiscard PDefH(bool, empty, ) const ynothrow
		ImplRet(container.empty())

	PDefH(iterator, end, )
		ImplRet(GetContainerRef().end())
	PDefH(const_iterator, end, ) const
		ImplRet(GetContainer().end())
	//@}

	//! \since build 761
	DefFwdTmpl(-> decltype(container.erase(yforward(args)...)), auto,
		erase, container.erase(yforward(args)...))

	//! \since build 844
	YB_ATTR_nodiscard YB_PURE
		PDefH(allocator_type, get_allocator, ) const ynothrow
		ImplRet(container.get_allocator())

	//! \since build 667
	DefFwdTmpl(-> decltype(container.insert(yforward(args)...)), auto,
		insert, container.insert(yforward(args)...))

	//! \since build 681
	//@{
	template<typename _tKey, class _tParam>
	yimpl(ystdex::enable_if_inconvertible_t)<_tKey&&, const_iterator,
		std::pair<iterator, bool>>
	insert_or_assign(_tKey&& k, _tParam&& arg)
	{
		// NOTE: See comments in %try_emplace.
		return ystdex::insert_or_assign(container, yforward(k), yforward(arg));
	}
	template<typename _tKey, class _tParam>
	iterator
	insert_or_assign(const_iterator hint, _tKey&& k, _tParam&& arg)
	{
		return ystdex::insert_or_assign_hint(container, hint, yforward(k),
			yforward(arg));
	}
	//@}

	//! \since build 696
	//@{
	YB_ATTR_nodiscard YB_PURE PDefH(reverse_iterator, rbegin, )
		ImplRet(GetContainerRef().rbegin())
	YB_ATTR_nodiscard YB_PURE PDefH(const_reverse_iterator, rbegin, ) const
		ImplRet(GetContainer().rbegin())

	YB_ATTR_nodiscard YB_PURE PDefH(reverse_iterator, rend, )
		ImplRet(GetContainerRef().rend())
	YB_ATTR_nodiscard YB_PURE PDefH(const_reverse_iterator, rend, ) const
		ImplRet(GetContainer().rend())
	//@}

	//! \since build 598
	YB_ATTR_nodiscard YB_PURE PDefH(size_t, size, ) const ynothrow
		ImplRet(container.size())

	/*!
	\brief 交换。
	\pre 分配器相等。
	\since build 844
	*/
	YF_API friend void
	swap(ValueNode&, ValueNode&) ynothrowv;

	//! \since build 681
	template<typename _tKey, typename... _tParams>
	yimpl(ystdex::enable_if_inconvertible_t)<_tKey&&, const_iterator,
		std::pair<iterator, bool>>
	try_emplace(_tKey&& k, _tParams&&... args)
	{
		// NOTE: This is not efficient as a single %emplace when %k is not
		//	equivalent to %cont_traits::get_value_key(yforward(args)...), as
		//	per the notes in %ystdex::mapped_set. However, this function is
		//	needed as a convenient base to other operations and there is in
		//	general no superior alternatives (when it is needed to decide to
		//	insert on two different keys). Thus, it is kept as a public
		//	function. All other functions using this one would keep the
		//	equivalence, so they are still preferred unless the difference of
		//	keys is intended, though.
		return ystdex::try_emplace(container, yforward(k), yforward(args)...);
	}

	//! \since build 844
	template<typename _tKey, typename... _tParams>
	iterator
	try_emplace_hint(const_iterator hint, _tKey&& k, _tParams&&... args)
	{
		return ystdex::try_emplace_hint(container, hint, yforward(k),
			yforward(args)...);
	}
};

//! \relates ValueNode
//@{
//! \since build 666
//@{
/*!
\brief 访问节点的指定类型对象。
\exception std::bad_cast 空实例或类型检查失败。
*/
//@{
template<typename _type>
YB_ATTR_nodiscard YB_PURE inline _type&
Access(ValueNode& nd)
{
	return nd.Value.Access<_type>();
}
template<typename _type>
YB_ATTR_nodiscard YB_PURE inline const _type&
Access(const ValueNode& nd)
{
	return nd.Value.Access<_type>();
}
//@}

//! \since build 670
//@{
//! \brief 访问节点的指定类型对象指针。
//@{
template<typename _type>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<_type>
AccessPtr(ValueNode& nd) ynothrow
{
	return nd.Value.AccessPtr<_type>();
}
template<typename _type>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<const _type>
AccessPtr(const ValueNode& nd) ynothrow
{
	return nd.Value.AccessPtr<_type>();
}
//@}
/*!
\brief 访问节点的指定类型对象指针。
\since build 749
*/
//@{
template<typename _type, typename _tNodeOrPointer>
YB_ATTR_nodiscard YB_PURE inline auto
AccessPtr(observer_ptr<_tNodeOrPointer> p) ynothrow
	-> decltype(YSLib::AccessPtr<_type>(*p))
{
	return p ? YSLib::AccessPtr<_type>(*p) : nullptr;
}
//@}
//@}
//@}

//! \since build 749
//@{
//! \brief 取指定名称指称的值。
YB_ATTR_nodiscard YF_API YB_PURE ValueObject
GetValueOf(observer_ptr<const ValueNode>);

//! \brief 取指定名称指称的值的指针。
YB_ATTR_nodiscard YF_API YB_PURE observer_ptr<const ValueObject>
GetValuePtrOf(observer_ptr<const ValueNode>);
//@}
//@}


//! \since build 730
//@{
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE observer_ptr<ValueNode>
AccessNodePtr(ValueNode::Container*, const _tKey&) ynothrow;
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE observer_ptr<const ValueNode>
AccessNodePtr(const ValueNode::Container*, const _tKey&) ynothrow;

/*!
\brief 访问节点。
\throw std::out_of_range 未找到对应节点。
*/
//@{
//! \since build 670
YB_ATTR_nodiscard YF_API YB_PURE ValueNode&
AccessNode(ValueNode::Container*, const string&);
//! \since build 670
YB_ATTR_nodiscard YF_API YB_PURE const ValueNode&
AccessNode(const ValueNode::Container*, const string&);
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE ValueNode&
AccessNode(ValueNode::Container* p_con, const _tKey& name)
{
	if(const auto p = YSLib::AccessNodePtr(p_con, name))
		return *p;
	ValueNode::ThrowWrongNameFound(name);
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE const ValueNode&
AccessNode(const ValueNode::Container* p_con, const _tKey& name)
{
	if(const auto p = YSLib::AccessNodePtr(p_con, name))
		return *p;
	ValueNode::ThrowWrongNameFound(name);
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE inline ValueNode&
AccessNode(observer_ptr<ValueNode::Container> p_con, const _tKey& name)
{
	return YSLib::AccessNode(p_con.get(), name);
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE inline const ValueNode&
AccessNode(observer_ptr<const ValueNode::Container> p_con, const _tKey& name)
{
	return YSLib::AccessNode(p_con.get(), name);
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE inline ValueNode&
AccessNode(ValueNode::Container& con, const _tKey& name)
{
	return YSLib::AccessNode(&con, name);
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE inline const ValueNode&
AccessNode(const ValueNode::Container& con, const _tKey& name)
{
	return YSLib::AccessNode(&con, name);
}
/*!
\note 时间复杂度 O(n) 。
\since build 670
*/
//@{
YB_ATTR_nodiscard YF_API YB_PURE ValueNode&
AccessNode(ValueNode&, size_t);
YB_ATTR_nodiscard YF_API YB_PURE const ValueNode&
AccessNode(const ValueNode&, size_t);
//@}
template<typename _tKey,
	yimpl(typename = ValueNode::enable_if_key_t<_tKey>)>
YB_ATTR_nodiscard YB_PURE inline ValueNode&
AccessNode(ValueNode& nd, const _tKey& name)
{
	return YSLib::AccessNode(nd.GetContainerRef(), name);
}
template<typename _tKey,
	yimpl(typename = ValueNode::enable_if_key_t<_tKey>)>
YB_ATTR_nodiscard YB_PURE inline const ValueNode&
AccessNode(const ValueNode& nd, const _tKey& name)
{
	return YSLib::AccessNode(nd.GetContainer(), name);
}
//! \since build 670
//@{
//! \note 使用 ADL AccessNode 。
template<class _tNode, typename _tIn>
YB_ATTR_nodiscard YB_PURE _tNode&&
AccessNode(_tNode&& nd, _tIn first, _tIn last)
{
	return std::accumulate(first, last, ystdex::ref(nd),
		[](_tNode&& x, decltype(*first) c){
		return ystdex::ref(AccessNode(x, c));
	});
}
//! \note 使用 ADL begin 和 end 指定范围迭代器。
template<class _tNode, typename _tRange,
	yimpl(typename = typename ystdex::enable_if_t<
	!std::is_constructible<const string&, const _tRange&>::value>)>
YB_ATTR_nodiscard YB_PURE inline auto
AccessNode(_tNode&& nd, const _tRange& c)
	-> decltype(YSLib::AccessNode(yforward(nd), begin(c), end(c)))
{
	return YSLib::AccessNode(yforward(nd), begin(c), end(c));
}
//@}
//@}

//! \brief 访问节点指针。
//@{
//! \since build 670
YB_ATTR_nodiscard YF_API YB_PURE observer_ptr<ValueNode>
AccessNodePtr(ValueNode::Container&, const string&) ynothrow;
//! \since build 670
YB_ATTR_nodiscard YF_API YB_PURE observer_ptr<const ValueNode>
AccessNodePtr(const ValueNode::Container&, const string&) ynothrow;
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE observer_ptr<ValueNode>
AccessNodePtr(ValueNode::Container& con, const _tKey& name) ynothrow
{
	return make_observer(ystdex::call_value_or<ValueNode*>(ystdex::addrof<>(),
		con.find(name), {}, ystdex::end(con)));
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE observer_ptr<const ValueNode>
AccessNodePtr(const ValueNode::Container& con, const _tKey& name) ynothrow
{
	return make_observer(ystdex::call_value_or<const ValueNode*>(
		ystdex::addrof<>(), con.find(name), {}, ystdex::end(con)));
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<ValueNode>
AccessNodePtr(ValueNode::Container* p_con, const _tKey& name) ynothrow
{
	return p_con ? YSLib::AccessNodePtr(*p_con, name) : nullptr;
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<const ValueNode>
AccessNodePtr(const ValueNode::Container* p_con, const _tKey& name) ynothrow
{
	return p_con ? YSLib::AccessNodePtr(*p_con, name) : nullptr;
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<ValueNode>
AccessNodePtr(observer_ptr<ValueNode::Container> p_con, const _tKey& name)
	ynothrow
{
	return p_con ? YSLib::AccessNodePtr(*p_con, name) : nullptr;
}
template<typename _tKey>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<const ValueNode>
AccessNodePtr(observer_ptr<const ValueNode::Container> p_con, const _tKey& name)
	ynothrow
{
	return p_con ? YSLib::AccessNodePtr(*p_con, name) : nullptr;
}
/*!
\note 时间复杂度 O(n) 。
\since build 670
*/
//@{
YB_ATTR_nodiscard YF_API YB_PURE observer_ptr<ValueNode>
AccessNodePtr(ValueNode&, size_t);
YB_ATTR_nodiscard YF_API YB_PURE observer_ptr<const ValueNode>
AccessNodePtr(const ValueNode&, size_t);
//@}
template<typename _tKey, yimpl(typename = typename ystdex::enable_if_t<
	ystdex::is_interoperable<const _tKey&, const string&>::value>)>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<ValueNode>
AccessNodePtr(ValueNode& nd, const _tKey& name)
{
	return YSLib::AccessNodePtr(nd.GetContainerRef(), name);
}
template<typename _tKey, yimpl(typename = typename ystdex::enable_if_t<
	ystdex::is_interoperable<const _tKey&, const string&>::value>)>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<const ValueNode>
AccessNodePtr(const ValueNode& nd, const _tKey& name)
{
	return YSLib::AccessNodePtr(nd.GetContainer(), name);
}
//! \since build 670
//@{
//! \note 使用 ADL AccessNodePtr 。
template<class _tNode, typename _tIn>
YB_ATTR_nodiscard YB_PURE auto
AccessNodePtr(_tNode&& nd, _tIn first, _tIn last)
	-> decltype(make_obsrever(std::addressof(nd)))
{
	// TODO: Simplified using algorithm template?
	for(auto p(make_observer(std::addressof(nd))); p && first != last; ++first)
		p = AccessNodePtr(*p, *first);
	return first;
}
//! \note 使用 ADL begin 和 end 指定范围迭代器。
template<class _tNode, typename _tRange,
	yimpl(typename = typename ystdex::enable_if_t<
	!std::is_constructible<const string&, const _tRange&>::value>)>
YB_ATTR_nodiscard YB_PURE inline auto
AccessNodePtr(_tNode&& nd, const _tRange& c)
	-> decltype(YSLib::AccessNodePtr(yforward(nd), begin(c), end(c)))
{
	return YSLib::AccessNodePtr(yforward(nd), begin(c), end(c));
}
//@}
//@}
//@}

//! \since build 670
//@{
/*!
\exception std::bad_cast 空实例或类型检查失败 。
\relates ValueNode
*/
//@{
/*!
\brief 访问子节点的指定类型对象。
\note 使用 ADL AccessNode 。
*/
//@{
template<typename _type, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline _type&
AccessChild(ValueNode& nd, _tParams&&... args)
{
	return Access<_type>(AccessNode(nd, yforward(args)...));
}
template<typename _type, typename... _tParams>
inline const _type&
AccessChild(const ValueNode& nd, _tParams&&... args)
{
	return Access<_type>(AccessNode(nd, yforward(args)...));
}
//@}

//! \brief 访问指定名称的子节点的指定类型对象的指针。
//@{
template<typename _type, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<_type>
AccessChildPtr(ValueNode& nd, _tParams&&... args) ynothrow
{
	return AccessPtr<_type>(
		AccessNodePtr(nd.GetContainerRef(), yforward(args)...));
}
template<typename _type, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<const _type>
AccessChildPtr(const ValueNode& nd, _tParams&&... args) ynothrow
{
	return AccessPtr<_type>(
		AccessNodePtr(nd.GetContainer(), yforward(args)...));
}
template<typename _type, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<_type>
AccessChildPtr(ValueNode* p_node, _tParams&&... args) ynothrow
{
	return p_node ? AccessChildPtr<_type>(*p_node, yforward(args)...) : nullptr;
}
template<typename _type, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline observer_ptr<const _type>
AccessChildPtr(const ValueNode* p_node, _tParams&&... args) ynothrow
{
	return p_node ? AccessChildPtr<_type>(*p_node, yforward(args)...) : nullptr;
}
//@}
//@}
//@}


//! \since build 867
//@{
//! \note 结果不含子节点。
//@{
//! \since build 852
YB_ATTR_nodiscard YB_PURE inline
	PDefH(const ValueNode&, AsNode, const ValueNode& nd) ynothrow
	ImplRet(nd)
//! \brief 传递指定名称和值参数构造值类型节点。
//@{
template<typename _tString, typename... _tParams,
	yimpl(typename = ystdex::enable_if_inconvertible_t<_tString&&,
	const ValueNode::allocator_type&>)>
YB_ATTR_nodiscard YB_PURE inline ValueNode
AsNode(_tString&& str, _tParams&&... args)
{
	return {NoContainer, yforward(str), yforward(args)...};
}
template<typename _tString, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline ValueNode
AsNode(const ValueNode::allocator_type& a, _tString&& str, _tParams&&... args)
{
	return {std::allocator_arg, a, NoContainer, yforward(str),
		yforward(args)...};
}
//@}

//! \brief 传递指定名称和退化值参数构造值类型节点。
//@{
template<typename _tString, typename... _tParams,
	yimpl(typename = ystdex::enable_if_inconvertible_t<_tString&&,
	const ValueNode::allocator_type&>)>
YB_ATTR_nodiscard YB_PURE inline ValueNode
MakeNode(_tString&& str, _tParams&&... args)
{
	return {NoContainer, yforward(str), ystdex::decay_copy(args)...};
}
template<typename _tString, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline ValueNode
MakeNode(const ValueNode::allocator_type& a, _tString&& str, _tParams&&... args)
{
	return {std::allocator_arg, a, NoContainer, yforward(str),
		ystdex::decay_copy(args)...};
}
//@}
//@}

/*!
\brief 取指定名称和转换为字符串的值类型节点。
\note 使用 ADL to_string 和 ADL to_pmr_string 转换。
\todo 对特定的已被 std::to_string 支持的类型使用更高效的字符串转换算法实现。
*/
//@{
template<typename _tString, typename... _tParams,
	yimpl(typename = ystdex::enable_if_inconvertible_t<_tString&&,
	const ValueNode::allocator_type&>)>
YB_ATTR_nodiscard YB_PURE inline ValueNode
StringifyToNode(_tString&& str, _tParams&&... args)
{
	return {NoContainer, yforward(str),
		to_pmr_string(to_string(yforward(args)...))};
}
template<typename _tString, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline ValueNode
StringifyToNode(const ValueNode::allocator_type& a, _tString&& str,
	_tParams&&... args)
{
	return {std::allocator_arg, a, NoContainer, yforward(str),
		to_pmr_string(to_string(yforward(args)...))};
}
//@}
//@}


/*!
\brief 从引用参数取值类型节点：返回自身。
\since build 338
*/
//@{
YB_ATTR_nodiscard YB_PURE inline
	PDefH(const ValueNode&, UnpackToNode, const ValueNode& arg)
	ImplRet(arg)
YB_ATTR_nodiscard YB_PURE inline
	PDefH(ValueNode&&, UnpackToNode, ValueNode&& arg)
	ImplRet(std::move(arg))
//@}
/*!
\brief 从参数取以指定分量为初始化参数的值类型节点。
\note 取分量同 std::get ，但使用 ADL 。仅取前两个分量。
\since build 338
*/
template<class _tPack>
YB_ATTR_nodiscard YB_PURE inline ValueNode
UnpackToNode(_tPack&& pk)
{
	return {0, get<0>(yforward(pk)),
		ValueObject(ystdex::decay_copy(get<1>(yforward(pk))))};
}

/*!
\brief 取指定值类型节点为成员的节点容器。
\since build 598
*/
//@{
template<typename _tElem>
YB_ATTR_nodiscard YB_PURE inline ValueNode::Container
CollectNodes(std::initializer_list<_tElem> il)
{
	return il;
}
template<typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline ValueNode::Container
CollectNodes(_tParams&&... args)
{
	return {yforward(args)...};
}
//@}

/*!
\brief 取以指定分量为参数对应初始化得到的值类型节点为子节点的值类型节点。
\since build 338
*/
template<typename _tString, typename... _tParams>
YB_ATTR_nodiscard YB_PURE inline ValueNode
PackNodes(_tString&& name, _tParams&&... args)
{
	return {CollectNodes(UnpackToNode(yforward(args))...), yforward(name)};
}


//! \since build 674
//@{
//! \brief 移除空子节点。
YF_API void
RemoveEmptyChildren(ValueNode::Container&) ynothrow;

/*!
\brief 移除第一个子节点。
\pre 断言：节点容器非空。
*/
//@{
YF_API void
RemoveHead(ValueNode::Container&) ynothrowv;
inline PDefH(void, RemoveHead, ValueNode& nd) ynothrowv
	ImplExpr(RemoveHead(nd.GetContainerRef()))
//@}
//@}

/*!
\brief 根据节点和节点容器创建操作设置目标节点的值或子节点。
\note 可用于创建副本。
\warning 不检查嵌套深度，不支持嵌套调用安全。
\since build 834
*/
template<typename _tNode, typename _fCallable>
void
SetContentWith(ValueNode& dst, _tNode&& nd, _fCallable f)
{
	// NOTE: Similar reason but different to the implementation of
	//	%ValueNode::MoveContent.
	auto con(yforward(nd).CreateWith(f));
	auto vo(ystdex::invoke(f, yforward(nd).Value));

	dst.SetContent(std::move(con), std::move(vo));
}


/*!
\brief 判断字符串是否是前缀索引：一个指定字符和非负整数的组合。
\pre 断言：字符串参数的数据指针非空。
\note 仅测试能被 <tt>unsigned long</tt> 表示的整数。
\since build 659
*/
YB_ATTR_nodiscard YF_API YB_PURE bool
IsPrefixedIndex(string_view, char = '$');

/*!
\brief 转换节点大小为新的节点索引值。
\return 保证按字典序递增的字符串。
\note 重复以子节点数作为参数使用作为新节点的名称，可用于插入不重复节点。
\note 字符串不保证可读。
\note 具体算法未指定。相同实现的算法结果保证稳定，但不保证版本间稳定。
\since build 598
*/
YB_ATTR_nodiscard YF_API YB_PURE string
MakeIndex(size_t);

//! \throw std::invalid_argument 存在子节点但名称不是前缀索引。
//@{
/*!
\brief 解码节点名称的前缀索引。
\pre 断言：参数的数据指针非空。
\return 若不存在子节点则为 \c size_t(-1) ，否则为名称对应的索引。
\since build 851
*/
YB_ATTR_nodiscard YF_API size_t
DecodeIndex(string_view);
//@}


/*!
\brief 节点序列容器。
\since build 597

除分配器外满足和 std::vector 相同的要求的模板的一个实例，元素为 ValueNode 类型。
*/
using NodeSequence = yimpl(YSLib::vector)<ValueNode>;


/*!
\brief 包装节点的组合字面量。
\since build 598
*/
class YF_API NodeLiteral final
{
private:
	ValueNode node;

public:
	//! \since build 599
	NodeLiteral(const ValueNode& nd)
		: node(nd)
	{}
	//! \since build 599
	NodeLiteral(ValueNode&& nd)
		: node(std::move(nd))
	{}
	NodeLiteral(const string& str)
		: node(NoContainer, str)
	{}
	NodeLiteral(const string& str, string val)
		: node(NoContainer, str, std::move(val))
	{}
	template<typename _tLiteral = NodeLiteral>
	NodeLiteral(const string& str, std::initializer_list<_tLiteral> il)
		: node(NoContainer, str, NodeSequence(il.begin(), il.end()))
	{}
	//! \since build 674
	template<typename _tLiteral = NodeLiteral, class _tString,
		typename... _tParams>
	NodeLiteral(ListContainerTag, _tString&& str,
		std::initializer_list<_tLiteral> il, _tParams&&... args)
		: node(ValueNode::Container(il.begin(), il.end()), yforward(str),
		yforward(args)...)
	{}
	DefDeCopyMoveCtorAssignment(NodeLiteral)

	DefCvt(ynothrow, ValueNode&, node)
	DefCvt(const ynothrow, const ValueNode&, node)

	//! \since build 853
	DefGetter(ynothrow, ValueNode&, NodeRef, node)
};

/*!
\brief 传递参数构造值类型节点字面量。
\relates NodeLiteral
\since build 668
*/
template<typename _tString, typename _tLiteral = NodeLiteral>
YB_ATTR_nodiscard inline YB_PURE NodeLiteral
AsNodeLiteral(_tString&& name, std::initializer_list<_tLiteral> il)
{
	return {ListContainer, yforward(name), il};
}

} // namespace YSLib;

#endif

