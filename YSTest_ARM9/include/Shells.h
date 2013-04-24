﻿/*
	Copyright by FrankHB 2010 - 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\defgroup YReader YReader
\brief YReader 阅读器。
*/

/*!	\file Shells.h
\ingroup YReader
\brief Shell 框架逻辑。
\version r2895
\author FrankHB <frankhb1989@gmail.com>
\since 早于 build 132
\par 创建时间:
	2010-03-06 21:38:16 +0800
\par 修改时间:
	2013-04-20 18:13 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YReader::Shells
*/


#ifndef INC_YReader_Shells_h_
#define INC_YReader_Shells_h_ 1

#include <Helper/shlds.h>
#include <Helper/ShellHelper.h>
#include <YCLib/Debug.h>

YSL_BEGIN

typedef decltype(__LINE__) ResourceIndex;
typedef map<ResourceIndex, ValueObject> ResourceMap;

#define DeclResource(_name) const ResourceIndex _name(__LINE__);

YSL_END


YSL_BEGIN_NAMESPACE(YReader)

#define G_COMP_NAME u"Franksoft"
#define G_APP_NAME u"YReader"
#define G_APP_VER u"Build PreAlpha"

using namespace YSLib;

using namespace UI;
using namespace Drawing;

using namespace DS;
using namespace DS::UI;

using platform::WaitForInput;
using platform::YDebugSetStatus;


/*!
\brief 全局资源清理。
\since build 400
*/
void
Cleanup();

shared_ptr<Image>&
FetchImage(size_t);

//! \since build 390
double
FetchImageLoadTime();

YSL_END_NAMESPACE(YReader)

#endif

