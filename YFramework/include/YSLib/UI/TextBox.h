﻿/*
	© 2014 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file TextBox.h
\ingroup UI
\brief 样式无关的用户界面文本框。
\version r91
\author FrankHB <frankhb1989@gmail.com>
\since build 482
\par 创建时间:
	2014-03-02 16:17:46 +0800
\par 修改时间:
	2014-03-03 19:32 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::TextBox
*/


#ifndef YSL_INC_UI_TextBox_h_
#define YSL_INC_UI_TextBox_h_ 1

#include "YModules.h"
#include YFM_YSLib_UI_YControl
#include YFM_YSLib_UI_Label
#include YFM_YSLib_Service_YTimer
#include YFM_YSLib_UI_Animation

namespace YSLib
{

namespace UI
{

/*!
\brief 文本框。
\sa Label
\since build 482
\todo 实现插入符光标移动、文本选中和插入。
*/
class YF_API TextBox : public Control, protected MLabel
{
public:
	using MLabel::Font;
	using MLabel::Margin;
	using MLabel::HorizontalAlignment;
	using MLabel::VerticalAlignment;
	using MLabel::AutoWrapLine;
	using MLabel::Text;

	//! \brief 插入符横向位置。
	size_t XOffset = 0;
	//! \brief 插入符纵向位置。
	size_t YOffset = 0;
	//! \brief 插入符画刷。
	HBrush CaretBrush;
	//! \brief 决定是否显示插入符的计时器。
	Timers::Timer CaretTimer{std::chrono::seconds(1)};

private:
	//! brief 插入符闪烁动画。
	GAnimationSession<InvalidationUpdater> caret_animation;

public:
	//! \brief 构造：使用指定边界和字体。
	explicit
	TextBox(const Rect& = {}, const Drawing::Font& = {});
	DefDeMoveCtor(TextBox)

	//! \brief 刷新：按指定参数绘制界面并更新状态。
	void
	Refresh(PaintEventArgs&&) override;

	//! \brief 绘制默认插入符。
	void
	PaintDefaultCaret(PaintEventArgs&&);
};

} // namespace UI;

} // namespace YSLib;

#endif
