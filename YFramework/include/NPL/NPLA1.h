﻿/*
	© 2014-2016 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file NPLA1.h
\ingroup NPL
\brief NPLA1 公共接口。
\version r1715
\author FrankHB <frankhb1989@gmail.com>
\since build 472
\par 创建时间:
	2014-02-02 17:58:24 +0800
\par 修改时间:
	2016-10-24 02:13 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	NPL::NPLA1
*/


#ifndef NPL_INC_NPLA1_h_
#define NPL_INC_NPLA1_h_ 1

#include "YModules.h"
#include YFM_NPL_NPLA // for NPLATag, ValueNode, TermNode, LoggedEvent,
//	YSLib::Access;

namespace NPL
{

//! \since build 665
namespace A1
{

/*!
\brief NPLA1 元标签。
\note NPLA1 是 NPLA 的具体实现。
\since build 597
*/
struct YF_API NPLA1Tag : NPLATag
{};


//! \brief 值记号：节点中的值的占位符。
enum class ValueToken
{
	Null,
	/*!
	\brief 未定义值。
	\since build 732
	*/
	Undefined,
	/*!
	\brief 未指定值。
	\since build 732
	*/
	Unspecified,
	GroupingAnchor,
	OrderedAnchor
};


//! \since build 674
//@{
//! \brief 插入 NPLA1 子节点。
//@{
/*!
\note 插入后按名称排序顺序。

第一参数指定的变换结果插入第二参数指定的容器。
若映射操作返回节点名称为空则根据当前容器内子节点数量加前缀 $ 命名以避免重复。
*/
YF_API void
InsertChild(TermNode&&, TermNode::Container&);

/*!
\note 保持顺序。

直接插入 NPLA1 子节点到序列容器末尾。
*/
YF_API void
InsertSequenceChild(TermNode&&, NodeSequence&);
//@}

/*!
\brief 变换 NPLA1 节点 S 表达式抽象语法树为 NPLA1 语义结构。
\exception std::bad_function_call 第三至五参数为空。
\return 变换后的新节点（及子节点）。

第一参数指定源节点，其余参数指定部分变换规则。
当被调用的第二至第四参数不修改传入的节点参数时变换不修改源节点。
过程如下：
若源节点为叶节点，直接返回使用第三参数创建映射的节点。
若源节点只有一个子节点，直接返回这个子节点的变换结果。
否则，使用第四参数从第一个子节点取作为变换结果名称的字符串。
	若名称非空则忽略第一个子节点，只变换剩余子节点。
		当剩余一个子节点（即源节点有两个子节点）时，直接递归变换这个节点并返回。
		若变换后的结果名称非空，则作为结果的值；否则，结果作为容器内单一的值。
	否则，新建节点容器，遍历并变换剩余的节点插入这个容器，返回以这个容器构造的节点。
		第二参数指定此时的映射操作，若为空则默认使用递归 TransformNode 调用。
		调用第五参数插入映射的结果到容器。
*/
YF_API ValueNode
TransformNode(const TermNode&, NodeMapper = {}, NodeMapper = MapNPLALeafNode,
	NodeToString = ParseNPLANodeString, NodeInserter = InsertChild);

/*!
\brief 变换 NPLA1 节点 S 表达式抽象语法树为 NPLA1 序列语义结构。
\exception std::bad_function_call 第三至五参数为空。
\return 变换后的新节点（及子节点）。
\sa TransformNode

和 TransformNode 变换规则相同，
但插入的子节点以 NodeSequence 的形式作为变换节点的值而不是子节点，可保持顺序。
*/
YF_API ValueNode
TransformNodeSequence(const TermNode&, NodeMapper = {},
	NodeMapper = MapNPLALeafNode, NodeToString = ParseNPLANodeString,
	NodeSequenceInserter = InsertSequenceChild);
//@}


/*!
\brief 加载 NPLA1 翻译单元。
\throw LoggedEvent 警告：被加载配置中的实体转换失败。
*/
//@{
template<typename _type, typename... _tParams>
ValueNode
LoadNode(_type&& tree, _tParams&&... args)
{
	TryRet(A1::TransformNode(std::forward<TermNode&&>(tree),
		yforward(args)...))
	CatchThrow(ystdex::bad_any_cast& e, LoggedEvent(YSLib::sfmt(
		"Bad NPLA1 tree found: cast failed from [%s] to [%s] .", e.from(),
		e.to()), YSLib::Warning))
}

template<typename _type, typename... _tParams>
ValueNode
LoadNodeSequence(_type&& tree, _tParams&&... args)
{
	TryRet(A1::TransformNodeSequence(std::forward<TermNode&&>(tree),
		yforward(args)...))
	CatchThrow(ystdex::bad_any_cast& e, LoggedEvent(YSLib::sfmt(
		"Bad NPLA1 tree found: cast failed from [%s] to [%s] .", e.from(),
		e.to()), YSLib::Warning))
}
//@}


//! \since build 685
//@{
//! \brief 访问守护遍。
YF_API GuardPasses&
AccessGuardPassesRef(ContextNode&);

//! \brief 访问叶节点遍。
YF_API EvaluationPasses&
AccessLeafPassesRef(ContextNode&);

//! \brief 访问列表节点遍。
YF_API EvaluationPasses&
AccessListPassesRef(ContextNode&);
//@}

//! \sa InvokePasses
//@{
/*!
\brief 调用守护遍。
\sa GuardPasses
\since build 726
*/
YF_API Guard
InvokeGuard(TermNode& term, ContextNode&);

/*!
\sa EvaluationPasses
\since build 730
*/
//@{
//! \brief 调用叶节点遍。
YF_API ReductionStatus
InvokeLeaf(TermNode& term, ContextNode&);

//! \brief 调用列表节点遍。
YF_API ReductionStatus
InvokeList(TermNode& term, ContextNode&);
//@}
//@}


/*!
\brief NPLA1 表达式节点规约：调用至少一次求值例程规约子表达式。
\return 规约状态。
\note 可能使参数中容器的迭代器失效。
\note 默认不需要重规约。这可被求值遍改变。
\note 可被求值遍调用以实现递归求值。
\sa DetectReducible
\sa InvokeGuard
\sa InvokeLeaf
\sa InvokeList
\sa ValueToken
\since build 730
\todo 实现 ValueToken 保留处理。

规约顺序如下：
调用 InvokeGuard 进行必要的上下文重置；
迭代规约，直至不需要进行重规约。
对应不同的节点次级结构分类，一次迭代按以下顺序判断选择以下分支之一，按需规约子项：
对枝节点调用 InvokeList 求值；
对空节点替换为 ValueToken::Null ；
对已替换为 ValueToken 的叶节点保留处理；
对其它叶节点调用 InvokeLeaf 求值。
单一求值的结果作为 DetectReducible 的第二参数，根据结果判断是否进行重规约。
此处约定的迭代中对节点的具体结构分类默认也适用于其它 NPLA1 实现 API ；
例外情况应单独指定明确的顺序。
例外情况包括输入节点不是表达式语义结构（而是抽象语法树）的 API ，如 TransformNode 。
*/
YF_API ReductionStatus
Reduce(TermNode&, ContextNode&);

/*!
\note 按语言规范，子项规约顺序未指定。
\note 可能使参数中容器的迭代器失效。
\sa Reduce
*/
//@{
//! \note 忽略子项重规约要求。
//@{
/*!
\brief 对容器中的第二项开始逐项规约。
\throw InvalidSyntax 容器内的子项数不大于 1 。
\sa ReduceChildren
\since build 685
*/
YF_API void
ReduceArguments(TermNode::Container&, ContextNode&);

/*!
\since build 735
\todo 使用更确切的异常类型。
*/
//@{
/*!
\brief 规约并检查成功：调用 Reduce 并检查结果，失败时抛出异常。
\throw NPLException Reduce 结果不是 ReductionStatus::Success。
\sa CheckedReduceWith
\sa Reduce
*/
YF_API void
ReduceChecked(TermNode&, ContextNode&);

/*!
\brief 规约闭包：使用第四参数指定的闭包项规约后替换到指定项上。
\exception NPLException 异常中立：由 ReduceChecked 抛出。
\note 第三参数指定是否转移而不保留原项。
\sa ReduceChecked
*/
YF_API void
ReduceCheckedClosure(TermNode&, ContextNode&, bool, TermNode&);
//@}

/*!
\brief 规约子项。
\since build 697
*/
//@{
YF_API void
ReduceChildren(TNIter, TNIter, ContextNode&);
inline PDefH(void, ReduceChildren, TermNode::Container& con, ContextNode& ctx)
	ImplExpr(ReduceChildren(con.begin(), con.end(), ctx))
inline PDefH(void, ReduceChildren, TermNode& term, ContextNode& ctx)
	ImplExpr(ReduceChildren(term.GetContainerRef(), ctx))
//@}
//@}

/*!
\brief 移除容器首项到指定迭代器的项后规约。
\since build 733
*/
YF_API ReductionStatus
ReduceTail(TermNode&, ContextNode&, TNIter);
//@}

/*!
\brief 规约首项。
\return 规约状态。
\sa Reduce
\see https://en.wikipedia.org/wiki/Fexpr 。
\since build 730

快速严格性分析：无条件求值枝节点第一项以避免非确定性推断子表达式求值的附加复杂度。
*/
inline PDefH(ReductionStatus, ReduceFirst, TermNode& term, ContextNode& ctx)
	ImplRet(IsBranch(term) ? Reduce(Deref(term.begin()), ctx)
		: ReductionStatus::Success)


/*!
\brief 设置跟踪深度节点：调用规约时显示深度和上下文等信息。
\note 主要用于调试。
\sa InvokeGuard
\since build 685
*/
YF_API void
SetupTraceDepth(ContextNode& ctx, const string& name = yimpl("$__depth"));


/*!
\note ValueObject 参数分别指定替换添加的前缀和被替换的分隔符的值。
\since build 697
*/
//@{
/*!
\note 移除子项中值和指定分隔符指定的项，并以 AsIndexNode 添加指定前缀值作为子项。
\note 最后一个参数指定返回值的名称。
\sa AsIndexNode
*/
//@{
//! \brief 变换分隔符中缀表达式为前缀表达式。
YF_API TermNode
TransformForSeparator(const TermNode&, const ValueObject&, const ValueObject&,
	const string& = {});

//! \brief 递归变换分隔符中缀表达式为前缀表达式。
YF_API TermNode
TransformForSeparatorRecursive(const TermNode&, const ValueObject&,
	const ValueObject&, const string& = {});
//@}

/*!
\brief 查找项中的指定分隔符，若找到则替换项为去除分隔符并添加替换前缀的形式。
\return 是否找到并替换了项。
\sa EvaluationPasses
\sa TransformForSeparator
\since build 730
*/
YF_API ReductionStatus
ReplaceSeparatedChildren(TermNode&, const ValueObject&, const ValueObject&);
//@}


/*!
\brief 形式上下文处理器。
\since build 674
*/
class YF_API FormContextHandler
{
public:
	ContextHandler Handler;
	/*!
	\brief 项检查例程：验证被包装的处理器的调用符合前置条件。
	\since build 733
	*/
	std::function<bool(const TermNode&)> Check{};

	//! \since build 697
	template<typename _func,
		yimpl(typename = ystdex::exclude_self_t<FormContextHandler, _func>)>
	FormContextHandler(_func&& f)
		: Handler(yforward(f))
	{}
	//! \since build 733
	template<typename _func, typename _fCheck>
	FormContextHandler(_func&& f, _fCheck c)
		: Handler(yforward(f)), Check(c)
	{}

	/*!
	\brief 处理一般形式。
	\exception NPLException 异常中立。
	\throw LoggedEvent 警告：类型不匹配，
		由 Handler 抛出的 ystdex::bad_any_cast 转换。
	\throw LoggedEvent 错误：由 Handler 抛出的 ystdex::bad_any_cast 外的
		std::exception 转换。
	\throw std::invalid_argument 项检查未通过。

	项检查不存在或在检查通过后，对节点调用 Hanlder ，否则抛出异常。
	*/
	void
	operator()(TermNode&, ContextNode&) const;
};


/*!
\brief 函数上下文处理器。
\since build 696
*/
class YF_API FunctionContextHandler
{
public:
	FormContextHandler Handler;

	//! \since build 697
	template<typename _func,
		yimpl(typename = ystdex::exclude_self_t<FunctionContextHandler, _func>)>
	FunctionContextHandler(_func&& f)
		: Handler(yforward(f))
	{}
	//! \since build 733
	template<typename _func, typename _fCheck>
	FunctionContextHandler(_func&& f, _fCheck c)
		: Handler(yforward(f), c)
	{}

	/*!
	\brief 处理函数。
	\throw ListReductionFailure 列表子项不大于一项。
	\sa ReduceArguments

	对每一个子项求值；然后检查项数，对可调用的项调用 Hanlder ，否则抛出异常。
	*/
	void
	operator()(TermNode&, ContextNode&) const;
};


//! \since build 733
//@{
template<typename... _tParams>
inline void
RegisterFormContextHandler(ContextNode& node, const string& name,
	_tParams&&... args)
{
	NPL::RegisterContextHandler(node, name,
		FormContextHandler(yforward(args)...));
}

//! \brief 转换上下文处理器。
template<typename... _tParams>
inline ContextHandler
ToContextHandler(_tParams&&... args)
{
	return FunctionContextHandler(yforward(args)...);
}

/*!
\brief 注册函数上下文处理器。
\note 使用 ADL ToContextHandler 。
*/
template<typename... _tParams>
inline void
RegisterFunction(ContextNode& node, const string& name, _tParams&&... args)
{
	NPL::RegisterContextHandler(node, name,
		ToContextHandler(yforward(args)...));
}
//@}

/*!
\brief 注册分隔符转换变换和处理例程。
\sa NPL::RegisterContextHandler
\sa ReplaceSeparatedChildren
\since build 726
*/
YF_API void
RegisterSequenceContextTransformer(EvaluationPasses&, ContextNode&,
	const string&, const ValueObject&);


/*!
\return 规约状态。
\since build 730
*/
//@{
/*!
\brief 检查项的首项并尝试按上下文列表求值。
\throw ListReductionFailure 规约失败：找不到可规约项。
\note 若项不是枝节点则视为规约成功，没有其它作用。
\sa ContextHandler
\sa Reduce
*/
YF_API ReductionStatus
EvaluateContextFirst(TermNode&, ContextNode&);

/*!
\brief 求值标识符。
\pre 断言：第三参数的数据指针非空。
\throw BadIdentifier 标识符未声明。
\note 第一参数指定输出的值。
\note 不验证标识符是否为字面量；仅以字面量处理时可能需要重规约。
\sa FetchValue
\sa LiteralHandler
\since build 735

依次进行以下求值操作：
调用 FetchValue 查找值，若失败抛出未声明异常；
以 LiteralHandler 访问字面量处理器，若成功调用并返回字面量处理器的处理结果；
否则，以 TermNode 访问值，若发现是枝节点，返回要求重规约；
否则，视为规约成功。
*/
YF_API ReductionStatus
EvaluateIdentifier(TermNode&, ContextNode&, string_view);
//@}


/*!
\brief NPLA1 语法形式对应的功能实现。
\since build 732
*/
namespace Forms
{

/*!
\pre 断言：项或容器对应枝节点。
\since build 733
*/
//@{
//! \brief 引用项：延迟求值。
inline PDefH(void, Quote, const TermNode& term) ynothrowv
	ImplExpr(YAssert(IsBranch(term), "Invalid term found."))

/*!
\return 项的参数个数。

可使用 RegisterFormContextHandler 注册上下文处理器，参考文法：
$quote|$quoteN <expression>
*/
//@{
//! \brief 取项的参数个数：子项数减 1 。
inline PDefH(size_t, FetchArgumentN, const TermNode& term) ynothrowv
	ImplRet(Quote(term), term.size() - 1)

/*!
\brief 引用经检查确保具有指定个数参数的项：延迟求值。
\throw ArityMismatch 项的参数个数不等于第二参数。
*/
YF_API size_t
QuoteN(const TermNode&, size_t = 1);
//@}


/*!
\brief 检查项中是否存在为修饰符的第二个子项，若存在则移除。
\return 是否存在并移除了修饰符。
\since build 732

检查第一参数指定的容器或项是否存在第二参数指定的修饰符为项的第一参数，若存在则移除。
*/
//@{
YF_API bool
ExtractModifier(TermNode::Container&, const ValueObject& = string("!"));
inline PDefH(bool, ExtractModifier, TermNode& term,
	const ValueObject& mod = string("!"))
	ImplRet(ExtractModifier(term.GetContainerRef(), mod))
//@}

//! \brief 规约可能带有修饰符的项。
template<typename _func>
void
ReduceWithModifier(TermNode& term, ContextNode& ctx, _func f)
{
	const bool mod(ExtractModifier(term));

	if(IsBranch(term))
		f(term, ctx, mod);
	else
		throw InvalidSyntax("Argument not found.");
}
//@}


/*!
\brief 访问节点并调用一元函数。
\since build 734
*/
//@{
template<typename _func>
void
CallUnary(_func f, TermNode& term)
{
	QuoteN(term);

	f(Deref(std::next(term.begin())));
	term.ClearContainer();
}

template<typename _type, typename _func>
void
CallUnaryAs(_func f, TermNode& term)
{
	return Forms::CallUnary([f](TermNode& node){
		f(YSLib::Access<_type>(node));
	}, term);
}
//@}


//! \since build 735
//@{
/*!
\note 支持修饰符。
\sa ReduceWithModifier
*/
//@{
/*!
\brief 定义或设置项。
\throw InvalidSyntax 节点分类非法，或由 DefineOrSetFor 抛出的异常。
\note 第三参数指定使用定义而不是设置（重定义）。
\sa DefineOrSetFor
\sa Lambda

限定第三参数后可使用 RegisterFormContextHandler 注册上下文处理器，参考文法：
$define|$set [!] <variable> <expression>
$define|$set [!] (<variable> <formals>) <body>
*/
YF_API void
DefineOrSet(TermNode&, ContextNode&, bool);

/*!
\brief 定义或设置标识符的值。
\note 参数分别为标识符、被规约的项、上下文、使用定义以及是否存在修饰符。
\throw InvalidSyntax 标识符是字面量。
\sa CategorizeLiteral
\sa DefineValue
\sa RedefineValue

定义或设置参数指定的值：首先检查标识符不是字面量。
排除可选项外，若第二子项是列表，
则定义或设置以此列表第一子项为名称、剩余项为参数的 λ 抽象。
*/
YF_API void
DefineOrSetFor(const string&, TermNode&, ContextNode&, bool, bool);
//@}

/*!
\brief 以容器作为参数列表提取 λ 抽象的参数。
\throw InvalidSyntax 存在重复的参数。
*/
YF_API YSLib::shared_ptr<vector<string>>
ExtractLambdaParameters(const TermNode::Container&);

/*!
\brief λ 抽象：产生一个捕获当前上下文的过程。
\exception InvalidSyntax 异常中立：由 ExtractLambdaParameters 抛出。
\sa ExtractLambdaParameters
\todo 优化捕获开销。

使用 ExtractLambdaParameters 检查参数列表并捕获和绑定变量，
然后设置节点的值为表示 λ 抽象的上下文处理器。
可使用 RegisterFormContextHandler 注册上下文处理器，参考文法：
$lambda <formals> <body>
*/
YF_API void
Lambda(TermNode&, ContextNode&);
//@}

} // namespace Forms;

} // namesapce A1;

} // namespace NPL;

#endif

