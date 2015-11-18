﻿/*
	© 2009-2015 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file YCommon.h
\ingroup YCLib
\brief 平台相关的公共组件无关函数与宏定义集合。
\version r3663
\author FrankHB <frankhb1989@gmail.com>
\since build 561
\par 创建时间:
	2009-11-12 22:14:28 +0800
\par 修改时间:
	2015-11-18 11:35 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YCLib::YCommon
*/


#ifndef YCL_INC_ycommon_h_
#define YCL_INC_ycommon_h_ 1

#include "YModules.h"
#include YFM_YCLib_Platform
#include <ystdex/functional.hpp> // for ystdex::decay_t, ystdex::result_of_t,
//	ystdex::retry_on_cond;
#include <ystdex/cassert.h> // yconstraint, yassume for other headers;
#include <ystdex/cwctype.h> // for ystdex::isprint, ystdex::iswprint;
#include <ystdex/cstring.h> // for ystdex::uchar_t, ystdex::replace_cast;
#include YFM_YBaseMacro

//! \brief 默认平台命名空间。
namespace platform
{

/*!
\brief 异常终止函数。
\since build 319
*/
YB_NORETURN YF_API void
terminate() ynothrow;


/*!
\brief 声明检查存在合式调用的辅助宏。
\sa ystdex::is_detected
\since build 651
*/
//@{
#define YCL_CheckDecl_t(_fn) CheckDecl##_fn##_t
#define YCL_DeclCheck_t(_fn, _call) \
	template<typename... _tParams> \
	using YCL_CheckDecl_t(_fn) \
		= decltype(_call(std::declval<_tParams&&>()...));
//@}


/*!
\brief 平台描述空间。
\since build 456
*/
namespace Descriptions
{

/*!
\brief 记录等级。
\since build 432
*/
enum RecordLevel : std::uint8_t
{
	Emergent = 0x00,
	Alert = 0x20,
	Critical = 0x40,
	Err = 0x60,
	Warning = 0x80,
	Notice = 0xA0,
	Informative = 0xC0,
	Debug = 0xE0
};

} // namespace Descriptions;


/*!
\brief 检查默认区域下指定字符是否为可打印字符。
\note MSVCRT 的 isprint/iswprint 实现缺陷的变通。
\sa https://connect.microsoft.com/VisualStudio/feedback/details/799287/isprint-incorrectly-classifies-t-as-printable-in-c-locale
\since build 512
*/
//@{
inline PDefH(bool, IsPrint, char c)
	ImplRet(ystdex::isprint(c))
inline PDefH(bool, IsPrint, wchar_t c)
	ImplRet(ystdex::iswprint(c))
template<typename _tChar>
bool
IsPrint(_tChar c)
{
	return platform::IsPrint(wchar_t(c));
}
//@}


//! \since build 631
//@{
inline PDefH(ystdex::uchar_t*, ucast, wchar_t* p) ynothrow
	ImplRet(ystdex::replace_cast<ystdex::uchar_t*>(p))
inline PDefH(const ystdex::uchar_t*, ucast, const wchar_t* p) ynothrow
	ImplRet(ystdex::replace_cast<const ystdex::uchar_t*>(p))
template<typename _tChar>
_tChar*
ucast(_tChar* p) ynothrow
{
	return p;
}

inline PDefH(wchar_t*, wcast, ystdex::uchar_t* p) ynothrow
	ImplRet(ystdex::replace_cast<wchar_t*>(p))
inline PDefH(const wchar_t*, wcast, const ystdex::uchar_t* p) ynothrow
	ImplRet(ystdex::replace_cast<const wchar_t*>(p))
template<typename _tChar>
_tChar*
wcast(_tChar* p) ynothrow
{
	return p;
}
//@}


/*!
\brief 循环重复操作。
\since build 625
*/
template<typename _func, typename _tErrorRef,
	typename _tError = ystdex::decay_t<_tErrorRef>,
	typename _type = ystdex::result_of_t<_func&()>>
_type
RetryOnError(_func f, _tErrorRef&& err, _tError e = _tError())
{
	return ystdex::retry_on_cond([&](_type res){
		return res < _type() && _tError(err) == e;
	}, f);
}


/*!
\brief 执行 UTF-8 字符串的环境命令。
\note 使用 std::system 实现；若参数为空则和 std::system 行为一致。
\since build 539
*/
YF_API int
usystem(const char*);

} // namespace platform;

/*!
\brief 平台扩展命名空间。
\note 为便于移植，部分平台扩展接口保留至平台中立文件，依赖性按后者处理。
*/
namespace platform_ex
{

#if YCL_DS
/*!
\brief 设置允许设备进入睡眠的标识状态。
\return 旧状态。
\note 默认状态为 true 。
\warning 非线程安全。
\since build 278
*/
YF_API bool
AllowSleep(bool);
#endif

} // namespace platform_ex;

#endif

