﻿/*
	© 2013-2016, 2018-2021 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file Loader.cpp
\ingroup UI
\brief 动态 GUI 加载。
\version r383
\author FrankHB <frankhb1989@gmail.com>
\since build 433
\par 创建时间:
	2013-08-01 20:39:49 +0800
\par 修改时间:
	2021-12-24 23:56 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::Loader
*/


#include "YSLib/UI/YModules.h"
#include "NPL/YModules.h"
#include YFM_YSLib_UI_Loader // for to_std_string, istringtream;
#include <sstream> // for complete istringtream;
#include YFM_YSLib_UI_YPanel
#include YFM_NPL_Configuration // for NPL, Session, A1::LoadNode;

namespace YSLib
{

namespace UI
{

Rect
ParseRect(const string& str)
{
	Rect res;

	try
	{
		istringstream iss(str);

		iss.exceptions(std::ios::failbit | std::ios::badbit);
		iss >> res.X >> res.Y >> res.Width >> res.Height;
	}
	catch(std::exception& e)
	{
		YTraceDe(Warning, "Error: %s", e.what());
		throw std::invalid_argument("Parse 'Rect' failed: bad state.");
	}
	YTraceDe(Debug, "ParseRect: %s.", to_string(res).c_str());
	return res;
}


ImplDeDtor(WidgetNotFound)


IWidget&
AccessWidget(const ValueNode& node)
{
	if(const auto p = AccessNodePtr(node, "$pointer"))
		// NOTE: It may still throw %bad_any_cast.
		return Deref(Access<shared_ptr<IWidget>>(*p));
	throw WidgetNotFound(node.GetName(), "Widget pointer not found.");
}


unique_ptr<IWidget>
WidgetLoader::DetectWidgetNode(const ValueNode& node) const
{
	const auto& child(AccessNode(node, "$type"));

	if(const auto p_type_str = AccessPtr<string>(child))
	{
		if(const auto p_bounds_str = AccessChildPtr<string>(node, "$bounds"))
			try
			{
				const Rect& bounds(ParseRect(*p_bounds_str));

				return Bounds.Invoke(*p_type_str, bounds);
			}
			CatchIgnore(std::invalid_argument&)
		return Default.Invoke(*p_type_str);
	}
	return {};
}

ValueNode
WidgetLoader::LoadUILayout(string_view sv) const
{
	using namespace NPL;
	Session sess{};

	return TransformUILayout(
		A1::LoadNode(SContext::Analyze(sess, sess.Process(sv))));
}

ValueNode
WidgetLoader::TransformUILayout(const ValueNode& node) const
{
	if(unique_ptr<IWidget> p_new_widget{DetectWidgetNode(node)})
	{
		ValueNode res(AsNode(node.GetName()));
		const auto& key(AccessChild<string>(node, "$type"));
		const bool ins(Insert.Contains(key));
		const bool insz(InsertZOrdered.Contains(key));

		if(ins || insz)
		{
			ValueNode::Container node_con;

			for(const auto& vn : node)
				if(CheckChildName(vn.GetName()))
					if(ValueNode child{TransformUILayout(vn)})
					{
						const auto& wgt_ptr_nd(AccessNode(child, "$pointer"));

						if(const auto p_wgt_ptr
							= AccessPtr<shared_ptr<IWidget>>(wgt_ptr_nd))
						{
							const auto& wgt_ptr(Deref(p_wgt_ptr));

							const auto p_z(AccessChildPtr<string>(vn, "$z"));
							auto z(DefaultZOrder);

							if(insz && p_z)
								try
								{
									const auto
										r(std::stoul(to_std_string(*p_z)));

									// TODO: Do not use magic number.
									if(r < 0x100)
										z = r;
								}
								CatchIgnore(std::invalid_argument&)
							if(node_con.insert(std::move(child)).second)
							{
								auto& wgt(*wgt_ptr);

								if(insz && (p_z || !ins))
									InsertZOrdered.Invoke(key, *p_new_widget,
										wgt, z);
								else
									Insert.Invoke(key, *p_new_widget, wgt);
							}
						}
					}
			res += {std::move(node_con), "$children"};
		}
		res += AsNode("$pointer", shared_ptr<IWidget>(std::move(p_new_widget)));
		return res;
	}
	return {};
}

} // namespace UI;

} // namespace YSLib;

