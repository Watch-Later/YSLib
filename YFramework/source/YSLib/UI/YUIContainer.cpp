﻿/*
	© 2011-2016, 2021 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file YUIContainer.cpp
\ingroup UI
\brief 样式无关的 GUI 容器。
\version r1945
\author FrankHB <frankhb1989@gmail.com>
\since build 188
\par 创建时间:
	2011-01-22 08:03:49 +0800
\par 修改时间:
	2021-12-24 23:45 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::YUIContainer
*/


#include "YSLib/UI/YModules.h"
#include YFM_YSLib_UI_YDesktop
#include <ystdex/functional.hpp> // for ystdex::bind1;

using namespace ystdex;

namespace YSLib
{

namespace UI
{

IWidget&
FetchTopLevel(IWidget& wgt)
{
	auto wgt_ref(ystdex::ref(wgt));

	while(const auto p = FetchContainerPtr(wgt_ref))
		wgt_ref = *p;
	return wgt_ref;
}
IWidget&
FetchTopLevel(IWidget& wgt, Point& pt)
{
	auto wgt_ref(ystdex::ref(wgt));

	while(const auto p = FetchContainerPtr(wgt_ref))
	{
		pt += GetLocationOf(wgt_ref);
		wgt_ref = *p;
	}
	return wgt_ref;
}

vector<pair<observer_ptr<const IWidget>, Point>>
FetchTrace(const IWidget& wgt)
{
	vector<pair<observer_ptr<const IWidget>, Point>> lst;
	Point pt;

	for(auto p_wgt(make_observer(&wgt)); p_wgt;
		p_wgt = FetchContainerPtr(*p_wgt))
	{
		lst.emplace_back(p_wgt, pt);
		pt += GetLocationOf(*p_wgt);
	}
	return lst;
}


Point
LocateOffset(observer_ptr<const IWidget> p_end, Point pt,
	observer_ptr<const IWidget> p_wgt)
{
	while(p_wgt && p_wgt != p_end)
	{
		pt += GetLocationOf(*p_wgt);
		p_wgt = FetchContainerPtr(*p_wgt);
	}
	return pt;
}

Point
LocateForTopOffset(const Point& origin, IWidget& wgt, const Point& pt)
{
	auto loc(origin);

	FetchTopLevel(wgt, loc);
	return loc + pt;
}

Point
LocateForTrace(const vector<pair<observer_ptr<const IWidget>, Point>>& lst,
	const IWidget& wgt)
{
	Point pt;

	for(auto p_wgt(make_observer(&wgt)); p_wgt;
		p_wgt = FetchContainerPtr(*p_wgt))
	{
		const auto
			i(std::find(lst.cbegin() | get_key, lst.cend() | get_key, p_wgt));

		if(i != lst.cend())
			return pt - i.get()->second;
		pt += GetLocationOf(*p_wgt);
	}
	return Point::Invalid;
}

Point
LocateForParentContainer(const IWidget& wgt)
{
	return FetchContainerPtr(wgt)
		? LocateContainerOffset(*FetchContainerPtr(wgt), GetLocationOf(wgt))
		: Point::Invalid;
}


void
MoveToBottom(IWidget& wgt)
{
	SetLocationOf(wgt, Point(GetLocationOf(wgt).X, GetSizeOf(
		Deref(FetchContainerPtr(wgt))).Height - GetSizeOf(wgt).Height));
}

void
MoveToCenter(IWidget& wgt)
{
	SetLocationOf(wgt,
		LocateCenter(GetSizeOf(Deref(FetchContainerPtr(wgt))), GetSizeOf(wgt)));
}

void
MoveToLeft(IWidget& wgt)
{
	YAssertNonnull(FetchContainerPtr(wgt));
	SetLocationOf(wgt, Point(0, GetLocationOf(wgt).Y));
}

void
MoveToRight(IWidget& wgt)
{
	SetLocationOf(wgt, Point(GetSizeOf(Deref(FetchContainerPtr(wgt))).Width
		- GetSizeOf(wgt).Width, GetLocationOf(wgt).Y));
}

void
MoveToTop(IWidget& wgt)
{
	YAssertNonnull(FetchContainerPtr(wgt));
	SetLocationOf(wgt, Point(GetLocationOf(wgt).X, 0));
}


bool
RemoveFrom(IWidget& wgt, IWidget& con)
{
	if(FetchContainerPtr(wgt).get() == &con)
	{
		SetContainerPtrOf(wgt);
		if(FetchFocusingPtr(con).get() == &wgt)
			con.GetView().FocusingPtr = {};
		return true;
	}
	return {};
}


void
MLinearUIContainer::operator+=(IWidget& wgt)
{
	if(!Contains(wgt))
		vWidgets.push_back(ystdex::ref(wgt));
}

bool
MLinearUIContainer::operator-=(IWidget& wgt)
{
	auto t(vWidgets.size());

	erase_all_if(vWidgets, bind1(is_equal(), std::ref(wgt)));
	t -= vWidgets.size();
	YAssert(t <= 1, "Duplicate widget references found.");
	return t != 0;
}

bool
MLinearUIContainer::Contains(IWidget& wgt) const
{
	return std::count_if(vWidgets.cbegin(), vWidgets.cend(),
		bind1(is_equal(), std::ref(wgt))) != 0;
}

size_t
MLinearUIContainer::Find(IWidget& wgt) const
{
	return size_t(std::find_if(vWidgets.cbegin(), vWidgets.cend(),
		bind1(is_equal(), std::ref(wgt))) - vWidgets.cbegin());
}

MLinearUIContainer::iterator
MLinearUIContainer::begin()
{
	return vWidgets.begin() | get_get;
}

MLinearUIContainer::iterator
MLinearUIContainer::end()
{
	return vWidgets.end() | get_get;
}


bool
MUIContainer::operator-=(IWidget& wgt)
{
	auto t(mWidgets.size());

	erase_all_if(mWidgets, mWidgets.cbegin(), mWidgets.cend(),
		[&](decltype(*mWidgets.cbegin()) pr){
		return is_equal()(pr.second, wgt);
	});
	t -= mWidgets.size();
	YAssert(t <= 1, "Duplicate widget references found.");
	return t != 0;
}

void
MUIContainer::Add(IWidget& wgt, ZOrder z)
{
	if(!Contains(wgt))
		mWidgets.emplace(z, ystdex::ref(wgt));
}

bool
MUIContainer::Contains(IWidget& wgt)
{
	return std::count_if(mWidgets.cbegin(), mWidgets.cend(),
		[&](decltype(*mWidgets.cbegin()) pr) ynothrow{
		return is_equal()(pr.second, wgt);
	}) != 0;
}

void
MUIContainer::PaintVisibleChildren(PaintEventArgs& e)
{
	std::for_each(mWidgets.cbegin() | get_value, mWidgets.cend() | get_value,
		[&](const ItemType& wgt_ref){
		PaintVisibleChildAndCommit(wgt_ref, e);
	});
}

ZOrder
MUIContainer::QueryZ(IWidget& wgt) const
{
	for(auto& pr : mWidgets)
		if(is_equal()(pr.second, wgt))
			return pr.first;
	throw std::out_of_range("Widget not found.");
}

MUIContainer::iterator
MUIContainer::begin()
{
	return mWidgets.rbegin() | get_value | get_get;
}

MUIContainer::iterator
MUIContainer::end()
{
	return mWidgets.rend() | get_value | get_get;
}

} // namespace UI;

} // namespace YSLib;

