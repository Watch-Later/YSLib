﻿/*
	Copyright (C) by Franksoft 2011.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file HexBrowser.h
\ingroup YReader
\brief 十六进制浏览器。
\version r1186;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2011-10-14 18:13:04 +0800;
\par 修改时间:
	2011-10-28 13:55 +0800;
\par 字符集:
	UTF-8;
\par 模块名称:
	YReader::HexBrowser;
*/


#ifndef INCLUDED_HEXVIEWER_H_
#define INCLUDED_HEXVIEWER_H_

#include <YSLib/UI/scroll.h>
#include <YSLib/Service/yfile.h>
#include <YSLib/Service/ytext.h>

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Components)

/*!
\brief 十六进制视图区域。
*/
class HexViewArea : public ScrollableContainer
{
public:
	static yconstexpr size_t ItemPerLine = 8; //!< 每行数据总数（字节）。

	typedef std::uintptr_t IndexType; //!< 索引类型。
	typedef array<byte, ItemPerLine> LineType; //!< 行数据类型。
	typedef list<LineType> ListType; //!< 列表数据类型。

private:
	Drawing::TextState text_state; //!< 文本状态。
	u16 item_num; //!< 行数。
	ListType lines; //!< 行数据。
	File source; //!< 文件数据源。

public:
	explicit
	HexViewArea(const Drawing::Rect& = Drawing::Rect::Empty,
		FontCache& = FetchGlobalInstance().GetFontCache());

	/*!
	\brief 取项目行高。
	*/
	SDst
	GetItemHeight() const;
	/*!
	\brief 取包含指定点且被指定谓词过滤的顶端部件指针。
	*/
	virtual IWidget*
	GetTopWidgetPtr(const Point&, bool(&)(const IWidget&));
	DefGetter(const File&, source, source)

	void
	Load(const_path_t);

	/*!
	\brief 定位视图顶端至指定竖直位置。
	*/
	void
	LocateViewPosition(SDst);

	virtual Rect
	Refresh(const PaintContext&);

	void
	Reset();

	void
	UpdateData(u32);
};

inline SDst
HexViewArea::GetItemHeight() const
{
	return GetTextLineHeightExOf(text_state);
}

YSL_END_NAMESPACE(Components)

YSL_END

#endif
