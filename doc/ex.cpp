﻿/*
	Copyright (C) by Franksoft 2009 - 2012.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file ex.cpp
\ingroup Documentation
\brief 设计规则指定和附加说明 - 存档与临时文件。
\version r3562; *build 303 rev 51;
\author FrankHB<frankhb1989@gmail.com>
\since 早于 build 132 。
\par 创建时间:
	2009-12-02 05:14:30 +0800;
\par 修改时间:
	2012-04-23 10:54 +0800;
\par 文本编码:
	UTF-8;
\par 模块名称:
	Documentation::Designation;
*/

/*
	NOTE: this is NOT a source file. The base name of this file is named
	intentionally to activate syntax highlight and other functions in an IDE
	(specially, Visual Studio). The content is mainly about log of the
	process of development, conformed with a set of virtual set of syntax rules
	which constitutes an informal pseudo-code-based language.
	This file shall be safe of deletion when building the projects.
*/

#if 0

$META:
//$configureation_for_custom_NPL_script_parser:
$parser
(
	$eval($ignore_not_impl);
	$eval($for_each $anything $in $.iterators $do_something) unfold;
);
$parser.$ __pstate;
$parser.unfold __unfold;
$parser.$.iterators __iterators;

$match_each m;

__unfold.(m($ctor, $dtor)) $= __pstate.behavior.ignore
	$= m($true, $false);
__unfold __iterators.for_labeled_paragraph
(
	$DONE,
	$DOING,
	$NEXT_TODO,
	$LOW_PRIOR_TODO,
	$KNOWN_ISSUE,
	$RESOLVED_ENVIRONMENT_ISSUE,
	$TODO
);

$script_preprocessor_escapse:
//$Record prefix and abbrevations:
$parser.$preprocessor.$define_schema "<statement> ::= $statement_in_literal";
// $evaluating.structure;
// semi-colon is used for remarking eval-order-sensitive expressions;
, ::= non-sequenced seperater
... ::= ellipse
; ::= sequenced seperater(statement termination)
// $evaluating.content;
= ::= equivalent
+ ::= added
- ::= removed
* ::= fixed
/ ::= modified
% ::= identifier
~ ::= from
! ::= not
& ::= and
| ::= or
^ ::= used
-> ::= changed to
>> ::= moved to
=> ::= renamed to
<=> ::= swapped names
@ ::= in / belonged to
\a ::= all
\ab ::= abstract
\ac ::= access
\adtor ::= abstract destructor
\amb ::= ambiguities
\amf ::= abstract/pure virtual member function
\arg ::= arguments
\as ::= assertions
\bg ::= background
\c ::= const
\cb ::= catch blocks
\cl ::= classes
\clt ::= class templates
\conf ::= configuration
\cp ::= copied
\ctor ::= constructors
\ctort ::= constuctor templates
\cv ::= const & volatile
\d ::= derived
\dat ::= data
\de ::= default/defaulted
\def ::= definitions
\del ::= deleted/deletion
\dep ::= dependencies
\decl ::= declations
\dir ::= directories
\doc ::= documents
\dtor ::= destructors
\e ::= exceptions
\em ::= empty
\en ::= enums
\eh ::= exception handling
\err ::= errors
\es ::= exception specifications
\evt ::= events
\evh ::= event handling
\ex ::= extra
\exc ::= excluded
\ext ::= extended
\exp ::= explicit
\expr ::= expressions
\f ::= functions
\fn ::= \f
\ft ::= function templates
\fw ::= forward
\g ::= global
\gs ::= global scpoe
\h ::= headers
\i ::= inline
\impl ::= implementations
\in ::= interfaces
\init ::= initializations
\inc ::= included
\inh ::= inherited
\inv ::= invoke
\k ::= keywords
\lib ::= library
\lit ::= literals
\ln ::= lines
\loc ::= local
\m ::= members
\mac ::= macros
\mem ::= memory
\mf ::= member functions
\mft ::= member function templates
\mg ::= merged
\mo ::= member objects
\mt ::= member templates
\n ::= names
\ns ::= namespaces
\o ::= objects
\op ::= operators
\or ::= overridden
\param ::= parameters
\param.de ::= default parameters
\post ::= postfix
\pre ::= prepared
\pref ::= prefix
\proj ::= projects
\pt ::= points
\ptr ::= pointers
\q ::= qualifiers
\rem ::= remarked/comments
\ren ::= renamed
\ref ::= references
\refact ::= refactorings
\reg ::= regular
\res ::= resources
\ret ::= return
\s ::= static
\scm ::= static const member
\sf ::= non-member static functions
\simp ::= simplified
\sm ::= static member
\smf ::= static member functions
\smft ::= static member function templates
\snm ::= static non-member
\spec ::= specifications
\st ::= structs
\str ::= strings
\stt ::= struct templates
\t ::= templates
\tb ::= try blocks
\tg ::= targets
\tr ::= trivial
\tp ::= types
\u ::= units
\un ::= unnamed
\v ::= volatile
\vt ::= virtual
\val ::= values

$using:
\u YObject
(
	\cl ValueObject,
	\clt GDependency
),
\u YFileSystem
(
	\cl Path
),
\u YShell
(
	\cl Shell
),
\u YApplication
(
	\cl Log,
	\cl Application
),
\u YConsole
(
	\cl Console
),
\u YRender
(
	\cl Renderer,
	\cl BufferedRenderer
),
\h YWidgetView
(
	\cl Visual,
	\cl View
),
\h YWidgetEvent
(
	\st UIEventArgs;
	\st RoutedEventArgs;
	\st InputEventArgs;
	(
		\st KeyEventArgs,
		\st TouchEventArgs
	),
	\clt GValueEventArgs,
	(
		\st PaintContext,
		\st PaintEventArgs
	),
	typedef \en VisualEvent,
	\stt EventTypeMapping,
	\st BadEvent,
	\cl AController;
	\cl WidgetController
),
\u YWidget
(
	\in IWidget,
	\cl Widget
),
\u YUIContainer
(
	\cl MUIContainer
),
\u YControl
(
	\cl Controller,
	\cl Control
),
\u YPanel
(
	\cl Panel,
),
\u YWindow
(
	\cl Window
),
\u YStyle
(
	\cl Palette
),
\u YGUI
(
	\cl InputTimer,
	\cl GUIState
),
\u YBrush
(
	\cl SolidBrush,
	\cl ImageBrush,
	\cl BorderStyle,
	\cl BorderBrush
),
\u Label
(
	\cl MLabel,
	\cl Label,
	\cl MTextList
),
\u TextArea
(
	\cl TextArea,
	\cl BufferedTextArea
),
\u Progress
(
	\cl ProgressBar
),
\u Button
(
	\cl Thumb,
	\cl Button
),
\u UIContainerEx
(
	\cl DialogBox,
	\cl DialogPanel
),
\u Selector
(
	\cl CheckBox
),
\u TextList
(
	\cl TextList
),
\u Menu
(
	\u Menu
),
\u Scroll
(
	\cl ScrollEventArgs;
	\cl ATrack;
	\cl HorizontalTrack,
	\cl VerticalTrack;
	\cl AScrollBar;
	\cl HorizontalScrollBar,
	\cl VerticalScrollBar;
	\cl ScrollableContainer
),
\u ComboList
(
	\cl ListBox,
	\cl FileBox,
	\cl DropDownList
),
\u Form
(
	\cl Form
),
\u TextBase
(
	\cl PenStyle,
	\cl TextState
),
\u TextRenderer
(
	\cl EmptyTextRenderer,
	\clt GTextRendererBase,
	\cl TextRenderer
	\cl TextRegion
),
\u TextManager
(
	\cl TextFileBuffer
);


$DONE:
r1:
(
	/ @ \h TypeOperation $=
	(
		+ \stt variadic_sequence;
		+ \stt make_natrual_sequence
	);
	/ @ \h Utilities $=
	(
		/ \inh \h YDefinition -> \h TypeOperation,
		+ \inc \h <tuple>;
		+ \clt unseq_dispatcher @ \ns details;
		+ \ft unseq_apply @ \h Utilities
	);
	/ \ft seq_apply @ \h YFunc >> \h Utilities;
	+ using ystdex::(seq_apply, unseq_apply) @ \h YAdaptor,
	/ @ \h ShellHelper $=
	(
		/ @ \cl (ChildPainter, ContainerSetter) $=
		(
			/ \m ^ std::reference_wrapper ~ raw \ref,
			- \tr 'yconstfn' @ \ctor,
			/ \tr \impl @ \op()
		),
		/ \tr \impl @ \mf ContainerSetter::\op()
	)
);
/ \a seq_apply @ \impl \u ShlReader -> unseq_apply;

r2:
/= test 1 ^ \conf release;

r3:
/ \a make_natrual_sequence => make_natural_sequence,
(
	+ \ft (AddWidgets, AddWidgetsZ, RemoveWidgets) @ \h YUIContainer;
	/ \simp \impl @ \impl \u (ShlReader, ColorPicker, Shells) ^ \ft AddWidgets
);

r4:
/= test 2 ^ \conf release;

r5:
/ @ \h Utilities $=
(
	/ \simp \impl @ \mf unseq_dispatcher::call @ \ns ystdex::details;
	- \inc \h <tuple>
);

r6:
/ @ \h Utilities $=
(
	- typedef type @ \stt variadic_sequence
	+ \stt make_successor,
	/ \tr \impl @ \stt make_natural_sequence
);

r7:
/= test 3 ^ \conf release;

r8:
+ \i @ 3 \ft @ \h YUIContainer;
/= test 4 ^ \conf release;

r9:
* $design @ \ft (parameterize_static_object, get_init) for ambiguous overloading
	with \em \tp \arg \li $since b301;
/ \f FetchCurrentSetting @ \un \ns \mg -> \ctor ShlTextReader
	@ \impl \u ShlReader;

r10:
/ @ \impl \u ShlReader $=
(
	/ @ \un \ns $= (/ \f FetchScrollDurations \mg
		-> \f UpdateScrollDropDownList)
	/ \f FetchLastRead @ \un \ns \mg -> \ctor ShlTextReader
);

r11:
/ @ \u ShlReader $=
(
	/ \simp \impl @ \ctor ShlTextReader ^ AddWidgets @ \impl \u,
	- \dtor @ \cl ShlHexBrowser
);

r12:
/= test 5 ^ \conf release;

r13:
/ @ \impl \u ShlReader $=
(
	/ \f FetchEncodingNames @ \un \ns \mg -> \ctor SettingPanel,
	/ @ \un \ns $=
	(
		+ \en MNU_READER \inh Menu::IndexType,
		/ \a 'MR_*' -> \m @ \en MNU_READER
	),
	(
		/ \decl @ \ft SetBufferRendererAndText @ \un \ns,
		/ \impl @ \cl ReaderBox
	)
);

r14-r30:
/ \impl @ \mf ShlTextReader::OnKeyDown \impl \u ShlReader,
(
	* $design / \mac YCL_KEY(X) @ \h Shells $since @ defined YCL_DS b299;
	* key 'Y' no respond $since b299,
	* wrong font size set when responding key ('X', 'Y') and boundary font size
		reached $since b299
),
/= test 6,
/ \simp \impl @ main \f @ \impl \u ARM9_Main ^ AddWidgets;

r31:
+ \mf void Delay(const Duration&) @ \cl (Timer; InputTimer);
* %KeyDown responded more than once when the input waiting interval
	is smaller than actual delay $since b300
	$= (/\impl @ \mf ShlTextReader::OnKeyDown \impl \u ShlReader);

r32:
/= test 7 ^ \conf release;

r33:
/ @ \impl \u Shells $=
(
	/ \simp \impl @ \ctor ShlExplorer::TFormExtra;
	- \cl TestObj @ \un \ns
);

r34:
(
	/ \dir Helper @ \dir YSLib -> \dir @ \proj YFramework;
	/ \tr @ Makefile @ \proj YFramework_DS,
	/ \tr \inc \h @ \h YSLib::Build,
	/ \tr @ \proj YSTest_ARM9;
),
/ \mac @ \h YEvent;

r35:
/= test 8 ^ \conf release;

r36:
/ \simp \impl @ \ctor (ShlExplorer, ShlExplorer::TFormTest) @ \impl \u Shells
	^ (AddWidgets, AddWidgetsZ, std::tie, std::forward_as_tuple);

r37:
* slash-ended item not recognized as directory @ file box @ defined YCL_MINGW32
	$since b299 $= (\impl @ GetEntryType @ \un \ns @ \impl \u Shells),
/ \a \mac YCL_KEY* @ \h Shells >> \h Input;

r38-r40:
/ \impl @ \ctor ShlExplorer @ \impl \u Shells;

r41:
/ \simp \impl @ \ctor DialogPanel,
/ \simp \ctor SettingPanel @ \impl \u ShlReader
	^ (std::tie, std::forward_as_tuple);

r42:
/ \impl @ \ctor ShlExplorer @ \impl \u ShlExplorer;
/= test 9 ^ \conf release;

r43:
/ @ \u ShlExplorer $=
(
	/ \f RemoveGlobalTasks @ \ns YReader 
		>> \ns YSLib @ \u ShellHelper,
	- typedef ShlDS ParentType @ \cl ShlExplorer
);

r44:
* \impl @ \ctor SettingPanel $since r41;

r45-r46:
/= test 10;

r47:
+ using std::(tie, forward_as_tuple, get, ignore, tuple_cat) @ \h YContainer,
* \impl @ \ft RemoveWidgets @ \h YUIContainer;
/ \simp \impl @ \impl \u (ShlReader, Shells);

r48:
/= test 11 ^ \conf release;

r49:
/ @ \u Initialization $=
(
	+ \f InitializeEnviornment,
	/ \f (EpicFail, LibfatFail, CheckSystemFontCache)
		@ \u Initialization >> \un \ns @ \impl \u
),
/ \simp \impl @ \ctor DSApplication;

r50:
/ @ \u Initialization $=
(
	/ \f CheckSystemFontCache @ \un \ns mg -> \f InitializeSystemFontCache;
	/ \impl @ \f (InitializeSystemFontCache, InitializeEnviornment,
		CheckInstall),
	/ \f LibfatFail @ \un \ns -> \f libfatFail;
	+ ynothrow @ \f (InitializeSystemFontCache, InitializeEnviornment,
		CheckInstall)
);

r51:
/= test 12 ^ \conf release;


$DOING:

$relative_process:
2012-04-23:
-8.2d;
//Mercurial rev1-rev175: r8314;

/ ...


$NEXT_TODO:
b304-b332:
/ YReader $=
(
	/ \simp \impl @ \u (DSReader, ShlReader),
	+ bookmarks manager
),
+ dynamic character mapper loader for \u CharacterMapping;


$TODO:
b333-b384:
/ $design $low_prior robustness and cleanness $=
(
	/ noncopyable GUIState,
	* (copy, move) @ \cl Menu,
	/ access control @ \inh @ \clt deref_comp
),
+ $design $low_prior helpers $=
(
	+ noinstance base class,
	+ nonmovable base class,
),
/ services $=
(
	+ \impl @ images loading
),
/ $low_prior YReader $=
(
	+ settings manager,
	+ reading history,
	+ improved smooth scrolling with lower limit of scrolling cycle supported,
	/ improved tests and examples
),
/ @ "GUI" $=
(
	+ viewer models,
	/ fully \impl @ \cl Form,
	+ icons
),
/ project structure $=
(
	/ build command @ \a \conf proj YBase,
	+ Microsoft Windows(MinGW32) port with free hosted window size
);

b385-b768:
/ improving pedantic compatiblity $=
(
	/ \mac with no \arg
),
/ $design $low_prior robustness and cleanness $=
(
	/ @ \ns platform @ \u YCommon $=
	(
		/ \ac @ \inh touchPosition @ \cl CursorInfo,
		+ \exp \init @ \m @ \cl KeysInfo
	),
	/ \impl @ \ctor \t fixed_point#2 @ \h Rational ^ 'std::llround' ~ '::llround',
	/ \mf \vt Clone -> \amf @ \cl AController ^ g++ 4.7 later,
	+ 'yconstexpr' @ \s \m Graphics::Invalid,
	/ reconsidered 1st parameter type(without rvalue \ref) for %seq_apply
),
/ completeness of core abstraction $=
(
	+ shell session;
	+ UI scenes,
	+ UI modes,
	+ UI subsessions
),
/ services $=
(
	+ general resouce management,
	/ @ "GDI" $=
	(
		+ basic animation support,
		+ more GDI algorithms
	),
	/ fully \impl @ encoding checking
),
/ debugging $=
(
	/ more APIs,
	+ namespaces
),
/ $low_prior tests and examples $=
(
	+ overlapping test @ \cl Rect,
	+ partial invalidation support @ %(HexViewArea::Refresh)
),
/ @ "GUI" $=
(
	+ generic timers multiplexing for input holding events,
		// To resolve routed events repeating preemption.
	+ formal abstraction of rectangular hit test,
	+ key accelerators,
	+ widgets for RTC,
	/ GUI brushes $=
	(
		+ more base class templates,
		+ transformations
	),
	/ long list test @ %DropDownList,
	* View position switch through scrall bar not accurate enough
		@ class %ListBox,
	+ synchronization of viewer length @ class %TextList,
	+ widget layout managers,
	+ widget-based animation support
);

b769-b1256:
^ \mac __PRETTY_FUNCTION__ ~ custom assertion strings @ whole YFramework
	when (^ g++),
/ memory fragment issues,
/ basic routines $=
(
	/ ystdex::fixed_point $=
	(
		* \impl @ \op/= for signed types,
		+ 64-bit integer underlying type support
	),
	+ u16printf,
	+ u32printf
),
/ completeness of core abstraction $=
(
	/ shell switching $=
	(
		+ locking;
		+ exceptional state and handlers
	),
	/ messaging $=
	(
		+ general predicates for removing message,
		^ timing triggers @ message loop,
		^ weak_ptr @ shell messages
	),
	/ \cl String interface;
	/ fully \impl @ \cl Path
),
/ services $=
(
	+ general \impl @ images processing interface,
	^ <chrono> to completely abstract system clocks,
	+ general data configuragion,
	/ general file type abstraction
),
/ @ "GUI" $=
(
	+ widget models,
	+ IMEs,
	+ widget layout \impl,
	+ modal widget behavior
);

b1257-b1728:
/ platform dependent system functions $=
(
	+ correct DMA (copy & fill) @ DS
),
/ completeness of core abstraction $=
(
	/ messaging $=
	(
		+ general predicates for removing message,
	),
	/ fully \impl @ \cl Path,
	/ handles $=
	(
		- \a direct dereference operations of handle type,
		+ real handle type with no \op*
	)
),
/ services $=
(
	+ user-defined stream filters,
	/ improving \impl font switching,
	/ fully \impl logging $=
	(
		+ more clarified log Levels,
		+ log streams
	),
	/ @ "GDI" $=
	(
		+ basic shapes abstraction,
		+ spline nodes abstraction,
		/ more efficient Font switching,
		/ text alignment,
		/ advanced text layout like Unicode layout control
	)
),
/ @ "GUI" $=
(
	+ document-view models,
	/ focusing $=
	(
		+ focus iteration,
		+ direct focus paths controling
	)
	+ more complex controls,
	+ fully \impl styles @ widgets,
	+ general widget decorators,
	+ clipping areas,
	+ dynamic widget prototypes
);


$LOW_PRIOR_TODO:
^ $low_prior $for_labeled_scope;
b1729-b5312:
+ general monomorphic iterator abstraction,
/ partial invalidation support @ \f DrawRectRoundCorner,
/ user-defined bitmap buffer @ \cl Desktop,
+ additional shared property,
/ improve efficiency @ \ft polymorphic_crosscast @ \h YCast for \conf release,
+ more advanced console wrapper,
+ a series set of robust gfx APIs,
+ (compressing & decompressing) @ gfx copying,
+ general component operations $=
(
	+ serialization,
	+ designers
),
+ automatic adaptors for look and feels,
+ networking,
+ database interface,
+ other stuff to be considered to append $=
(
	+ design by contract: DbC for C/C++, GNU nana
);


$KNOWN_ISSUE:
// NOTE: obsolete issues all resolved are ignored.
* "corrupted loading or fatal errors on loading font file with embedded \
	bitmap glyph like simson.ttc" $since b185,
* "g++ 4.6.1 internal error for closure object in constructors" $since b253,
	// g++ 4.6.2 tested @ b293.
* "<cmath> cannot use 'std::*' names" @ "!defined %_GLIBCXX_USE_C99_MATH_TR1"
	@ "libstdc++ with g++ 4.6";
* "g++ 4.6.1 ignores non-explicit conversion templates when there exists \
	non-template explicit conversion function" $since b260,
* "crashing after sleeping(default behavior of closing then reopening lid) on \
	real machine due to libnds default interrupt handler for power management"
	$since b279;
* "sorry, unimplemented: use of 'type_pack_expansion' in template"
	@ ^ "g++ (4.6, 4.7)" $before $future;

$RESOLVED_ENVIRONMENT_ISSUE:
* "g++ 4.5.2 fails on compiling code with defaulted move assignment operator"
	@ $interval([b207, b221));


$HISTORY:

$parser.state.style $= $natral_NPL;
$dep_from; // take evaluation dependence from;
$dep_to; // put evaluation dependence to;
$label; // label for locating in code portions;

$design; // features changing probably only made sense to developers who needs \
	to reference or modify the implementation of these libraries;
$comp; // features consist of dependencies with no additional work;
$doc; // for documents target;
$add_features +; // features added;
$fix_bugs *; // bugs fixed;
$modify_features /; // features modified;
$remove_features -; // features removed;
$using ^; // using;
$not !; // not;
$source_from ~; // features replaced from;
$belonged_to @;
$changed_to ->;
$moved_to >>;
$renamed_to =>;

//$transform $list ($list_member $pattern $all($exclude $pattern \
//	$string_literal "*")) +;

$ellipse_refactoring;
$ellipse_debug_assertion;

$ref $=
(
b298 $=
(
$note "access violation examples"
/*
http://stackoverflow.com/questions/5955682/boostmake-shared-causes-access\
-violation
http://lists.cs.uiuc.edu/pipermail/cfe-dev/2011-October/017831.html
*/
)
)

$module_tree $=
(
	'YBase',
	(
		'YStandardEx'
		(
			'Any',
			'CStandardIO',
			'Iterator',
			'Algorithms',
			'Utilities'
		)
	),
	'YFramework'
	(
		'CHRLib'
		(
			'CharacterMapping',
			'encoding conversion'
		),
		'YCLib' $=
		(
			'native APIs',
			'common input APIs',
			'common file system APIs',
			'debug helpers'
		),
		'YSLib'
		(
			'adaptors',
			'core'
			(
				'basic objects',
				'core utilities',
				'devices',
				'messaging',
				'events',
				'shell abstraction',
				'file system abstraction',
				'application abstraction'
			),
			'services',
			'GUI',
			'UI styles'
		)
		'Helper'
		(
			'global helper unit',
			'shells for DS';
			'DS main unit',
			'initialization'
		),
	),
	'YReader'
	(
		'main',
		'initialization',
		'file explorer',
		'shells test example',
		'text reader',
		'hexadecimal browser'
	)
);

$now
(
	/ %'YBase' $=
	(
		(
			+ "variadic meta types and operations";
			+ "unsequenced call function template %unseq_apply",
		)
		* $design "ambiguous overloading with empty template argument list"
			@ "function template %(parameterize_static_object, get_init)"
			$since b301;
	);
	/ %'YFramework'.'YSLib' $=
	(
		/ %'YSLib' $=
		(
			/ %'GUI' $=
			(
				+ "function template %(AddWidgets, AddWidgetsZ, RemoveWidgets)"
					@ "header %yuicont.h" @ %'GUI',
				(
					$dep_from "timer delay";
					+ "member function %Delay" @ "class %InputTimer"
					$dep_from "input timer delay";
				)
			),
			(
				+ "member function %Delay" @ "class %Timer" @ %'services'
				$dep_to "timer delay";
			),
			(
				/ %'helpers' >> %$$'Helper';
				$dep_to "helpers"
			)
		),
		+ "adapting macros for DS and MinGW32" @ %'YCLib'.'common input APIs',
		(
			$dep_from "helpers";
			/ $design "simplified interfaces" @ %'initialization'
		)
	);
	/ %'YReader' $=
	(
		$design "simplified implementation",
		/ %'text reader' $=
		(
			* @ defined YCL_DS $since b299
			(
				* $design "wrong implementation" @ "macro %YCL_KEY(X)"
					@ "header Shells.h";
				* "key 'Y' no respond",
				* "wrong font size set when responding key ('X', 'Y') and \
				  boundary font size reached"
			)
			(
				$dep_from "input timer delay";
				* "%KeyDown responded more than once when the input waiting \
					interval is smaller than actual delay" $since b300;
					// ('X', 'Y') on DS delayed and led to over setting \
						of font size.
			),
		),
		- "direct UI drawing test" @ %'shells test example',
			// It has never been compatible on MinGW32.
		* "slash-ended item not recognized as directory" @ defined YCL_MINGW32
			@ %'file explorer' $since b299
	)
),

b302
(
	/ %'YFramework'.'YSLib' $=
	(
		/ %'GUI' $=
		(
			/ @ "unit %YStyle" $=
			(
				+ "typedef %hue" @ "unit %YStyle"
				* "overflow on boundary saturation or light value"
					@ "function %HSLToColor" $since b276,
			);
			/ @ "unit %Button"
			(
				+ "hue settings for button painting",
				- "class %CloseButton"
			),
			(
				+ "function %DrawArrow" @ "unit %YWidget";
				/ $design "simplified arrow painting" @ "scroll bar buttons"
			)
		),
		/ "shell switching helper function" %'helpers'
			// Destination shell handle message post to set shell is set \
				to null to avoid potentional dead lock in Message loop when \
				called more than once.
	),
	* "infinite loop after pressing %Esc" @ "main UI" @ %'YReader'.'text reader'
		$since b300
),

b301
(
	/ $design "all %inline member outside of class or class template definition"
		@ "headers" >> "class or class template definition"
	/ %'YBase'.'YStandardEx' $=
	(
		/ %'Utilities' $=
		(
			+ "function template %call_once",
				// Just like %std::call_once, but with no thread safety \
					guarantee.
			(
				+ "function template %parameterize_static_object";
				+ "function template %get_init";
				% dep_to "get_init"
			)
		),
		/ $design "simplified implementation" ^ "constexpr brace-initializers"
			@ "class template %iterator_operations" @ %'Iterator'
	),
	/ %'YReader' $=
	(
		/ $design "simplified implementation" @ %'main',
		(
			/ $dep_from "get_init";
			/ $design "simplified implementation" @ %'text reader',
		),
		+ "recovering path on exiting reader" $=
		(
			+ "initialized with path argument" @ "class %ShlExplorer";
			/ "set shell path on exiting" @ "class %ShlReader"
		)
	)
	/ %'YFramework'.'core' $=
	(
		* $doc @ "class template %safe_delete_obj" @ %'core utilities'
			$since b263
		/ "completed proper result type" @ "function %GetAreaOf"
			@ "header ygdibase.h"
	)
),

b300
(
	/ %'YFramework' $=
	(
		/ %'YSLib' $=
		(
			/ %'GUI' $=
			(
				/ @ "unit %YGUI" $=
				(
					+ "input timer class %InputTimer";
					+ "input timer API for repeating key input"
				),
				+ "function template %OnEvent_Call for forwarding event calling"
					@ "unit %YControl"
			),
			- "static const objects" @ "class %Application"
				@ %'core'.'application abstraction'
		),
		* "key updating" @ "defined %YCL_MINGW32"
			@ %'YCLib'.'common input APIs' $since b299
			// It seems that sometimes %GetAsyncKeyState reads unexpected \
				virtual key value like '0xFF'.
	);
	/ %'YReader'.'text reader' $=
	(
		* "abnormal delaying on responding event %KeyHeld" $since b271,
			// Since the event routing strategy was not %Tunnel.
		* "crashing when setting font size" $since b297
	),
	(
		/ $design "reduce several global objects";
		/ $design "deprecated unit %GBKEX.cpp" >> "library %YSLib"
			~ "project %YSTest_ARM9";
		+ "dynamic linked library targets" @ "Code::Blocks projects for MinGW32"
	),
	/ "directory configurations" >> "unit %Initialization" @ "directory %Helper"
		~ "file %GBKEX.cpp";
	- "deprecated file %GBKEX.cpp";
	/ $doc "directory %doc moved to top directory",
	- $doc @ "Code::Blocks project file",
	^ "updated devkitARM release 38" ~ "devkitARM release 37" @ "platform %DS",
	^ "updated libnds 1.5.4 with default arm7 0.5.23"
		~ "libnds 1.5.1 with default arm7 0.5.21" @ "platform %DS"
),

b299
(
	/ %'YFramework' $=
	(
		/ %'YSLib' $=
		(
			/ %'helpers' $=
			(
				/ $design "input dispatching" @ "member function \
					%ShlDS::OnGotMessage" >> %'DS main unit' ~ %'shells for DS',
				/ "function %InitConsole" @ %'global helper unit'
					>> 'DS main unit',
				/ %'DS main unit' $=
				(
					* "shells not destroyed before the application class \
						destruction" $since b269,
					* "wrong context buffer pointer initialized" @ "constructor"
						@ "class %DSScreen" @ "defined %YCL_MINGW32" $since b298
				)
			),
			/ "improved implementation" @ "function %IsAbsolute"
				@ %'YCLib'.'common file system APIs' $=
			(
				+ "support for absolute path beginning with 'sd:/'"
					@ "defined %YCL_DS",
				+ "implementation" @ "defined %YCL_MINGW32"
			),
			* "destructor" @ "class %Shell" @ %'core'.'shell abstraction'
				$since $before b132 $= (- "wrong assertion"),
			* @ %GUI $since b298 $=
			(
				/ "constructor implementation" @ "class %TextList";
				* $comp "missing response to 'Enter' key"
					@ "class %(TextList, ListBox, Menu, DropDownList)"
			),
			/ %'CHRLib' $=
			(
				+ $design "static assertion for volatile type"
					@ "function template %FillByte" @ "header %smap.hpp",
					// To avoid undefined behavior.
				* "EOF not checked for distinguishing with invalid source \
					state and raising by conversion" @ "character conversion \
					for source type %std::FILE*" @ 'encoding conversion'
					$since b248
					// End valid byte in the file is always untouched. This \
						would probably cause infinite loop in buffering \
						file with size less than block size in text file \
						reader, for there is no check for EOF in the \
						loop for conversion while reading from the file.
			)
		),
		/ %'YCLib' $=
		(
			/ %'common input APIs' $=
			(
				+ "header %Input.h for several input APIs";
				/ "APIs moved"
			),
			/ %'debug helpers' $=
			(
				+ "header %Debug.h for debugging and diagnose";
				/ "APIs moved"
			),
			(
				+ "wide character string path support"
					^ %'CHRLib'.'encoding conversion'
					@ %'common file system APIs' @ "defined %YCL_MINGW32"
				$dep_to "wide character string path"
			)
		)
	),
	/ %'YReader'.'shells test example' $=
	(
		$dep_from "wide character string path";
		+ "wide character string path support" @ "defined %YCL_MINGW32"
	),
	+ $design "MinGW32 projects" @ "Code::Blocks workspace"
),

b298
(
	/ %'YFramework' $=
	(
		/ %'YCLib' $=
		(
			/ "native key types" @ %'common input APIs',
			/ "file system APIs" @ %'Native APIs' $=
			(
				+ $design "assertions",
				* "missing directory validation" @ "function %opendir"
					@ "defined %YCL_MINGW32" $since b296
			);
			/% 'common file system APIs' $=
			(
				/ "types and macros",
				(
					/ @ "file node iterator" $=
					(
						/ "simplified interface",
						/ "iteration performance improved for removal of \
							copying node names",
						* "broken member function %IsDirectory" $since b221,
						^ "class name %HFileNode" ~ "%HDirectory"
					)
				)
			)
		);
		/ %'YSLib' $=
		(
			/ %'GUI' $=
			(
				/ "key events response" @ "controls"
				* "implementation" @ "class %ListBox" $since b261
					$= (- "wrong assertion"),
				(
					$dep_from "file list interface";
					/ $design "implementation" @ "class %FileBox"
				),
				+ "member function %Reset" @ "class template %GSequenceViewer",
				/ "default holding intervals"
					// Key: (240ms, 120ms) -> (240ms, 60ms); \
						Touch: (240ms, 60ms) -> (240ms, 80ms);
			),
			/ "key events response" @ "class %ShlDS" @ %'helpers'.'shells',
			/ %'core'.'file system abstraction' $=
			(
				/ @ "class %Path" $=
				(
					* "directory path validation" $since b153;
					+ "member function %NormalizeTrailingSlash"
				);
				/ "class %FileList" @ $=
				(
					(
						/ "simplified interface";
						$dep_to "file list interface"
					),
					* "directory path validation" $since b153,
					/ "automatic normalizing trailing slash when validating \
						path string"
				)
			)
		)
		/ $design "file system APIs" %'YCLib',
		(
			$dep_from "make_shared",
			^ $design "%ystdex::make_shared" ^ "%std::make_shared"
				// %std::make_shared with RTTI will enlarge size of object \
					files. Several implementations may cause access violation, \
					see $ref b298.
		),
		* $design "implementation" @ "unit %YNew" @ %'adaptors' $since b203
	),
	/ @ %'YBase' $=
	(
		+ $design "assertion macros",
		(
			+ "function template %ystdex::make_shared";
			$dep_to "make_shared"
		)
	),
	/ %'YReader' $=
	(
		/ $design "key event response" @ 'text reader',
		* "position argument overflow (wrapped by 512KB) when scrolling"
			@ %'hexadecimal browser' $since b268,
	)
),

b297
(
	/ %'YReader'. $=
	(
		/ %'text reader' %=
		(
			(
				(
					* "missing automatic scrolling response" $since b296;
					+ $comp "adjusting reader when key is down"
				);
				(
					/ "stopping automatic scrolling when setting panel entered";
					* $comp "smooth scrolling wrong behavior when the delay \
						set less than before" $since b292
				)
			),
			/ "default setting"
		),
		* "integer dividing by 0" @ "background" @ %'shells test example'
	),
	+ "function template %union_cast for type casting through anonymous union"
		@ %'YBase',
	/ %'YFramework' $=
	(
		/ "unit %YCommon" @ %'YCLib' $=
		(
			/ @ "defined %YCL_MINGW32" $=
			(
				/ "pixel format";
				/ $design "implementation" @ "class %Color \
					for efficiency bitmap transferring",
			)
			/ "function %ScreenSynchronize" @ "namespace %platform"
				>> "namespace %platform_ex"
		),
		/ $design "implementation" @ "class %Palette" ^ "initializer lists"
			~ "assignments for efficiency" @ %'UI styles',
		+ "blit algorithm implementation without pixel format dependency"
			@ "unit YBlit" @ %'services',
		* "using implicitly deleted copy constructor on events causing \
			ill-formed program" @ "class %BorderBrush" @ %'GUI' $since b295,
		/ %'core' $=
		(
			* "missing virtual destructor" @ "class %GraphicDevice"
				@ %'devices' $since b296,
			/ "message %SM_INPUT value and parameter type" @ %'messaging'
		),
		* "accessing uninitialized member" @ "font cache" $since $before b132,
		/ $design "exposed message dealing interface" @ 'helpers',
	),
	/ $design "VS2010 solution directories and filters",
	/ $design (+ '-D_GLIBCXX_DEBUG' @ "compiler command line" @ "debug mode")
		@ "makefiles" @ "all projects"
),

b296
(
	/ %'YFramework' $=
	(
		/ %'YSLib' $=
		(
			/ %'services' $=
			(
				/ @ "class %(BitmapBuffer; BitmapBufferEx)" $=
				(
					* $doc "comments for destructor wrongly designed to copy \
						constructor" $since b240,
					* "implementation" @ "move constructor" $since b241;
					+ "member function %Swap with exception specification \
						%ynothrow";
						// The using of %std::swap for %BitmapBuffer forbids \
							the move assignment operator throwing exceptions \
							or the no throwing specification will be broken.
					* "missing assignment operators" $since $before b132
						$= (+ "copy and move assignment operators"),
						// The unification assignment cannot be used here \
							because the copy assignment may throw but move \
							assignment may not.
				),
				$design
				(
					/ $design "accessors interface" @ "class %BitmapBuffer";
					$dep_to "simplified class %BitmapBuffer"
				),
				+ "explicitly defaulted copy and move constructors and \
					assignment operators" @ "class %TextRegion",
				- "member function %BitmapBuffer::BeFilledWith"
			),
			/ %'core' $=
			(
				+ $design "exception specification %ynothrow"
					@ "class %ValueObject" @ %'basic objects';
				/ $design @ "class %Message" @ %'message' $=
				(
					/ "unification operator" -> "copy assignment operator \
						without exception specification and move assignment \
						operator with exception specification %ynothrow";
					+ "exception specification %ynothrow" @ "member move \
						assignment operator with parameter type ValueObject&&"
				),
				$design
				(
					/ $design "accessors interface" @ "class %GraphicDevice",
					$dep_from "simplified class %BitmapBuffer";
					- "polymorphisms" @ "class %Graphics"
				),
				* $doc @ "static member" @ "screen object types"
					$since $before b132,
				(
					- "default arguments for removal of %Size::FullScreen"
						@ "function %(CopyTo, BlitTo)" @ "header %GDI",
					$dep_from "%Rect::FullScreen platform dependency from \
						widgets",
					$dep_from "%Rect::FullScreen platform dependency from \
						test";
					/ @ "static member %FullScreen" @ "class %(Rect; Size)"
						-> "%Invalid";
					- $design $comp "platform dependency" @ "FullScreen"
				),
				/ $design "access control" @ "unit %YGDIBase" $=
				(
					+ "constructors with parameter const %Rect"
						@ "class template %GBinaryGroup, class %Size",
					/ @ "class %Rect" $=
					(
						/ "public base classes" -> "private bases",
						+ "several using members",
						+ "mutable member reference accessors"
					)
				)
			),
			/ %'GUI' $=
			(
				(
					/ "widget size bound to screen" @ "class %Desktop"
						~ "platform dependent %Rect::FullScreen";
					$dep_to "%Rect::FullScreen platform dependency from \
						widgets"
				),
				* $doc @ "function %LocateForParentContainer"
					@ "header yuicont.h" $since b242,
			),
			/ %'helpers' $=
			(
				+ %'DS main unit';
				/ "class (DSScreen, DSApplication, MainShell)"
					>> %'DS main unit'
			)
		),
		/ $design %'YCLib' $=
		(
			/ "include guards macros renamed"
				@ "header %(NativeAPI.h, Platform.h)",
			- "unused macro %_ATTRIBUTE" @ "header Platform.h",
			/ ("several APIs" @ "namespace %platform"
				>> "namespace %platform_ex") @ "unit %YCommon",
			+ "file system interface adaptors for MinGW32"
		)
	),
	/ $design @ %'YReader' $=
	(
		/ "simplified shells",
		(
			!^ "%Rect::FullScreen";
			$dep_to "%Rect::FullScreen platform dependency from test"
		)
	),
	/ %'YBase' $=
	(
		* "empty implementation" @ "macro %(YCL_LIKELY, YCL_UNLIKELY)"
			@ "header %ydef.h when '!(YCL_IMPL_GNUCPP >= 29600)'" @ $since b294,
		- "exception specification %ynothrow" @ "implementation"
			@ "macro %yunseq",
			// Allowing throwing exceptions is convenient but not safe, and \
				users now have resposibility for checking by self.
		+ "macro %YCL_ATTRIBUTE for implementation supported attributes"
	)
),

b295
(
	/ %'YFramework'.'YSLib' $=
	(
		/ %'GUI' $=
		(
			(
				/ "simplified refreshing" @ "class %Thumb",
				/ $design "simplified refreshing" @ "class %(CheckBox, ATrack, \
					AScrollBar, Panel, Window)",
				/ "improved refreshing" @ "class %AScrollBar";
				* $comp "background of buttons over refreshed" $since b294,
				* $design $comp "background of scroll bars over refreshed"
					$since b294,
				/ $comp "refreshing performance improved"
			),
			+ $design "partial invalidation support on refreshing"
				@ "class %BufferedTextArea",
			/ $design "refreshing" ^ "border brush" @ "class %ProgressBar",
			* "memory leak" $since b243
				$= (+ "missing destructor" @ "class %AController"),
			+ $design "copy and move assignment ooperator" @ "class %View"
		),
		/ %'core'.'shell abstraction' $=
		(
			/ "interface of shell switching";
			$dep_to "DS shell member functions",
			- "message %(SM_ACTIVATED, SM_DEACTIVATED)" @ %'messaging'
		),
		/ %'helpers' $=
		(
			- "global desktop" @ %'global helper unit',
			/ %'shells for DS' $=
			(
				^ "dynamically allocated individual desktop objects"
					~ "shared static desktop objects",
				- $design "member functions %OnDeactivated",
				/ "member functions %OnActivated merged to constructors"
			);
			$dep_to "DS shell member functions"
		),
		/ $design "more strict access controlling" @ "several classes",
	),
	/ %'YReader'.'shells test example' $=
	(
		(
			+ "refreshing interval" @ "class %FPSCounter";
			+ "FPS counter refreshing frequency limit",
		),
		/ $design $dep_from "DS shell member functions" $=
		(
			- "all member functions %OnDeactivated",
			/ "all member functions %OnActivated merged to constructors",
		),
		/ $design "shell objects" ^ "dynamic storage" ~ "static storage"
	)
),

b294
(
	/ %'YFramework'.'YSLib' $=
	(
		/ %'core'.'events' $=
		(
			+ "event priority type";
			/ "event container" ^ "%multimap" ~ "list";
			$dep_to "event with priority"
		),
		/ %'GUI' $=
		(
			/ "widget controller event %Paint delayed to initialize in \
				initialization of objects with class %Widget",
			+ "brush class %ImageBrush";
			/ "widget background painting" ^ ("new member %HBrush Background"
				@ "class %Widget") ~ "function %Render",
			(
				$dep_from "event with priority";
				(
					+ "paint widget event priorities" @ "header %ywgtevt.h";
					$dep_to %'YReader'.'text reader'
				),
				/ $design "border painting with priority"
					@ "class %(TextList, DialogBox, DialogPanel)"
			),
			(
				* "missing clip area update" @ "class %BorderBrush" $since b284;
				* $comp "border overlapping between widgets" $since b284
				$dep_to %'YReader'.'text reader'
			),
			/ "widget refreshing interface",
			+ "widget rendering state sharing between parent and children"
		),
		+ "protected boolean members to determine whether the desktops would \
			be updated" @ %'helpers'.'shells for DS'
	),
	* $doc @ "function template %xcrease" @ %'YBase'.'YStandardEx'.'Utilities'
		$since b243,
	/ %'YReader'.'text reader'
	(
		/ $design "border painting with priority" @ "class %ColorBox",
		* "border of setting panel overlapped by border of automatic scrolling \
			setting drop down list" $since b292,
		* "text encoding not synchronized with setting" $since b292
	),
	(
		/ "header %ydef.h" @ %'YBase' $=
		(
			+ $doc "comments for macros",
			+ $design "branch predication hint macros"
		);
		^ $design "branch predication hint macros" @ "whole project"
	)
),

b293
(
	/ $design %'YBase' $=
	(
		* "broken implementation" @ "macro %yforward" @ "header %ydef.h"
			$since b245;
		+ "template function %make_unique" @ "header %memory.hpp"
	);
	/ %'YFramework' $=
	(
		/ $design "helper function usings" @ "header yref.hpp" @ %'adaptors',
			// Using of std::make_shared leads to code bloat for RTTI.
		* "member function template parameter types missing '&&'"
			@ "header %(yevent.hpp, yfunc.hpp)" @ %'core' $since b210,
		* "constructor template parameter type missing '&&'" @ "class %Widget"
			@ 'GUI' $since b258,
		/ %'GUI' $=
		(
			/ "rendering logic" @ "unit %(YRenderer, YWidget)" $=
				// Performance is improved a little.
			(
				/ "renderers" $=
				(
					/ "member function %Refresh" -> "%Paint",
					/ "member function %Validate" @ "class %BufferedRenderer"
				),
					// Now calling event %Paint instead of widget member \
						function %Refresh.
				/ "function %Render",
					// Now calling widget member function %Refresh instead of \
						renderer member function %Refresh.
				/ "function %PaintChild"
					// Now calling renderer member function %Paint instead of \
						event %Paint.
			),
			+ "brush class %SolidBrush",
			/ $design "base controller methods" @ "class %AController"
		),
		/ "constructors" @ "class %Timer" @ %'services',
		* $design "missing 'const' when casting" @ "implementation"
			@ "class %ValueObject" $since b217
	),
	* "wrong pixel offset when stopping smooth scrolling"
		@ %'YReader'.'text reader' $since b292
),

b292
(
	/ %'YFramework' $=
	(
		* "implementation" @ "function %(WaitForInput, WaitForKey)"
			@ "unit %YCommon" @ %'YCLib' $since b291,
		/ %'YSLib' $=
		(
			* "strict ISO C++ code compatibility" $since b273
				$= (/ "assertion strings" @ "implementation unit %TextManager"
					@ %'services' !^ "%__FUNCTION__"),
			/ %'GUI' $=
			(
				/ @ "class %DropDownList" $=
				(
					* "selection of top/bottom spaces to extract list to be \
						shown" @ $since b290,
					/ "fixed position of arrow and right margin",
					/ "refreshing as pressed down when list shown"
				),
				* "member function %ListBox::ResizeForPreferred when \
					limit width is zero" $since b282,
				(
					+ "container setter" @ "class template %GSequenceViewer";
					+ "member function %SetList"
						@ "class %(MTextList, TextList, ListBox, DropDownList)",
				)
			)
		)
	),
	/ %'YReader'.'text reader' $=
	(
		+ "implementation" @ "custom encoding switching UI",
		/ "contiguous repeated bookmarks would not be inserted to reading list \
		  when random locating",
		+ "smooth scrolling mode",
		+ "automatic scrolling duration settings"
	),
	^ "updated library freetype 2.4.9" ~ "modified freetype 2.4.8"
),

b291
(
	/ %'YBase'.'YStandardEx' $=
	(
		+ "helper 2 template functions %arrlen";
		$dep_to 'YReader'.'text reader'
	),
	/ %'YFramework' $=
	(
		/ $design "all encoding parameter type" ^ 'Encoding' ~ 'const Encoding&'
			@ %'CHRLib',
		/ "timers" @ %'YCLib' $=
		(
			/ $design "simplified implementation" ^ "libnds functions"
				~ "direct operations on registers";
				// Now only counter 2 is used by this library instead of \
					all the 4 counters on ARM9;
			+ "high resolution counter";
				// It provides at most resolution as nanoseconds.
			$dep_to %'YReader'.'shells test example',
			/ $design "simplified implementation" @ "input functions"
		);
		/ "timers" @ '%YSLib'.'services'
		(
			+ "class %HighResolutionClock" ^ "%std::(duration, time_point)",
			+ "duration and time point types",
			^ "high resolution counter" ~ "milliseconds" @ "class %Timer"
		)
	),
	^ "updated devkitARM release 37" ~ "devkitARM release 35",
	/ %'YReader' $=
	(
		/ "more accurate FPS counter" @ %'shells test example',
		/ $design "simplified implementation"
			^ "function template %ystdex::arrlen" @ %'text reader'
	)
),

b290
(
	/ %'YBase'.'YStandardEx' $=
	(
		/ $design @ %'TypeOperation' $=
		(
			+ "meta operation %array_decay",
			(
				+ "meta operation %qualified_decay";
				$label "meta qualified"
			),
			+ "meta operation %array_ref_decay"
		),
		/ @ %'Iterator' $=
		(
			+ "class template %pointer_iterator";
			+ "meta operation to classify pointer types";
			+ "non-class type iterator support"
				@ "class template %transformed_iterator",
			(
				$dep_from "meta qualified";
				/ $design "qualified or array type decaying"
					@ "function template %make_transform"
			)
		)
	),
	/ %'YFramework'.'YSLib' $=
	(
		+ "stream encoding checking without BOM" @ "class %TextFile"
			@ %'services',
			// At most first 64 bytes would be read for checking, \
				now only for UTF-8 and GBK.
		$dep_to "no BOM checking";
		+ "top and down space comparing when list shown" @ "class %DropDownList"
	);
	/ %'YReader'.'text reader' $=
	(
		* "reader content and text information not refreshed after switched to \
			empty file using reading list" $since b289,
		(
			$dep_from "no BOM checking";
			+ $comp "automatic encoding checking of UTF-8 without BOM"
		),
		+ "custom encoding switching UI"
			// Actually switching is not implemented yet.
	)
),

b289
(
	/ "algorithm function template %(erase_all, erase_all_if)"
		@ %'YBase'.'Algorithms' $=
	(
		/ "return value" @ "overloading for sequence containers",
		(
			+ "overloading template for associative containers";
			$dep_to %'YFramework'.'YSLib'.'core'.'messaging'
		)
	)
	/ %'YFramework'.'YSLib' $=
	(
		+ $design "protected member to expose current message"
			@ "class %Application" @ %'core'.'application abstraction';
		/ %'helpers'.'global helper unit' $=
		(
			/ %'global helper unit' $=
			(
				+ "exposed const current message getter",
				/ $design "implementation" @ "main message loop"
					@ "member function %DSApplication::Run"
			)
		),
		(
			/ $design "bound control pointer assignment" @ "constructor"
				@ "class %Control" @ !^ "std::mem_fn" @ 'GUI';
			$dep_to "class %ShlExplorer" @ %'shells test example'
		),
		/ @ %'core'.'messaging' $=
		(
			+ "removing with specified bound and shell" @ "class %MessageQueue";
			$dep_to %'YReader'.'text reader'
		),
		/ @ "class %ProgressBar" %'GUI' $=
		(
			/ "refreshing efficiency improved",
			/ "zero max value automatic set as 1" @ "constructor"
		),
		/ %'services' $=
		(
			/ @ "class %Timer" $=
			(
				(
					+ "member function %IsActive";
					$dep_to %'YReader'.'text reader'
				),
				/ "implementation" @ "friend function %(Activate, Deactivate)"
			),
			/ @ "class %TextFileBuffer" $=
			(
				- $define "assertion of block number" @ "constructor";
				(
					/ "no block required for empty file";
					/ $comp "size of empty file shown as 0 rather than 1";
					$dep_to "reader box" @ %'YReader'.'text reader'
				)
			)
		)
	),
	/ %'YReader' $=
	(
		/ @ "class %ShlExplorer" @ %'shells test example',
		(
			- "invalidating area test",
			/ $design "bound control pointer assignment" @ "constructor"
				@ !^ "std::mem_fn"
		),
		/ %'text reader' $=
		(
			+ "automatic scrolling",
			/ @ "reader box" $=
			(
				+ "buffered rendering",
				(
					+ "empty file check"
						@ "member function %UpdateData";
					* $comp "crash on opening empty file" $since b273
				),
				/ "action for empty file canceled"
			)
		),
		/ "set ignoring background on buffered rendering"
			@ %'hexadecimal browser'
	)
),

b288
(
	/ $design %'YBase'.'YStandardEx' $=
	(
		/ %'TypeOperations' $=
		(
			+ "template class %n_tag";
			+ "typedef n_tag<0> %first_tag",
			+ "typedef n_tag<0> %second_tag"
		),
		/ %'Iterator' $=
		(
			(
				+ "class template %transformed_iterator";
				+ "function template %make_transform",
			),
			+ "class template %pair_iterate for selecting members";
			$dep_from 'TypeOperations';
			+ "manipulators for selection members";
			+ "pipe operators for selecting members"
		),
		$dep_to 'YReader'.'text reader'
	),
	/ %'YFramework'.'YSLib' $=
	(
		/ $design "simplified implementation" @ "class %Thumb" @ %'GUI',
		* $design "comments" @ "header %ycutil.h" $since b281,
		/ $design "implementation" @ "idle handling",
		/ %'core'.'messaging' $=
		(
			+ "member function %MessageQueue::GetMaxPriority"
				@ "implementation" @ "message peeking",
			$dep_to "implementation" @ "main message loop";
			- "shell message identifier %SM_DROP",
			+ "shell message identifier %SM_TASK"
		),
		/ $design "implementation" @ "main message loop"
			@ "member function %DSApplication::Run" @ %'global helper unit'
	),
	/ %'YReader' $=
	(
		/ $design "simplified event handlers" @ "class %ShlExplorer"
			@ %'shells test example' ^ "lambda expressions"
			~ "static member functions",
		/ %'text reader' $=
		(
			$dep_from %'YBase'.'YStandardEx'.'Iterator';
			/ $design "simplified implementation",
			* "subsequent bookmarks not erased when loading a file"
				@ "reading list" $since b287
		)
	)
),

b287
(
	/ %'YFramework'.'YSLib' $=
	(
		/ %'GUI' $=
		(
			/ "right component of margin" @ "constructor"
				@ "class %DropDownList",
			* "wrong vertical alignment get"
				@ "member function %MLabel::PaintText" $since b208,
			+ "focus requesting/releasing" @ "function %(Show, Hide)"
				@ "unit %YWidget",
			* $comp "wrong alignment when refreshing \
				widget" @ "class %DropDownList",
			/ "GUI state abstraction" $=
			(
				+ "class %GUIState",
				/ "common state implementation" ^ "static shared %GUIState"
					~ "state of GUIShell";
				- "class %GUIShell"
			)
		),
		/ "string and path interface" $=
		(
			+ "member function %String::GetMBCS",
			- "function %MBCSToString",
			/ $design "inheritance" @ "class %Path"
				@ %'core'.'file system abstraction' ^ "class %String",
			- "local encoding alias %CP_Local"
			/ "all pattern 'CP_*'" => "pattern 'CS_*'"
		),
		+ $design "public inheritance %enabled_shared_from_this"
			@ "class %Shell" @ %'core'.'shell abstraction'
	);
	/ %'YReader'.'text reader' $=
	(
		* "functions of key %'Left' down" $since b286,
		/ "stopping handling key events when setting panel is shown",
		/ $design "reader manager as sessions",
		+ "cross-session shared shell state support"
			@ ("last reading list", "settings")
	)
),

b286
(
	/ %'YFramework' $=
	(
		/ $design "native path types" @ %'YCLib';
		/ %'YSLib' $=
		(
			/ $design "simplified inheritance" @ "class %Path"
				@ %'core'.'file system abstraction',
			+ "constructor %String(ucs2string&&)" @ "class %String",
			(
				/ "class %Padding as literal type";
				+ $design "literal type object %DefaultMargin"
					@ "header %TextBase.h";
				/ "margin setting" @ "constructor" @ "class %TextBase",
				/ %'GUI' $=
				(
					+ "return value" @ "function %Enable" @ "unit %YControl",
					/ "margin setting" @ "constructor" @ "class %DropDownList"
				)
			)
		)
	),
	/ %'YReader'.'text reader' $=
	(
		(
			+ "class %BookMark";
			+ "class %ReadingList";
			+ "shell last reading list"
				// This list is only valid in the same shell activated lifetime.
		),
		/ $design "static data members" @ "class %ReaderManager"
			>> "class %ShlReader",
		/ $design "simplified implementation of activating and deactivating",
		/ "functions of key %'(X, Y, L, R, Left, Right)' down",
		* "random locating error" @ "member function %DualScreenReader::Locate"
			$since b271
	)
),

b285
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		- "hosted menu referent pointer" @ "unit %Menu",
		+ "member function %(SetSelected, ClearSelected)"
			@ "class %ListBox";
		+ "item text matching when list shown" @ "class %DropDownList",
		* "viewer length of text list no synchronized when initialized"
			$since b203
	),
	* "menu not hidden when touching reader box" @ %'YReader'.'text reader'
		$since b283
),

b284
(
	/ %'YFramework'.'YSLib' $=
	(
		/ %'GUI' $=
		(
			/ "border APIs" @ "unit Border" $=
			(
				+ "class BorderBrush",
				* "border shown when widget is transparent" $since b276
			),
			+ "auto request focus" @ "class %ListBox for proper border color",
			* "focus not cleared when container being touched down" $since b283
		),
		- "shell message on activating and deactivating on shells"
			@ "class %(Shell, ShlCLI)"
	),
	/ "menu selected index cleared when shown" @ %'YReader'.'text reader'
),

b283
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		* @ "destructor" @ "class %Widget"
			$since b240 $= (- "calling event %LostFocus"),
			// It depends on itself to be a complete object in \
				its lifetime which does not fit for destructing.
		(
			/ @ "class %View"
			(
				+ "dependency pointer";
				/ "visible and transparent property setter and getter"
			);
			+ "top widget view state synchronizing" @ "class %DropDownList"
		),
		/ "focus operation for containers" @ "class %YGUIShell"
			>> "class %Control",
		/ "focus requesting on handling event %TouchDown of controls \
			only when the strategy is bubble";
		/ @ "class %DropDownList" $=
		(
			* "top widget not removed when destructing" $since b282,
			(
				+ "top widget focus requesting";
				* $comp "top widget not hid when touching again"
					$since b282
			)
		)
	);
	* $comp "crashing when switching shell after testing of drop down list"
		@ %'YReader'.'shells test example' $since b282
),

b282
(
	/ %'YFramework'.'YSLib' $=
	(
		/ %'GUI' $=
		(
			/ $design "APIs for text list resizing for contents",
			(
				/ $design "unit names" @ "directory %UI";
				+ "class %DropDownList" @ "unit %ComboList"
			),
			- "assertion" @ "constructor" @ "class %(HorizontalScrollBar, \
				VerticalScrollBar),
			* "wrong comments" @ "constructors of track classes" $since b173
		),
		* "wrong result for function %Intersect" @ "unit %YGDIBase" $since b227
	);
	+ "drop down list test" @ "class %ShlExplorer"
		@ %'YReader'.'shells test example'
),

b281
(
	/ %'YFramework'.'YSLib' $=
	(
		/ "text list refreshing implementation" @ %'GUI' $=
		(
			* "text not fully shown at bottom" $since b190;
			/ "high light area height"
		),
		$design
		(
			+ "deleting second member debug functor" @ "header %ycutil.h";
			/ $design "several implementations" @ "unit %(YTimer, Menu, \
				YUIContainer, YEvent, YMessage)" ^ "%std::for_each" ~ "for loop"
		),
		/ @ "unit %YString" $=
		(
			/ "enhancement of constructors of class %String";
			- "function %MBCSToString"
		),
		/ "event call catching %std::bad_function_call silently"
	),
	/ "unit %Shells" @ %'YReader' $=
	(
		/ "simplified testing implementation",
		+ $design "font family names list" @ "class %ShlExplorer"
	),
	/ %'YBase' $=
	(
		* $design "wrong Doxygen comments 'defgroup functors'"
			@ "header %utility.hpp" $since b193,
		* $design "wrong Doxygen comments file 'brief'"
			@ "header %any.hpp" $since b243
	)
),

b280
(
	/ %'YReader'.'text reader' $=
	(
		+ "automatic updating font before print text";
		/ @ "setting panel" $=
		(
			* "button text" $since b279,
			/ "label text",
			+ "text size setting"
		)
	),
	/ %'YFramework'.'YSLib' $=
	(
		/ $design "simplified font APIs",
		/ "font APIs" $=
		(
			/ $design "several getters" @ "class %FontCache" >> "class %Font",
			/ $design "stored state" @ "class %Font";
				// It brings the ability of automatically state synchronizing \
					with FT_Size with almost no efficiency changing, \
					but 'sizeof(Font)' grows obviously.
			/ "eliminating need of updating font"
		),
		* "undefined behavior raised when calculating advance of or rendering \
		  a glyph and the small bitmap descriptor is null" $since before b132
	)
),

b279
(
	/ %'YFramework'.'YSLib' $=
	(
		/ %'GUI' $=
		(
			(
				/ "support widget moving to top" >> "class %Panel"
					~ "class %Window";
				/ "support for class %Panel" ~ "class %Window"
					@ "function %RequestToTop"
			)
			* $design "assertion string" @ "4 function %(MoveToTop, \
				MoveToBottom, MoveToLeft, MoveToRight)" $since b171,
			/ "member funtion %Desktop::ClearContents" >> "class %Panel"
		),
		/ $design "default idle handler implementation"
			!^ "function %platform::AllowSleep to forbid sleeping \
			when the main message queue is not empty"
	),
	/ %'YReader'.'text reader' $=
	(
		/ $design "simplefied color API" @ "class %DualScreenReader";
		/ @ "setting panel" $=
		(
			/ "control appearance" !^ ("border", ^ "the top desktop")
			+ "text color setting"
		)
	)
),

b278
(
	/ %'YFramework' $=
	(
		+ "sleeping status configuring function %AllowSleep" @ %'YCLib';
		/ %'YSLib' $=
		(
			/ "font APIs" $=
			(
				* "memory leak when loading duplicate typefaces"
					@ "member function %FontCache::LoadTypefaces" $since b277,
				/ $design "font file loading implementation" @ "constructor of \
					class %Typeface" ~ "member function \
					FontCache::LoadTypefaces"
			),
			/ 'GUI' $=
			(
				* "missing painting of background for parent of buffered widgets"
					$since b225,
				+ "forced background painting ignorance state"
					@ "class %BufferedRenderer"
			),
			+ $design "sequence function application function template %seq_apply"
				@ "header %yfunc.hpp",
			+ "helper function object class %ContainerSetter",
			/ $design "default idle handler implementation"
				^ "function %platform::AllowSleep to forbid sleeping \
				when the main message queue is not empty";
			* "VRAM not flushed or crashing when opening lid on real DS"
				$since $before b132
		)
	),
	/ $design "simplified implementation" @ "file %main.cpp"
		@ "project %YSTest_ARM7",
	/ %'YBase' $=
	(
		/ $design "implementation for meta programing constant"
			^ "struct %integral_constant" ~ "static yconstexpr objects",
		/ "unsequenced expression returning first expression"
			^ "macro %yforward"
	);
	/ %'YReader'.'text reader' $=
	(
		/ "reader box shown as default",
		/ $design "simplified container pointer setting implementation"
			^ "function template %seq_apply"
	)
),

b277
(
	/ %'YReader'.'text reader' $=
	(
		+ "labels" @ "class %ColorBox",
		+ "labels" @ "setting panel"
	),
	/ %'YFramework'.'YSLib' $=
	(
		* "rounding of alpha blending color component" $since $before b132,
		* $design "platform-dependent bool type xor operation"
			@ "function template %Blit" $since b189,
		/ "font APIs" $=
		(
			/ $design "struct %FontStyle reimplemented as enum class",
			/ $design "simplified style names lookup function",
			* "wrong implementation" @ "all predicates" @ "class %Font"
				$since $before b132,
			- "class %FontFile",
			/ $design "data structure" @ "class %FontCache"
		)
	)
),

b276
(
	/ %'YBase' $=
	(
		/ "template %fixed_point" $=
		(
			* "wrong implementation" @ "casting to floating types" $since b260,
			* $design "wrong comments" $since b260,
			* "wrong implementation" @ "member function %operator*= \
				for signed types" $since b260,
			- "explicit @ casting operator"
		)
	);
	/ %'YFramework' $=
	(
		/ "storing 8-bit RGBA values in native color type class %Color"
			@ %'YCLib';
			// It spends more space but is more efficient in time.
		/ %'YSLib' $=
		(
			+ "adding/removing member function handlers with object type \
				distinct to class type" @ "class %GEvent",
			/ %'GUI' $=
			(
				+ $design "member function %OnTouch_Close" @ "class %Control";
				+ "class %ToolBox",
				/ "simplified color conversion APIs" @ "unit %YStyle",
				/ "color conversion APIs" ^ "%ystdex::fixed_point",
				+ "border APIs",
				/ "default related border color" @ "class %ProgressBar",
				/ "intersection calculation" >> "function %PaintChild#1"
					~ "function %Render"
			)
		)
	);
	/ %'YReader'.'text reader' $=
	(
		/ $design "simplified implementation",
		/ "control appearance" @ "setting panel" ^ "border"
	)
),

b275
(
	/ %'YFramework' $=
	(
		/ %'YSLib' $=
		(
			/ $design "decomposed unit %YText into unit %(TextBase.cpp, \
				CharRenderer, TextRenderer, TextLayout)",
			/ %'GUI' $=
			(
				+ "control default handler for requesting to top and focus \
					for event %TouchDown",
				- "%TouchDown handling for requesting to top and focus"
					@ "member function %GUIShell::ResponseTouch";
					// Clearing focusing pointer of containers is reserved in \
						%GUIShell::ResponseTouch, but now can only occur \
						before the control requesting for focus \
						in its parent widget.
					// It means that the requesting handler can be unloaded \
						by user manually.
				+ "hosted referent pointer" @ "class %MenuHost",
				/ @ "unit %YWidget" $=
				(
					* "no effect of function %(Show, Hide) \
						when used with widgets not buffered" $since b229,
					+ "function %Close for hiding and releasing focus"
				)
				* "missing thumb refreshing when invalidated area not \
				  intersecting with thumb" @ "member function %ATrack::Refresh"
				  $since b235,
				* "wrong position argument" @ "implementation"
					@ "member function %Button::Refresh" $since b266
			),
			- $design "dependency of header %platform.h" @ "header %ynew.h",
		),
		/ %'YCLib' $=
		(
			/ "efficiency improved" @ "function %ScreenSynchronize"
				^ "asynchronous DMA copy function %dmaCopyWordsAsynch"
				~ "function %dmaCopy"
		)
	);
	* $design "trivial comments not removed for %(is_not_null, is_null)"
		$since b254,
	+ "color picker class %ColorBox";
	/ %'YReader'.'text reader' $=
	(
		/ "behavior on clicking menu button",
		+ "setting panel"
	),
	$design "all code and document files" @ "project %YSTest"
		>> "project %YFramework",
	(
		- "string functions not strictly compliant to ISO" @ "all projects";
		/ $design "Makefile" @ "project %(YBase, YFramework)" ^ "-std=c++0x"
			~ "-std=gnu++0x"
	)
),

b274
(
	/ %'YFramework'.'YSLib' $=
	(
		/ "text rendering" @ %'services' $=
		(
			- $design "all function %SetMarginOf",
			* $design "2 function template %PrintLine cannot used with \
				iterator parameters without postfix operator++" $since b270,
			* $design "assertion string"
				@ "function %FetchResizedBottomMargin" $since b273,
			* $design "assertion string" @ "function %FetchResizedLineN"
				$since b252,
			* "implementation" @ "member function %TextLineNEx"
				@ "class template %GTextRendererBase" $since b267，
			* "implementation" @ "function %FetchLastLineBasePosition"
				$since $before b132 ^ "ascender" ~ "descender",
			* "implementation" @ "function %FetchResizedLineN"
				$since b252
		),
		/ $design "lessened header dependencies"
	);
	/ %'YReader'.'text reader' $=
	(
		/ @ "class %DualScreenReader" $=
		(
			+ $design "common margin",
			* $design "wrong line number"
				@ "member functions %(UpdateView, Execute)" $since b270,
			* "wrong view area shown after set line gaps" $since $before b132,
			+ "reading area resizing interface",
			* "wrong behavior for scrolling line up when top margins differed"
				$since $before b132;
			+ "top and bottom margins balancing"
		);
		/ "appearance"
		(
			+ "reading area resizing when showing or hiding reader box",
			/ "boxes transparency"
		)
	)
),

b273
(
	+ $design "class %pseudo_output" @ %'YBase'.'YStandardEx'.'Any',
	/ %'YFramework' $=
	(
		/ %'CHRLib' $=
		(
			+ "runtime character width getters",
			/ "encoding mapping APIs exposed",
			(
				/ $design "UCS2 output type of encoding mapping";
				+ "pseudo-converting-to-UCS2 APIs for width calculation"
			),
			/ "UTF-8 to Unicode code point mapping" $=
			(
				* "wrong behavior for 2 byte sequence characters" $since b250,
				+ "invalid byte checking",
				+ "4 byte sequence characters support"
			),
			(
				+ "enum class %ConversionResult";
				/ "converting-to-UCS2 APIs" ^ "enum class %ConversionResult"
					~ "byte as return type"
			)
		);
		/ %'YSLib' $=
		(
			+ "mapping buffering" @ "class %TextFileBuffer"
				@ "unit %TextManager" @ %'services',
			/ $design "member functions and types" @ "class %(File, TextFile)"
		),
	),
	/ "view" @ "class %TextInfoBox" @ %'YReader'.'text reader',
		// Efficiency for opening and random access operations to \
			large files improved obviously due to using of mapping buffering.
	* $design "several minor Doxygen warnings" @ $since b196
),

b272
(
	/ %'YFramework'.'YSLib' $=
	(
		/ %'core' $=
		(
			/ "simplified implementation" @ %'messaging';
			/ %'application abstraction' $=
			(
				- "messages producer calling" @ "function FetchMessage"
				- "default log object" @ "class %Application",
				- $design "try blocks" @ "all 2 function %SendMessage";
				/ $design "logging class %Log" >> "implementation unit %YGlobal"
					@ "directory %Helper"
			)
		);
		/ %'global helper unit' $=
		(
			/ "input message target type" ^ "class %InputContent"
				~ "shared_ptr<InputContent>";
			/ "efficiency improved" @ "function %Idle",
			/ $design "simplified implementation" @ "function %::main"
		),
		^ "rounding" @ "member function %ProgressBar::Refresh" @ %'GUI'
	),
	/ %'YReader'.'text reader' $=
	(
		* "scrolling down disabled after text bottom reached" $since b271,
		+ "return focus back to reader when box controls closed",
		+ "bottom position shown as distinct color" @ "box controls",
		/ "widgets layout" @ "class %ReaderBox"
	)
),

b271
(
	/ %'YReader'.'text reader' $=
	(
		+ "text reading progress information shown on both information boxes",
		/ "reading information indicating" @ "class %ReaderBox"
			^ "progress bar" ~ "track";
		+ "random locating of text by progress bar",
		/ "key responding all the time except menu shown"
	),
	/ %'YFramework' $=
	(
		/ %'YSLib' $=
		(
			/ %'GUI' $=
			(
				/ $design "implementation" @ "class %ScrollEventArgs"
					^ "class template %std::pair",
				/ @ "class %ProgressBar"
				(
					/ "main base class switched to class %Control"
						~ "class %Widget",
					/ "value type switched to float" ~ "u16"
				)
			),
			/ %'messaging' $=
			(
				/ $design "implementation of copying" @ "class %Message",
				/ @ "class MessageQueue" $=
				(
					- "redundant member functions",
					+ $design "copy assignment and move assignment merged \
						as unifying assignment"
					(
						+ "minor efficiency",
						+ "ability to use copy-elision optimization",
						* "exception safety of move assignment" $since b211
					)
				)
			)
		),
		* "wrong state in file stream conversion" @ %'CHRLib'.'CharacterMapping'
			$since b249
	),
	/ "behavior like class template %std::istream_iterator"
		@ %'YBase'.'YStandardEx'.'CStandardIO'.'class %ifile_iterator'
),

b270
(
	/ $design %'YReader'.'hexadecimal browser' $=
	(
		/ "resource management" ^ "class %unique_ptr" ~ "built-in pointers",
		/ "minor model interface"
	),
	/ "text rendering function 'Print*' ignoring newline characters";
	+ "macros for bitmask operations" @ "header %YBaseMacro",
	/ %'YReader'.'text reader' $=
	(
		// Using of unique_ptr and more contents of information box \
			at readers make it significantly slower than before in \
			%ShlExplorer when running on DeSmuMe at debug configuration.
		/ $design "resource management" ^ "class %unique_ptr"
			~ "built-in pointers"
		/ "unit %DSReader" $=
		(
			* "newline character cannot be print before EOF"
				$since $before b132,
			/ "more accurate indicating of text position when scrolling",
			/ "merge several interface as reading commands"
		),
		/ "size and content of file information box"
	),
	/ "implementation" @ "class %FontCache" $=
	(
		/ $design "interface" @ "member function %GetGlyph";
		/ "avoiding unnecessary rendering" @ "member function %GetAdvance"
	)
),

b269
(
	/ %'YReader' $=
	(
		/ %'hexadecimal browser' $=
		(
			+ "event subscription of actively locating the view"
				@ "class %HexViewArea",
			/ "information updating instantly to panel",
			+ "view scrolling by pressing arrow/L/R",
			+ "exiting by pressing Esc",
			* "encoding of updated path information" $since b263
		),
		/ "focus automatically requesting"
	),
	/ $design "simplified application instance management" $=
	(
		^ "assertion to check singleton constraint" ~ "access control",
		+ "destructor" @ "class %DSApplication",
		/ "main function cleanup"
	),
	* $design "null function call" @ "destructor" @ "class %Application"
		$since b243,
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		- "unnecessary view updating for empty list on event %KeyDown"
			@ "class %TextList",
		/ $design "simplified container control inheritance",
		+ "hiding by press Esc" @ "class %Menu"
	),
	/ $design "message loop" @ "function %::main"
),

b268
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ "invalidation on event %(Move, Resize) as default" @ "class %Control";
		/ "unnecessary invalidation reduced" @ "function %OnTouchMove_Dragging"
			@ "unit %Control",
		/ "unnecessary invalidation reduced"
			@ "class %(ATrack, Button, ListBox, MenuHost, TextList)",
		+ "partial invalidation support on event %Selected" @ "class %TextList",
		* "missing invalidation of the thumb before setting thumb position"
			@ "class %(HorizontalTrack, VerticalTrack, ATrack)" $since b224,
		* "minor line segments length on background"
			@ "member function ATrack::Refresh" $since b167,
		- "widget visibility check" @ "function %BufferedRenderer::Refresh",
		/= $design "confirmed no need of partial invalidation support \
			on event %Selected" @ "class %ATrack",
		(
			$design
			(
				+ "simple generic UI event argument class template \
					%GValueEventArgs";
				/ "simplified implementation" @ "class %IndexEventArgs"
			);
			+ "event subscription of active locating the view"
				@ "class %TextList"
		)
	),
	/ %'YReader' $=
	(
		/ "unnecessary invalidation reduced" @ "class %HexReaderManager",
		* "invalid scrollbar area shown for refreshing" @ "class %HexViewer"
	),
	* $design "ill-formed implementation of copy assignment function"
		@ "class template %GEvent" $since b207,
	/ $design "macro %ImplExpr for compatibility" @ "header %ybasemac.h"
),

b267
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ "simplified renderer interface",
		* "unstable length of scrolling thumb" @ "class %ATrack" $since b264
	),
	* "wrong buffered text rendering behavior" $since b266,
	^ "DMA copy on synchronizing to VRAM" @ "library %YCLib"
),

b266
(
	^ "updated library modified freetype 2.4.8" ~ "modified freetype 2.4.5",
	/ "invalidation algorithm",
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/= $design "confirmed no need of partial invalidation support \
			on event %Selected" @ "class %Button",
		/ "simplified button invalidation",
	),
	(
		/ "CRTP static polymorphic macros" @ "header %ybasemac.h",
		/ $design "simplified implementation" @ "header %(ydef.h, ybasemac.h,
			yevt.hpp)";
		/ "text renderers" ^ "CRTP" ~ "polymorphic classes"
	)
),

b265
(
	/ %'YFramework'.'YSLib' $=
	(
		+ "partial invalidation support for text rendering";
			// It makes efficiency decreased obviously \
				for non-overlapped widgets.
		/ %'GUI' $=
		(
			/ "window classes hierarchy" ^ "class %Panel",
			/ "refreshing algorithm" @ "class %Frame::Refresh" $=
			(
				+ "support for unbuffered windows",
				/ "minor efficiency improvement"
			),
			+ "member function %Panel::Refresh",
			+ "partial invalidation support"
				@ "class %(MLabel, TextList, Menu)",
			(
				* "sender checking missing for event %TouchMove"
					@ "class %TextList" $since b264;
				* "wrong behavior of listbox on event %TouchHeld when touching \
					widget changed" $since b219
			)
		),
		/ "intersection algorithm improvement for non-trivial result"
	)
),

b264
(
	/ %'YReader' $=
	(
		/ "format of time strings showed by information labels" @ "hexadecimal \
			browser" ^ "custom functions" ~ "function %std::ctime",
		* "file information box cannot be shown" @ "text reader" $since b263
	),
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ @ "class %ATrack"
		(
			* "scrolling overflow of value greater than about 0x1998"
				$since b260 $= (/ "value type" ^ "float"
				~ "ystdex::fixed_point<u32, 16>"),
			(
				/ "value mapping for scrollable length" ~ "total value range";
				* "wrong value calculation when minimum thumb length reached"
					@ "class %ATrack" $since b193
			),
			/ "GUI shell" ^ "temporary argument" @ "event %Enter",
			(
				* "wrong touch coordinate for event %TouchHeld when touching \
					widget changed" $since b219;
				* $comp "wrong behavior when the corresponding relative \
					touching coordinate component below zero on dragging"
					@ "class %ATrack" $since b219
			),
			/ $design "minor renderer interface"
		)
	)
),

b263
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		* "event %Confirmed checking fail for items out of initial view scope"
			$since b262,
		/ $design "rearranged rendering and updating implementation"
	),
	* $design "minor unexpected name pollution" @ "header %algorithm.hpp"
		@ "library %YStandardEx" $since b254,
	+ "information labels" @ "hexadecimal browser" @ "shells test example"
),

b262
(
	/ @ "class %TextList" @ "GUI" $=
	(
		* "wrong bottom adjustment of alignment" $since b261;
		* "wrong behavior on page selection" $since b191
	)
),

b261
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ $design "simplified interface" @ "class %ATrack";
		* "wrong alignment" @ "listbox when alignment is non-zero value \
			and scrolling down to end" $since b193
	)
),

b260
(
	/ %'%YBase' $=
	(
		+ $design "several operations for integer types",
		+ $design "reusable overloaded operators";
		+ "fixed-point arithmetic template %fixed_point"
	);
	* "improper underlying type of scroll event argument type"
		@ %'YFramework'.'YSLib'.'GUI' $since b201
),

b259
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		* "focusing pointer not removed when removing widgets"
			@ "class %(Panel, AFrame) $since b258;
		+ $design "subobject detaching of view class" @ "class %Widget",
		/ "simplified form class as typedef" ^ "class %Frame"
	);
	* "wrong behavior after pressing down exit button" @ %'YReader' $since b258
),

b258
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		- "focus responder runtime substituting capability",
		+ "widget view class %WidgetView";
		/ "unified focus requesting interface"
		/ "focusing state stored" @ "class %WidgetView"
	)
),

b257
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ $design "simplified entering/leaving event implementation",
		- $design "dependency events" @ "class %(ATrack, TextList)"
	),
	/ %'YReader' $=
	(
		/ %'hexadecimal browser' $=,
		(
			* "value of vertical scroll bar" @ "class %HexViewArea \
				not reset when file reloaded" $since b254,
			+ $design "individual model class and view class"
		)
	),
	- $design "suppout for language implementation without variadic macro";
	+ $design "void expression macros for function implementation"
),

b256
(
	* $design "undefined behavior when event arguments class not empty"
		@ %'YFramework'.'YSLib'.'GUI' $since b255,
	* "overloading error when using default template argument"
		@ "class template ExpandMemberFirstBinder" $since b171
	/ "first parameter for merged to second parameter" @ "event handling"
),

b255
(
	/ %'YReader' $=
	(
		/ %'hexadecimal browser' $=,
		(
			* "displaying of tail bytes less than one line" $since b253,
			+ "horizontal alignment controlling in displaying"
		)
	),
	* "wrong value construction" @ "class %TouchEventArgs"
		@ %'YFramework'.'YSLib'.'GUI' $since b195
),

b254
(
	/ $design "header %dependencies",
	/ %'YReader' $=
	(
		/ %'hexadecimal browser' $=,
		(
			+ "vertical scroll bar",
			+ "updating data for non-zero offset"
		)
	),
	+ $design "unsequenced evaluated expressions optimization" @ "widget class \
		constructors" @ "directory %YSLib::UI" @ %'YFramework'.'YSLib'.'GUI'
),

b253
(
	/ %'YReader' $=
	(
		+ "hexadecimal browser",
		+ $design "unsequenced evaluated expressions optimization"
			@ "unit %Shells",
		+ "automatic desktop invalidation when checkbox unticked"
	),
	* "wrong control %OnLostFocus behavior" @ %'YFramework'.'YSLib'.'GUI'
		$since b240,
	^ "new character types" $=
	(
		^ "fundamental types %(char16_t, char32_t) ~ %(std::uint16_t, \
			std::uint32_t) as basic types" @ "library %CHRLib",
		^ "literal syntax prefix u" ~ "macro _ustr"
	),
	* "size not refreshed when opening file excluded using constructor"
		@ "class %File" $since $before '~b1x'($with_timestamp 2009-12-01,
		$rev("yfile.cpp") = r221)
),

b252
(
	/ "libraries structure" $=
	(
		/ "project %YSLib" >> "%YFramework";
		/ "platform dependent library %YCLib" >> "%YFramework",
		/ "library %CHRLib" >> "%YFramework";
		/ "project %YCLib" >> "%YBase",
		/ "library %YStandardExtend" >> "%YStandardEx"
	);
	+ "POD type operations" @ "library %YStandardEx",
	/ "Doxygen file",
	+ $design "nested-use support" @ "macro %yunsequenced implementation",
	^ "updated devkitARM release 35" ~ "devkitARM release 34",
	* "implementation" @ "installation checking" $since b245
),

b251
(
	(
		+ $design "unsequenced evaluation macro %yunsequenced";
		+ $design "unsequenced evaluated expressions optimization"
			@ "library %(CHRLib, YCLib, YSLib)"
	),
	- "buffered text blocks",
	* "wrong ending of text checking @ text buffer" $since b246,
	/ @ "library %CHRLib" $=
	(
		/ "undereferencable conversion error treated as conversion faliure",
		/ "conversion functions returns non-zero if non-zero bytes read \
			when a conversion failure occured"
	)
),

b250
(
	+ $design "iterator checking operations" @ "library %YCLib";
	/ @ "library %CHRLib" $=
	(
		+ "valid conversion state support for %CharSet::UTF_8 \
			to %CharSet::UCS2 mapping function",
		+ "invalid conversion state support for %(CharSet::UTF_8, \
			CharSet::GBK) to %CharSet::UCS2 mapping function",
		+ "conversion state support for %(CharSet::UTF_16LE, \
			CharSet::UTF_16BE) to %CharSet::UCS2 mapping function"
	);
	* "EOF cannot be recognized by conversion routines" $since b248
),

b249
(
	/ @ "library %CHRLib" $=
	(
		/ "encoding item names with MIB enums from IANA" @  $=
		(
			+ "more enum items",
			/ "aliases"
		),
		+ "conversion state formal parameter support for multibyte-to-Unicode \
			conversion functions";
		+ "valid conversion state support for %CharSet::GBK to %CharSet::UCS2 \
			mapping function"
	),
	/ $design "exception macros" $=
	(
		/ $design "exception specification macro" @ ("header %config.h"
			@ "library %YSLib::Adaptor") >> ("header %ydef.h"
			@ "library %YCLib::YStandardExtend");
		* $design "macro (ythrow, ynothrow) used without definition"
			@ "library %YStandardExtend" $since b209;
	)
),

b248
(
	/ $design "deleted copy constructor" @ "class %input_monomorphic_iterator \
		for safety",
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		* "wrong return value" @ "member function %Refresh" @ "class %(Widget,
			Label, Progress, TextArea, Control, AWindow)" $since b226,
		* wrong overlapping condition @ (function %RenderChild,
			member function %Frame::DrawContents) $since b226;
		* "wrong invalidation on thumb of tracks after performing small \
			increase/decrease" $since b240
	)
),

b247
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ $design "common widget member operation function %CheckWidget";
		/ $design "simplified scrolling widget implementation"
			^ "function %CheckWidget"
	),
	+ "statically-typed object proxy class %void_ref";
	+ "input iterator adaptor class %monomorphic_input_iterator";
	^ $design "simplified isomorphic character converter prototypes"
		^ "class %monomorphic_input_iterator"
),

b246
(
	+ $design ^ "selectable C++11 generalized constant expressions features",
	/ "file classes" $=
	(
		+ "mode selecting at opening files",
		/ "opening files" ^ "binary mode as default" @ "class %File";
		/ "initializing text files" ^ "text mode" @ "class %TextFile"
	),
	/ (- "automatic Unix style newline conversion at loading files")
		@ "simplified text file buffering",
	+ "file information panel" @ %'YReader',
	* "character mapping for GBK" @ "library %CHRLib" $since b245
),

b245
(
	/ $design "interfaces" @ "library %CHRMap",
	/ $design "integer type definitions and string utilities"
		@ "library %(YCLib)",
	* "C-style string allocation" @ "library %CHRMap" $since $before
		'~b10x'($with_timestamp 2010-05-30, $rev("chrproc.cpp") = r1525),
	* $design "order of YCLib/YSLib in library linking command"
		@ "ARM9 makefile" $since b187;
	* "character mapping functionality implementation" @ "library %CHRLib"$=
	(
		* "unspecified subexpression evaluation order"
			@ "implementation unit chrmap.cpp",
		* "wrong behavior of mapping functions for platforms where char \
			is a signed type"
	) $since before '~b4x'($with_timestamp 2009-11-22, $rev("chrproc.cpp")
		= r1319),
	+ "encoding conversion from UTF-8 to UCS-2" @ "library %CHRLib";
	* "path with non-ASCII characters cannot send to reader" @ %'YReader'
		$since b141
),

b244
(
	/ %'YReader' $=
	(
		/ "reader panel functionality",
		/ "more text file extensions supported"
	),
	* "unsafe nullable dependency object" $since b242
		$= (- "nullable features" @ "class template %GDependency"),
	* "path with non-ASCII characters wrongly displayed" $since b141,
	* "wrong value of unnamed namespace constant member FS_Parent_X"
		@ "file yfilesys.cpp" $since b156
),

b243
(
	/ $design "simplified class inheritance" @ "shell and application classes";
	- $design "inheritance GMCounter<Message>" @ "class %Message";
	- "class %(YCountableObject; YObject)";
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ "default GUI event %Paint for all widgets",
		+ "controller class %WidgetController for widgets which %Paint is the \
			only event permitted to call";
		/ "rendering of function %Render" ^ "mutable rvalue reference \
			parameter to store the result" ~ "returning";
		+ "event handler %Render" @ "constructor" @ "%WidgetController";
		/ "rendering logic" @ "member function %Frame::DrawContents"
			^ "event %Paint" ~ "directly call function %Render";
		/ $design "simplified updating" @ "renderer classes",
		/ $design "simplified refreshing" @ "class %TextList"
	)
),

b242
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ "class %PaintEventArgs",
		/ $design "simplified painting parameters" ^ "class %PaintEventArgs",
		- "automatic lifetime binding to containers" @ "class %Frame"
	),
	+ "nullable features" @ "class template %GDependency",
	/ "simplified event mapping" $=
	(
		+ "class template %GEventPointerWrapper",
		^ "non-member algorithm implementation" ~ ("member function"
			@ "class template %GEventMap");
		- "class template %GEventMap"
	),
	/ $design ^ "lambda expressions" ~ "most of member function handlers"
		@ %'YReader',
	/ "simplified screen object interface and implementation" $=
	(
		/ "class template %GBinaryGroup" ~ "class %(BinaryGroup, Point, Vec)",
		/ ("member function %(GetPoint, GetSize) return const references"
			~ "object type values") @ "class %Rect";
		/ ("platform independent const static member %Invalid"
			@ "(class template %GBinaryGroup, class %Size)")
			~ ("platform dependent %FullScreen" @ "class %(Point, Vec, Size)"
	)
),

b241
(
	+ "new macros to simplify defaulted or deleted constructors \
		and destructors definition" @ "header %ybase.h",
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ $design "%(MoveConstructible) support for all widget classes",
		+ $design "class %IController as controller interface type"
	),
	/ "event mapping interfaces" $=
	(
		+ "pointer type values confined as non-null",
		/ "member function %at returns %ItemType&" ~ "%PointerType&",
		/ $design "member function template %(GetEvent, DoEvent)"
			@ "class template %GEventMap" >> "unit %(YWidgetEvent, YControl) \
			as non-member"
	),
	* "contradict semantics of member template is_null" @ "header %memory.h"
		@ "library %YCLib" $since b222
),

b240
(
	/ "macros" @ "header %ybase.h" $=
	(
		/ $design "simplified macro parameter names",
		+ "new macros for template declarations and forwarding constructor \
			templates",
		+ "macro %DefClone(_t, _n) for define member function for cloning"
	),
	+ "support for types are not %EqualityComparable" @ "class %ValueObject"
		^ "always-be-equal strategy",
	+ $design "copy constructor" @ "class template %GEventMap",
	+ $design "const static reference getter function %GetPrototype"
		@ "header %ystatic.hpp",
	* $design "member function %Insert unavailable for %unique_ptr"
		@ "class template %GEventMap" $since b221,
	+ $design "%CopyConstructible, %MoveConstructible support"
		@ "class template %GEventMap",
	+ $design "%CopyConstructible and %MoveConstructible support"
		@ "class %(BitmapBuffer, BitmapBufferEx)";
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		* @ "class template %GFocusResponser" $since b239
			$= (+ "default constructor"),
		+ "virtual member function %Clone" @ "renderers, focus responsers \
			and widget classes",
		+ $design "prototype constructing of widgets" $=
		(
			+ $design "%(CopyConstructible, MoveConstructible) and clone \
				support" @ "class %(WidgetRenderer, BufferedWidgetRenderer)",
			+ $design "%(CopyConstructible, MoveConstructible) and clone \
				support" @ "class template %(GFocusResponser, \
				GCheckedFocusResponser)",
			+ $design "%(CopyConstructible, MoveConstructible) and clone \
				support" @ "class %WidgetController";
			+ $design "%CopyConstructible and %MoveConstructible support"
				@ "class %(Widget; Control)",
			+ "class %ControlEventMap" @ "class %Control"
		)
	)
),

b239
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ "dynamic focus responser switching",
		/ "simplified" @ "class %IWidget and derived classes",
		+ "common focus APIs shared by class %IWidget"
			~ "class %(Panel, AFrame, AUIBoxControl)" $=
		(
			+ "function %ClearFocusingPtrOf",
			+ "function %FetchFocusingPtr"
		)
	)
),

b238
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ "simplified" @ "class %IWidget",
		/ $design "controller pointer" @ "class %Control" >> "class %Widget",
		/ "simplified" @ "class %Control",
		/ "simplified implementation" @ "focus responsers"
	)
),

b237
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ "control functionality for widgets" ~ "for controls" $=
		(
			/ "support of events",
			/ "support of focusing",
			/ "enablity for all widgets"
		)
		/ "support of moving widget to the top" @ "class %MUIContainer"
			~ "class %Desktop";
		- "class %IControl",
		/ $design "member function %GetTopWidgetPtr" $=
		(
			- "containing test",
			+ "predicate parameter"
		),
		+ "automatic canceling pressing state when refreshing" @ "class %Thumb"
	)
),

b236
(
	^ "updated libnds 1.5.4 with default arm7 0.5.23"
		~ "libnds 1.5.1 with default arm7 0.5.21",
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ "controllers",
		/ "simplified %IControl interface",
		+ $design "rvalue forwarding and return value support"
			@ "function template %CallEvent",
		+ "limited focusing interfaces" @ "class %IWidget",
		/ "namespace %(Widgets, Controls, Forms) merged to parent \
			namespace %YSLib::Components",
		/ "simplified implementation and improved efficiency \
			in focus requesting" $=
		(
			^ $design "class %IControl" ~ "class %AFocusRequester"
				@ "member functions" @ "controls",
			- $design "class %AFocusRequester"
		)
	)
),

b235
(
	/ %'YReader' $=
	(
		/ "simplified background images loading",
		+ "reader panel" $=
		(
			+ "close button",
			+ "direct reading command controls"
		)
	),
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		- $design "redundant refreshing" $since b226
			@ "efficiency improved" @ "member function %ATrack::Refresh",
		/ "GUI shell" $=
		(
			/ "a little improvement of efficiency"
				@ "entering and leaving events handling"
		),
		/ "pointer to containers as top elements returned accepted \
			and treated as null pointers",
		+ "label text display supporting for non-direct contained widgets",
		/ "invalidating canceled when enablity not changed"
			@ "function %SetEnableOf" @ "unit %YControl"
	)
),

b234
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ "overwritable item enablity policy support" @ "class %TextList",
		/ "item enablity support" @ "class %Menu",
		* $design "recursively self call" @ "Control::Refresh"
			$since b230
	),
	/ %'YReader' $=
	(
		+ "checkbox to switch FPS visibility",
		- "checkbox to switch enablity of button",
		* "lost mapped global resource release call" $since b233,
		+ "empty panel test"
	)
),

b233
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ "partial invalidation for hosted menus",
		/ "partial invalidation for member function %Desktop::MoveToTop",
		/ "partial invalidation for class %TextArea"
	),
	/ %'YReader' $=
	(
		/ "key input response" ^ "direct routing strategy"
			~ "all routing strategy",
		/ "menu of reader",
		+ "mapped global resource management"
	),
	/ $design "enhancement" @ "class %ValueObject"
	(
		+ "mutable access",
		+ "empty predicate",
		+ "constructing by pointers"
	)
),

b232
(
	/ %'YReader' $=
	(
		/ "background images" @ "class %ShlExplorer",
		/ $design "simplified reader refreshing implementation",
		- $design "member function ShlProc"
			@ "class %(%ShlExplorer, ShlReader)",
	),
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		* "invalid parent menu displayed on confirming submenus" $since b231,
		/ "menus functionality" $=
		(
			+ "hiding unrelated non-parent menus"
		),
		* "GUI class member function %ClearFocusingPtr implementation"
			$since b194 $=
		(
			@ "class %AFrame" $since b194,
			@ "class %Panel" $since b201,
			@ "class %AUIBoxControl" $since b194
		)
	),
	/ $design "simplified shell classes" $=
	(
		/ "implementation" @ "shell message processing" @ "class %ShlDS"
			^ "default sending of %SM_PAINT message when processing %SM_INPUT"
	)
),

b231
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ "updating with limited area" $=
		(
			+ "updating with invalidated area"
				@ "class %BufferedWidgetRenderer",
			+ "empty implemented updating with invalidated area"
				@ "class %WidgetRenderer",
			+ "updating with limited area" @ "function %Widgets::Update",
			* "wrong updating for (at least 3) buffered widgets overlapping"
				$since b228
		),
		+ "free function %SetInvalidationOf for class %IWidget"
			~ "member function AWindow::SetInvalidation"
	),
	/ $design "style of free setters/getters naming"
),

b230
(
	+ $design "move constructors and move assignment operators"
		@ "class template %(pseudo_iterator, pair_iterator)"
		@ "header %YCLib::YStandardExtend::Iterator",
	/ $design "simplified GUI" $=
	(
		- "paint event for controls",
		- "DrawControl methods"
	),
	/ %'YReader' $=
	(
		/ "class %FPSCounter declared as external linkage in header",
		/ "updating FPS" $=
		(
			+ "when not getting input",
			/ $design "implementation" ^ "shell method %UpdateToScreen"
				~ "%OnActivated"
		)
	)
),

b229
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ "simplified controls rendering implementation" $=
		(
			- "widget boundary drawing APIs",
			/ "control drawing" @ "class %(TextList, CheckBox)"
		),
		/ "simplified APIs" $=
		(
			- "locating functions concerned with interface %IWindow",
			- "interface %IWindow inheritance" @ "class %AWindow",
			- "interface %IWindow"
		),
		+ "dynamic renderer switching"
	)
	/ %'YReader' $=
	(
		+ "buffered renderer for listbox",
		^ "namespace %::YReader" ~ "namespace ::YSLib"
	),
	- "using namespace %platform" @ "namespace DS" @ "header %yadaptor.h"
),

b228
(
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		+ "runtime buffering control" $=
		(
			+ "renderer class %(BufferedWidgetRenderer, WidgetRenderer)",
			/ "window buffering control moved to renderer" ~ "class %Frame"
		),
		/ "buffering-concerned methods" @ "class %IWindow"
			>> "renderer classes and class %IWidget"
	)
),

b227
(
	(
		* "wrong result when one argument actually contained by another"
			@ "intersection calculation for class %Rect" $since b226;
		* "invalidation area error" $since b226
	),
	/ "simplified background rendering" @ "class %ScrollableContainer"
		@ "widget rendering efficiency improvement",
	* $design "guard macro" @ "header %UIContainerEx" $since b203,
	/ "simplified UI class inheritance" $=
	(
		/ $design "implementation" ^ "class %IWidget" ~ "class %IUIBox",
		- "class %IUIBox",
		- "class %IUIContainer",
		/ $design "implementation" ^ "class %IControl" ~ "class %IPanel",
		- "class %IPanel"
	),
	* @ "strict ISO C++2003 code compatibility" $since b190
		$= (^ "fixed macros" ~ "variadic macros" @ "header %(YPanel, YWindow)"),
	/ "simplified widget fetcher and locating interfaces"
),

b226
(
	/ "widget rendering efficiency improvement" $=
	(
		+ "return value of actually drew area for member function %Refresh"
			@ "widgets",
		- "unnecessary drawing of overlaid windows" @ "windows refreshing",
		+ "partial refreshing optimization" @ "class %Widget"
	),
	* "wrong result when height of arguments are equal"
		@ "intersection calculation for class %Rect" $since b225,
	+ "child widget intersection confirming"
),

b225
(
	/ "windows partial invalidation support" $=
	(
		/ "windows partial invalidation committing for class %IWindow"
			~ "class %AWindow",
		+ "widget validation abstract member function" @ "class %IWidget",
		/ "widget invalidation interface as non-member function"
			~ ("abstract member function" @ "class %IWindow")
	),
	+ $design "assignment operators" @ "class %Rect",
	/ "widget rendering efficiency improvement" $=
	(
		+ "parameters for member function %Refresh" @ "widgets",
		/ "background refreshing" ^ "invalidation" @ "class %AWindow",
		/ "without validating old invalidated areas" @ "windows invalidating",
		+ "partial invalidation" @ "control default %TouchMove event handler \
			%OnTouchMove_Dragging"
	),
	+ "intersection and union calculation for class %Rect",
	/ %'YReader' $=
	(
		+ "opaque background" @ "pseudo-frame-per-second counter",
		/ "controls layout" @ "shell class %ShlExplorer",
		- "windows update state"
	)
),

b224
(
	* "wrong ascending when switching font size" $since b224
		$= (^ "freetype 2.4.4 cache system" ~ "freetype 2.4.5 cache system"),
		// It seems a new bug in freetype 2.4.5 cache system.
	+ "containing test" @ "class %Rect",
	+ $design "WinGDB settings" @ "main project file",
	+ "windows partial invalidation support" $=
	(
		/ "invalidation interface",
		+ "windows partial invalidation committing for class %AWindow"
	)
),

b223
(
	/ "DS painting" ^ "message %SM_PAINT" @ %'YFramework'.'YSLib'.'GUI'
			~ "directly calling of %ShlDS::UpdateToScreen",
	* "uncleared application message queues on program exit" $since b174,
		// This might cause memory leaks.
	/ %'YReader' $=
	(
		+ "pseudo-frame-per-second counter",
		/ "button enabling" ^ "file extension matching in the file box"
	),
	* "declaration of function %GetStemFrom" @ "header %yfilesys.h" $since b161,
	/ "updated library freetype" $=
	(
		^ "updated freetype 2.4.5" ~ "freetype 2.4.4",
		+ "exact bounding box calculation",
		+ "path stroker",
		+ "support for synthetic emboldening and slanting of fonts",
		- "obsolete header %/freetype/internal/pcftypes.h" $since b185
	),
	^ "updated libnds 1.5.1 with default arm7 0.5.21"
		~ "libnds 1.5.0 with default arm 7 0.5.20",
	^ "updated devkitARM release 34" ~ "devkitARM release 33"
),

b222
(
	* $design "minor faults detected by CppCheck" $=
	(
		/ "simplified implementation" @ "%YGUIShell::ResponseTouch" $since b195,
		* "implementation" @ "%ystdex::fexists" $since b151
	),
	+ "default GUI event %Paint",
	/ "controls drawing" ^ "event Paint" ~ "member function Draw"
		@ "class %(Frame, YGUIShell)",
	/ "GUI controls enhancement" $=
	(
		+ "key events for text list controls including menu" $=
		(
			+ "state-based cyclic traversing" @ "class %TextList",
			+ "%TouchDown %KeySpace::Up and %TouchDown %KeySpace::Down \
				of cyclic traversing",
			+ "%TouchDown %KeySpace::Up and %TouchDown %KeySpace::Down \
				of initializing selection when unselected"
		)
	),
	/ %'YReader' $=
	(
		+ "menu" @ "class %ShlReader",
		^ "widget drawing" ^ "direct desktop background drawing"
	),
	+ "controls: buffered text area"
),

b221
(
	/ %'YReader' $=
	(
	`	/ "simplified direct-painting test",
		/ "simplified key-to-touch event mapping implementation"
			@ "class %Control"
	),
	^ "updated devkitARM release 33" ~ "devkitARM release 32",
	^ "updated libnds 1.5.0 with default arm7 0.5.20"
		~ "libnds 1.4.8 with default arm 7 0.5.17",
	/ $design "header search path of VS2010 projects",
	/ "event map interfaces" $=
	(
		+ "sender type as template parameter type",
		/ "simplified sender argument forwarding"
		/ "more efficient implementation"^ "%unique_ptr" ~ "%shared_ptr",
			// Old version of libstdc++ might fail in compiling \
				due to members with parameter of rvalue-reference type \
				are not supported.
	),
	* "lost move constructor" @ "class template %GMCounter" $since b210,
	/ "class %HDirectory" ^ "POSIX dirent API" ~ "libnds-specific API",
	+ $design "diagnostic pragma for GCC 4.6.0 and newer"
		@ "header %type_op.hpp",
	* "class %HDirectory state not restored during operations" $since b175,
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ "controls key-to-touch event mapping" @ "class %Control" $=
		(
			/ "custom handler" ^ "std::function" ~ "member function",
			+ "%KeyPress to %Click event mapping methods" @ "class %Control",
			* "calling mapped to disabled control" $since b217
		),
		/ "input response" $=
		(
			* "%YShell::ResponseTouch wrong behavior for container not \
				locating at (0, 0)" $since b213,
			* "%YShell::ResponceKey wrong behavior for tunnel event for \
				container" $since b199
		),
		/ "menus functionality" $=
		(
			+ "key events for submenus" $=
			(
				+ "%TouchDown %KeySpace::Left for hiding",
				+ "%TouchDown %KeySpace::Right for showing"
			),
			+ "automatically selecting when submenu shown using keypad"
		),
		/ "sequence viewer" $=
		(
			/ $design "default argument" ^ "unsigned integral type"
				~ "signed integral type" @ "class template %GSequenceViewer",
			+ "restricted value setting",
			* "assertion failed in containing test when the length is zero"
				$since b171
		)
	)
),

b220
(
	+ "non-focused textlist border",
	/ "control focus interfaces" $=
	(
		+ "specified %IControl reference as sender parameter"
	),
	/ "menus functionality" $=
	(
		* "menu hiding when submenu shown and focus lost" $since b214,
		+ "automatically locating submenu when shown",
	),
	* "wrong text margin of widgets not entirely shown in the window"
		$since b190
),

b219
(
	/ $design "unit %YGUI decomposed for Blit and 2D API",
	/ "optimized widget-to-widget locating",
	/ $design "font cache moved to platform-dependent application class from
		class %YApplication",
	+ "several global helper functions as platform-independent interface",
	/ %'YReader' $=
	(
		+ "multiple background switch test"
	),
	* "wrong default argument of desktop background color" $since b160,
	+ "widgets: progress bar" @ "class %ProgressBar"
),

b218
(
	/ %'YReader' $=
	(
		+ "desktop background switch test"
	),
	+ $design "ownership tag structure",
	- $design "global resource helper functions",
	+ "DS screen class",
	^ "class %YMainShell for resource loading" ~ "class %ShlLoad",
	- "class %ShlLoad"
),

b217
(
	* "wrong lost focus behavior for menus \
		which had popped submenu(s) previously" $since b214,
	/ "improved compatibility" @ "class %GHEvent for types \
		not met EqualityComparable requirement interface \
		including closure types" $=
	(
		/ "result always true",
		* "compile-error for non-trivially copy-assignable objects",
		/ "optimized implementation to avoid bloat code instantiated"
	),
	+ $design "Code::Blocks project files" $=
	(
		+ "workspace file",
		+ "project files"
	),
	/ $design ^ "lambda expression" ~ "several private member function \
		as event handlers",
	+ "%KeyDown and %KeyHeld handlers for scroll bars \
		to perform mapped touch behaviors",
	+ "key-to-touch event mapping methods" @ "class %Control",
	(
		* "wrong coordinate passed to touch focus captured control" $since b195;
		* "wrong behavior" @ "track touch held event handler" $since b213
	),
	+ $design "cscope files"
),

b216
(
	/ $design "header %file dependencies",
	/ "focused boundary for controls",
	^ $design "using unnamed namespace" ~ "keyword static"
		@ "unit %YApplication",
	/ "more efficient message queue implementation" ^ "%multiset"
		~ %priority_queue",
	/ "simplified shell example",
	+ "default windows Z order",
	+ "message fetching automatically calling %Idle \
		when size less or equal than a user-specified limit"
),

b215
(
	+ "disabled state style of control %Thumb",
	* "disabled control touch event handling" $b199,
	/ + "checkbox and disabling test" @ %'YReader',
	+ "visible arrow indicating multi-level menus",
	/ "creating list" @ ("constructor of class %MTextList"
		~ "member function %GetList"),
	+ "event handler supporting for types not met EqualityComparable
		requirement interface including closure types",
	* $design "access of interitance of class std::function"
		@ "class template %GHEvent" $since b207,
	+ "disabled behavior test" @ "%ShlExplorer" @ %'YReader',
	/ "thumb/button boundary style",
	/ "global architecture" $=
	(
		/ "screen constant" >> "namespace scope" ~ "class %Global",
		+ "class %YDSApplication" ~ "class %Global",
		- "singleton mechanism" @ "class %YApplication"
	)
),

b214
(
	/ "functions of menus" $=
	(
		+ "hiding on confirmed",
		+ "resizing with width of text in list",
		/ "margin of menus",
		+ "submenus"
	),
	* $design "exception specification" @ "unit %YApplication",
	/ $doc $=
	(
		* "template parameter description" $since b189
			$= (^ "\tparam" ~ "\param"),
		* "\defgroup description spell error" $since b209,
		* $design "operator new & delete comments" $since b203,
		/ "simplified Doxygen file excluded paths"
	),
	/ $design ^ "public %noncopyable inheritance"
		~ "all private %noncopyable inheritance",
	* "point containing test for zero width or height rectangle \
		turned out assertion failure" $since b204,
	* "constness of text width measuring" $since b197,
	/ (- "unused parameter and argument") @
		"simplified focus operations interface"
),

b213
(
	* $design "UI assertion strings",
	* "menu colors",
	* "touch event coordinate error" @ "container controls" $since b195,
	+ "functions of menus" $=
	(
		+ "resizing for content",
		+ "hiding when focus lost"
	),
	+ "predicator %Contains" @ "UI container implementation"
),

b212
(
	/ $design "assertion strings",
	/ "menu constructor",
	+ "redundant menu state member" @ "class %Menu",
	/ "container member APIs" ^ "reference parameter" ~ "pointer parameter",
	* "constructor of class %Frame",
	+ "Z order for widget overlapping" @ " UI containers",
	+ "menu laid at top level" ^ "Z order"
),

b211
(
	/ "implemented messages with general object content holder"
		^ "non-pointer member" ~ "%shared_ptr",
	+ "message content mapping",
	/ $design "messaging APIs" >> "unit %YApplication" ~ "unit %YShell",
	/ "test menu fixed on the desktop" @ %'YReader'
),

b210
(
	+ "template %is_valid for checking pointers which cast bool explicitly",
	/ "part of template %raw moved to namespace %ystdex",
	- $design "some unused code",
	+ "macro %yforward for simply using std::forward and argument \
		type deduction",
	/ "handles and smart pointers rearranged" $=
	(

		- "%GHWeak",
		^ "%std::shared_ptr" ~ "%GHandle",
		- "%GHandle"
	),
	/ "implementation of messages" $=
	(
		/ "implementation of class %Message",
		- "message ID mapping"
	)
),

b209
(
	/ "merged library CHRLib" >> "library %YCLib",
	/ $design "protected function inheritance" @ "class template %GHEvent",
	/ "using directive of namespace %ystdex" @ "library %YSLib",
	+ "lost %Rect operations",
	* "strict ISO C++2003 code compatibility" $since b190
		$= (^ "fixed macros" ~ "variadic macros" @ "header %YFont"),
	/ "renamed directory %Shell to %UI @ "library %YSLib",
	/ "several memory utilities for std::shared_ptr and std::unique_ptr"
		>> "library %YCLib::YStandardExtend"
),

b208
(
	^ "rvalue references as parameter types of event handlers",
	/ "set default font size smaller",
	+ "vertical alignment" @ "labeled controls",
	* "fatel error" @ "direct UI drawing test"
),

b207
(
	/ $design "event handler implementation ^ std::function" ~ "Loki::Function",
	- "library %AGG",
	- "library %Loki",
	+ $design "union %no_copy_t and union %any_pod_t for suppressing \
		static type checking",
	+ $design "polymorphic function object template and base class",
	- "single-cast event class template",
	^ "rvalue reference" @ "event class",
	* "ystdex algorithms",
	* "minor event handler parameter type mismatch" @ %'YReader',
	* "key events response(added as %KeyDown, but removed as %KeyPress)"
		@ "project YSTest_ARM9" $since b201,
	* "tunnel and direct key and touch response repeated" @ "class %YGUIShell"
		$since b198
),

b206
(
	+ "menu test button",
	/ "file API",
	* "strict ISO C++2003 code compatibility" $since $before 132
		$= (+ "function %memcmp declaration" @ "namespace %ystdex"),
	* "strict ISO C++0x code compatibility" $=
	(
		* "implicit narrowing conversion(N3242 8.5.4/6)"
			@ "ISO C++0x(N3242 5.17/9)" ^ "explicit static_cast",
		/ "character types" @ "header %platform.h"
	),
	/ "coding using limited C++0x features" $=
	(
		/ $design ^ "C++0x style nested template angle brackets",
		/ $design ^ "keyword %auto",
		/ ^ "C++2011 type_traits" @ "namespace std" ~ "std::tr1"
	),
	- "Loki type operations",
	/ ^ "namespace %ystdex" @ "namespace %YSLib"
),

b205
(
	/ "simplified widgets and controls inheritance",
	/ "simplified image resource type",
	/ $design "protected inheritance of module classes except \
		%MScreenPositionEventArgs",
	/ "listbox highlight text color"
),

b204
(
	/ "weak handle improvement",
	/ $design "exception specification macro",
	- "window handles" $=
	(
		- "type %HWND",
		^ "%IWindow*" ~ "%HWND"
	),
	* "track background painting ignored" $since b191,
	+ "class %nullptr_t" @ "namespace ystdex",
	^ "nullptr at YSLib",
	/ $design "widgets virtual member functions"
),

b203
(
	/ $design "units rearrangement",
	/ $design "global architecture" $=
	(
		+ "global instance function",
		- "all global objects",
		- "platform dependent global resource" @ "class %YApplication",
		- "global object" @ "unit %YNew"
	),
	+ "class %MTextList",
	/ "class %YSimpleText List using class %MTextList inheritance",
	/ "class %YSimpleText renamed to %YMenu",
	/ "using std::tr1::shared_ptr" ~ "smart pointers" @ "library %Loki"
),

b202
(
	/ $design "unit %renaming",
	/ "improved windows painting efficiency",
	* "buffered coordinate delayed in painting dragged control" $since b169
),

b201
(
	/ "focused button style",
	+ "key holding response" @ "class %ShlReader",
	+ "GDI API %BlitTo",
	/ "widgets drawing",
	/ $design "GDI API %CopyTo",
	+ "controls: panel",
	/ $design "YCLib" $=
	(
		+ "partial version checking for compiler and library implementation",
		+ "minor macros" @ "library %YCLib",
	),
	+ "type conversion helper template",
	+ $design "implicit member overloading by interface parameter with type \
		%IControl and %IWidget" @ "container classes"
),

b200
(
	* "direct UI drawing test",
	/ "test UI view",
	* "%KeyHeld response" $since b199,
	+ "direct desktop drawing for %ShlSetting"
),

b199
(
	+ "event routing for %(KeyUp, KeyDown, KeyHeld)",
	* "event behavior with optimizing" $since b195,
	+ "keypad shortcut for file selector",
	+ $design "returning number of called handles in event calling"
),

b198
(
	* "font caching without default font file load successfully",
	+ "showing and hiding windows",
	* "%ListBox scroll bar length",
	* "handle constructors",
	- "class %ShlGUI" @ "unit %Shell_DS" @ $design "shells for DS",
	/ %'YFramework'.'YSLib'.'GUI' $=
	(
		/ $design "using pointers" ~ "references in parameters \
			of container methods",
		/ "simplified GUI shell" $=
		(
			/ "input points matching",
			- "windows list"
		)
	),
	/ $design "simplified destructors",
	/ "simplified window drawing",
	+ "desktop capability of non-control widget container",
	- "container pointer parameter" @ "constructor widgets",
	/ "desktops as window" @ "shells"
),

b197
(
	* "label alignment",
	+ "%std::string based font cache"
),

b196
(
	+ "controls: checkbox",
	* "platform color type",
	+ "horizontal text alignment" @ "class %MLabel"
),

b195
(
	* $design "makefiles",
	+ "dependency events",
	+ "simple events routing"
),

b194
(
	/ "global architecture",
	/ "GUI shell class architecture" @ "unit %YGUI",
	/ "default control event handlers implementation",
	/ "implementation" @ "unit %YInitialization",
	/ "controls always visual",
	- "class %IVisualControl",
	- "class %VisualControl",
	/ "container focus" @ "class %(AUIBoxControl, AWindow)"
),

b193
(
	/ "scroll controls implementation",
	/ "core utilities"
	/ "empty event arguments" $=
	(
		- "empty constant of event arguments",
		+ "inline function template GetStaticRef",
		/ "all event empty arguments" ^ "GetStaticRef"
	),
	/ "scroll controls and listbox implementation"
),

b192
(
	+ "empty constant of event arguments",
	/ "event interfaces",
	/ $design "header %including",
	+ "unit %YUIContainerEx" @ "directory Shell",
	+ "class %AUIBoxControl" @ "unit %YUIContainerEx",
	+ "controls: class %YScrollableContainer" @ "unit %YGUIComponent",
	/ $design "function %FetchWindowPtr as non-inline" @ "unit %YUIContainer",
	/ "class %IWindow as non-virtual inheritance" @ "class %AWindow"
		@ "class %YWindow",
	/ "arm9 makefile",
	/ "scroll controls implementation"
),

b191
(
	/ "listbox APIs implementation",
	/ "class template %GSequenceViewer" $=
	(
		* $design "declaration of member function %Contains" @ $since b171,
		/ "interfaces"
	),
	* "implementation of function %GetTouchedVisualControlPtr" @ "unit %YGUI"
		$since b167,
	/ "simplified focus implementation"
),

b190
(
	* "fixed macros" ~ "variadic macros" @ "strict ISO C++2003 code \
		compatibility" $since $before b132,
	/ "text region and renderers APIs implementation",
	/ "widget, label and listbox APIs implementation"
),

b189
(
	+ "class %pair_iterator" @ "YCLib",
	/ "GDI blit and transformer implementations"
),

b188
(
	/ "shells test example",
	/ "GDI blit implementations",
	- "DLDI patch commands" @ "makefile",
	* "invalid listbox input when the list not beginning from the top"
		$since b171
),

b187
(
	/ $design "header %files and source files put in separated directories"
		@ "solution configuration",
	^ "-O3 & arm specified options compiled library libloki.a"
		@ "project YSTest",
	+ "minor templates" @ "YCLib",
	/ "GDI blit implementations"
),

b186
(
	/ "solution configuration",
	* "makefiles",
	/ $design "file including",
	* "some wrong Doxygen comments"
),

b185
(
	^ "updated freetype 2.4.4" ~ "freetype 2.3.12",
	- "DMA implantation" $=
	(
		/ "remarked",
		^ "normal %memcpy and %memset"
	)
	/ "desktop window refreshing logic",
	/ $design "MDualScreenReader constructor" ^ "%ynew" ~ "%new"
),

b184
(
	* $design "some spell errors",
	/ "DMA operations implementation"
),

b170_b183
(
	/ "GUI"
	(
		+ "controls: tracks",
		+ "controls: scroll bars";
		+ "controls: scrollable container",
		+ "controls: listbox"
	)
),

b159_b169
(
	+ "controls: buttons" @ "class %(YThumb, YButton)",
	+ "controls: listbox class",
	+ "event mapping"
),

b134_b158
(
	+ $design "core utility templates",
	/ "smart pointers",
	+ "GUI focus",
	+ "shells" @ %'YReader' $=
	+ "controls",
	+ "virtual inheritance" @ "control classes",
	+ "exceptions",
	+ "debug macros & functions",
	+ "color type",
	+ "class template %general_cast",
	+ "timer class"
),

b133
(
	/ $design "simplified primary types definition",
	/ "event interfaces",
	+ "several except classes",
	/ "exception handling" @ "some functions"
),

b132
(
	/ "log interfaces",
	+ "backup message queue object" @ "the application class",
	/ "automatically clearing screen windows when deactivating class %ShlGUI",
	+ "background image indexing" @ %'YReader'
),

b1_b131
(
	+ "initial test with PALib & libnds",
	+ "shell classes",
	+ "CHRLib: character set management",
	+ "fonts management using freetype 2",
	+ "YCLib: platform independence",
	+ "the application class",
	+ "basic objects & counting",
	+ "global objects",
	+ "string class",
	+ "file classes",
	+ "dual screen text file reader framework",
	+ "output devices & desktops",
	+ "messaging",
	+ "program initialization",
	+ "simple GUI: widgets & windows",
	+ "simple GDI",
	+ "base abbreviation macros",
	+ "events",
	+ "smart pointers using Loki",
	+ "Anti-Grain Geometry test",
	+ "simple resource classes"
);


//---- temp code;
	// TODO: remove '*printf';

#endif
