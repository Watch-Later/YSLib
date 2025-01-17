﻿/*
	© 2018-2022 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file memory_resource.h
\ingroup YStandardEx
\brief 存储资源。
\version r1570
\author FrankHB <frankhb1989@gmail.com>
\since build 842
\par 创建时间:
	2018-10-27 19:30:12 +0800
\par 修改时间:
	2022-07-10 17:57 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YStandardEx::MemoryResource

提供 ISO C++17 标准库头 \c \<memory_resource> 兼容的替代接口和实现。
除了部分关系操作使用 operators 实现而不保留命名空间内的声明及散列支持提供偏特化外，
其它接口同 std::pmr 中的对应接口。
原始设计主要来自 Library fundamental TS ，参见 WG21 P0220R1 。
因为一些兼容问题，std::experimental::pmr 中的接口不被可选地使用，
	即使其可用性仍然会被检查。
和原始的 std::experimental::pmr 中提供的接口及其实现有以下不同：
LWG 2724 ：纯虚函数为 private 而不是 protected 。
LWG 2843 ：成员 do_allocate 对不支持的对齐值直接抛出异常而非回退 std::max_align 。
WG21 P0337R0 ：polymorphic_allocator 的默认 operator= 定义为 = delete 。
包括以下已有其它实现支持的 ISO C++17 后的缺陷修复：
LWG 2961 ：不需要考虑无法实现的后置条件。
LWG 2969 ：明确 polymorphic_allocator 的 construct 函数模板使用 *this 而不是
	memory_resource 进行构造。
LWG 2975 ：明确 polymorphic_allocator 的 construct 函数模板排除对第一参数为
	pair 实例的指针类型时的重载。
LWG 3000 ：存储资源类的 do_is_equal 的实现没有冗余的 dynamic_cast 。
LWG 3036 ：删除 polymorphic_allocator 的 destroy 函数模板，
	因为 allocator_traits 已提供实现。
LWG 3037 : 明确 polymorphic_allocator 支持不完整的值类型。
LWG 3038 ：在 polymorphic_allocator 的 allocate 函数处理不能被 size_t 表示的大小。
LWG 3113 ：明确 polymorphic_allocator 的 construct 函数模板转移构造的元组值。
LWG 3237 ：修改 LWG 3038 和 WG21 P0339R6 抛出的异常类型。
包括以下 ISO C++17 后的修改：
WG21 P0339R6 ：支持 polymorphic_allocator 的 byte 默认模板参数和对象分配函数模板。
WG21 P0591R4 ：在 polymorphic_allocator 中支持递归构造 std::pair 。
WG21 P0619R4 ：在 memory_resource 中显式声明默认构造函数和复制构造函数。
提供以下扩展：
pmr::polymorphic_allocator 对 is_bitwise_swappable 特化。
实现注记：
注意 ISO C++ 标准库的静态对象中，只有 [narrow.stream.objects] 和
	[wide.stream.objects] 通过 [basic.ios.cons] 和 [ios.init]
	允许程序创建 std::basic_ios::Init 对象而隐含指定相对其它静态对象销毁的顺序要求。
	其它静态对象包括 pmr 资源没有相应的保证。
和 libstdc++ 、libc++ 和 Microsoft VC++ 的实现（另见 WG21 P1247 ）不同，在
	ISO C++17 中指定为静态对象的资源仍保证析构，且不保证常量初始化，
	而无法保证和其它静态对象之间的析构顺序。
使用默认资源初始化的静态对象可能因销毁时资源对象的生存期已终止而引起未定义行为；
	依赖这些实现行为的程序的可移植性被削弱，这些实现可能隐藏程序缺陷。
另一方面，[basic.stc.static]/2 要求不能省略静态对象的析构的副作用，
	而保证析构多态类对象不能排除副作用（依赖 ABI ；实际上，一些典型实现中，
	销毁多态类对象时会修改内部的非平凡数据结构）而无法忽略，
	因此可移植的 C++ 实现不能排除这些对象的析构函数调用。
*/


#ifndef YB_INC_ystdex_memory_resource_h_
#define YB_INC_ystdex_memory_resource_h_ 1

#include "memory.hpp" // for internal "memory.hpp", byte, size_t,
//	std::bad_array_new_length, yalignof, yconstraint, yaligned, yforward,
//	ystdex::uninitialized_construct_using_allocator, std::pair, yassume, list,
//	equal_to, std::hash, is_bitwise_swappable;
// NOTE: See "placement.hpp" for comments on inclusion conditions.
#if (YB_IMPL_MSCPP >= 1910 && _MSVC_LANG >= 201603L) \
	|| (__cplusplus >= 201603L && __has_include(<memory_resource>))
#	include <memory_resource>
#	if (YB_IMPL_MSCPP >= 1910 && _MSVC_LANG >= 201603L) \
	|| __cpp_lib_memory_resource >= 201603L
// NOTE: This is for %std::memory_resource, not the header. WG21 P0619R4 does
//	not change the available features, so '201603L' is enough.
#		define YB_Has_memory_resource 1
#	endif
#elif __cplusplus > 201402L && __has_include(<experimental/memory_resource>)
// NOTE: For exposition only. The implementations in libstdc++ and libc++ are
//	incomplete as of ISO C++17 is published.
#	include <experimental/memory_resource>
#	if __cpp_lib_experimental_memory_resources >= 201402L
#		define YB_Has_memory_resource 2
#	endif
#endif
#if YB_Has_memory_resource && __cplusplus >= 202002L \
	&& (!defined(__GLIBCXX__) || __GLIBCXX__ > 20190731)
// XXX: See https://github.com/cplusplus/draft/issues/3111.
// XXX: See https://gcc.gnu.org/git/?p=gcc.git;a=blobdiff;f=libstdc%2B%2B-v3/include/std/memory;h=0a483d2d8d1a1287685cb5cc8a7d338a14e7fef3;hp=3036802f8c3eb1c3013dc1720ad85087e5202694;hb=3090082cbefd8b1374f237bd4242b554490b2933;hpb=e6c847fb8f90b1c119a677b81cfc294b13eb7772.
#	define YB_Impl_P0339R6 true
#else
#	define YB_Impl_P0339R6 false
#endif
#if YB_Has_memory_resource != 1
#	include <tuple> // for std::tuple, std::forward_as_tuple, std::tuple_cat;
#	include "operators.hpp" // for equality_comparable;
#endif
#include "base.h" // for noncopyable, nonmovable;
#include "bit.hpp" // for is_power_of_2_positive;
#include "list.hpp"// for list;
#include "algorithm.hpp" // for ystdex::max;
#include <unordered_map> // for std::unordered_map;
#include <vector> // for std::vector;
#if YB_Has_memory_resource != 1
#	if (defined(__GLIBCXX__) && !(defined(_GLIBCXX_USE_C99_STDINT_TR1) \
	&& defined(_GLIBCXX_HAS_GTHREADS))) \
	|| (defined(_LIBCPP_VERSION) && defined(_LIBCPP_HAS_NO_THREADS))
// XXX: The synchonization does not work. However, this still makes
//	%synchronized_pool_resource different than %unsynchronized_pool_resource in
//	%ystdex::pmr. Preserving %ystdex::single_thread pseudo implementation
//	introduces some basic checks of sanity on mutex types.
#		include "pseudo_mutex.h" // for ystdex::single_thread::mutex,
//	ystdex::single_thread::lock_guard;
#		define YB_Impl_mutex_ns ystdex::single_thread
#	else
#		include <mutex> // for std::mutex, std::lock_guard;
#		define YB_Impl_mutex_ns std
#	endif
#	include "type_pun.hpp" // for pun_ref;
#endif

/*!
\brief \c \<memory_resource> 特性测试宏。
\see ISO C++20 [version.syn] 。
\see WG21 P0941R2 2.2 。
\see https://docs.microsoft.com/cpp/visual-cpp-language-conformance 。
\see https://docs.microsoft.com/cpp/preprocessor/predefined-macros 。
\since build 842
*/
//@{
#ifndef __cpp_lib_memory_resource
#	if (YB_IMPL_MSCPP >= 1910 && _MSVC_LANG >= 201603L) \
	|| __cplusplus >= 201603L
#		define __cpp_lib_memory_resource 201603L
#	endif
#endif
//@}

//! \since build 842
namespace ystdex
{

// TODO: Check support of P0591R4 and use %std::pmr::polymorphic_allocator if
//	possible.

namespace details
{

//! \note 和 Microsoft VC++ 实现中的 %std::_Get_size_of_n 类似。
//@{
// XXX: Neither %SIZE_MAX nor %std::numeric_limits<size_t>::max() (as per LWG
//	3310) are used, to avoid dependencies on <climits> or <limits>. This is
//	totally legimate as %size_t(-1) is equivalent.
template<size_t _vSize>
YB_ATTR_nodiscard YB_PURE inline size_t
get_size_of_n(size_t n)
{
	if(size_t(-1) / _vSize >= n)
		return size_t(n * _vSize);
	throw std::bad_array_new_length();
}
template<>
YB_ATTR_nodiscard YB_PURE yconstfn size_t
get_size_of_n<1>(size_t n)
{
	return n;
}
//@}

} // namespace details;

namespace pmr
{

inline namespace cpp2017
{

// NOTE: Microsoft VC++ 15.8.2 implements ISO C++17 version (using WG21 N4700
//	suggested by the assertion messages), while current libstdc++ and libc++
//	only provide %std::experimental::fundamentals_v2 version, although some
//	resoultions have applied on libc++.
#if YB_Has_memory_resource == 1
using std::pmr::memory_resource;

using std::pmr::get_default_resource;
#else
//! \ingroup YBase_replacement_features
//@{
/*!
\brief 存储资源。
\see LWG 2724 。
*/
class YB_API YB_ATTR_novtable memory_resource
	yimpl(: private equality_comparable<memory_resource>)
{
private:
	static constexpr size_t max_align = yalignof(std::max_align_t);

public:
	/*!
	\since build 850
	\see WG21 P0619R4 。
	*/
	//@{
	memory_resource() = default;
	memory_resource(const memory_resource&) = default;
	//@}
	// XXX: Microsoft VC++, libstdc++ (in GCC 9.1 and 9.2) and libc++
	//	(in %std::experimanetal) all defined the destructor inline. Keeping it
	//	out-of-line for the style consistency and to avoid Clang++ warning
	//	[-Wweak-vtables] reilably.
	// TODO: Measure the benefits preciesly. Alternatively, change it as
	//	https://github.com/microsoft/STL/issues/1314?
	//! \brief 虚析构：类定义外默认实现。
	virtual
	~memory_resource();

	YB_ATTR_nodiscard friend bool
	operator==(const memory_resource& a, const memory_resource& b) ynothrow
	{
		return &a == &b || a.is_equal(b);
	}

	//! \pre 断言：对齐值是 2 的整数次幂。
	//@{
	//! \post 断言：返回值符合参数指定的对齐值要求。
	YB_ALLOCATOR YB_ATTR(alloc_align(3), alloc_size(2)) YB_ATTR_returns_nonnull
		void*
	allocate(size_t bytes, size_t alignment = max_align)
	{
		// NOTE: This is not specified directly by the specification, but the
		//	resolution of LWG 2843 has made the requirement on the alignment
		//	same to %do_allocation. Microsoft VC++ 15.8.2 implements it using
		//	an assertion here similarly. Libc++ experimental does not assert and
		//	does silently round up the alignment value which is always the
		//	maximul alignment in its %__resource_adaptor_imp::do_allocate.
		yconstraint(is_power_of_2_positive(alignment));

		const auto p(do_allocate(bytes, alignment));

		return yaligned(p, alignment);
	}

	void
	deallocate(void* p, size_t bytes, size_t alignment = max_align)
	{
		// NOTE: The alignment requirement is same to %allocate.
		yconstraint(is_power_of_2_positive(alignment));
		return do_deallocate(p, bytes, alignment);
	}
	//@}

	YB_ATTR_nodiscard bool
	is_equal(const memory_resource& other) const ynothrow
	{
		// NOTE: See LWG 2843.
		return do_is_equal(other);
	}

	// XXX: LWG 2724 is implemented by Microsoft VC++ 15.8.2. Not sure which is
	//	the initial version as it seems undocumented.
	// XXX: LWG 2724 is NOT implemented in libstdc++ and libc++ in
	//	%std::experimental currently. It is not likely to be because the issue
	//	is resolved after adoption of P0220R1.
private:
	//! \sa LWG 2843 。
	YB_ALLOCATOR YB_ATTR(alloc_align(3), alloc_size(2)) YB_ATTR_returns_nonnull
		virtual void*
	do_allocate(size_t, size_t) = 0;

	virtual void
	do_deallocate(void*, size_t, size_t) = 0;

	YB_ATTR_nodiscard virtual bool
	do_is_equal(const memory_resource&) const ynothrow = 0;
};


YB_ATTR_nodiscard YB_API YB_ATTR_returns_nonnull memory_resource*
get_default_resource() ynothrow;
//@}
#endif

} // inline namespace cpp2017;


//! \ingroup allocators
//@{
#if YB_Impl_P0339R6
//! \since build 863
using std::pmr::polymorphic_allocator;
#else
/*!
\ingroup YBase_replacement_features
\brief 多态分配器。
*/
template<class _type = byte>
class polymorphic_allocator
{
public:
	using value_type = _type;

private:
	//! \invariant \c memory_rsrc 。
	memory_resource* memory_rsrc;

public:
	polymorphic_allocator() ynothrow
		: memory_rsrc(get_default_resource())
	{}
	//! \pre 断言：参数指针非空。
	YB_NONNULL(2)
	polymorphic_allocator(memory_resource* r) ynothrowv
		: memory_rsrc(r)
	{
		yconstraint(r);
	}
	polymorphic_allocator(const polymorphic_allocator& other) = default;
	template<typename _tOther>
	polymorphic_allocator(const polymorphic_allocator<_tOther>& other) ynothrow
		: memory_rsrc(other.resource())
	{}

	//! \since build 843
	polymorphic_allocator&
	operator=(const polymorphic_allocator&) = delete;

	//! \see LWG 3038 。
	YB_ALLOCATOR YB_ATTR(alloc_size(2)) YB_ATTR_returns_nonnull _type*
	allocate(size_t n)
	{
		return static_cast<_type*>(memory_rsrc->allocate(
			details::get_size_of_n<sizeof(_type)>(n), yalignof(_type)));
	}

	void
	deallocate(_type* p, size_t n)
	{
		// XXX: No need to verify that %size_t can represent the size.
		memory_rsrc->deallocate(p, sizeof(_type) * n, yalignof(_type));
	}

	/*!
	\see WG21 P0339R6 。
	\since build 863
	*/
	//@{
	void*
	allocate_bytes(size_t nbytes, size_t alignment = yalignof(std::max_align_t))
	{
		return memory_rsrc->allocate(nbytes, alignment);
	}

	void
	deallocate_bytes(void* p, size_t nbytes,
		size_t alignment = yalignof(std::max_align_t))
	{
		return memory_rsrc->deallocate(p, nbytes, alignment);
	}

	template<typename _tObj>
	_tObj*
	allocate_object(size_t n = 1)
	{
		if(size_t(-1) / sizeof(_tObj) >= n)
			return static_cast<_tObj*>(allocate_bytes(n * sizeof(_tObj),
				yalignof(_tObj)));
		throw std::bad_array_new_length();
	}

	template<typename _tObj>
	void
	deallocate_object(_tObj* p, size_t n = 1)
	{
		deallocate_bytes(p, n * sizeof(_tObj), yalignof(_tObj));
	}

	template<typename _tObj, typename... _tCtorParams>
	_tObj*
	new_object(_tCtorParams&&... ctor_args)
	{
		_tObj* p(allocate_object<_tObj>());

		try
		{
			construct(p, yforward(ctor_args)...);
		}
		catch(...)
		{
			deallocate_object(p);
			throw;
		}
		return p;
	}

	template<typename _tObj>
	void
	delete_object(_tObj* p)
	{
		destroy(p);
		deallocate_object(p);
	}
	//@}

	/*!
	\see WG21 P0591R4 。
	\since build 850
	*/
	template<typename _tObj, typename... _tParams>
	YB_NONNULL(2) void
	construct(_tObj* p, _tParams&&... args)
	{
		ystdex::uninitialized_construct_using_allocator(p, *this,
			yforward(args)...);
	}

	YB_ATTR_nodiscard polymorphic_allocator
	select_on_container_copy_construction() const
	{
		return {};
	}

	YB_ATTR_nodiscard YB_ATTR_returns_nonnull memory_resource*
	resource() const
	{
		return memory_rsrc;
	}
};

//! \relates polymorphic_allocator
//@{
template<typename _type1, typename _type2>
YB_ATTR_nodiscard inline bool
operator==(const polymorphic_allocator<_type1>& a,
	const polymorphic_allocator<_type2>& b) ynothrow
{
	return *a.resource() == *b.resource();
}

template<typename _type1, typename _type2>
YB_ATTR_nodiscard inline bool
operator!=(const polymorphic_allocator<_type1>& a,
	const polymorphic_allocator<_type2>& b) ynothrow
{
	return !(a == b);
}
//@}
#endif
#undef YB_Impl_P0339R6
//@}


inline namespace cpp2017
{

#if YB_Has_memory_resource == 1
using std::pmr::new_delete_resource;
using std::pmr::null_memory_resource;
using std::pmr::set_default_resource;

using std::pmr::pool_options;
#else
//! \ingroup YBase_replacement_features
//@{
YB_ATTR_nodiscard YB_API memory_resource*
new_delete_resource() ynothrow;

YB_ATTR_nodiscard YB_API memory_resource*
null_memory_resource() ynothrow;

//! \see LWG 2961 。
YB_API YB_ATTR_returns_nonnull memory_resource*
set_default_resource(memory_resource*) ynothrow;


/*!
\note 实现定义：默认限制参见池的构造函数说明。
\see WG21 N4778 [mem.res.pool.options] 。
*/
struct YB_API pool_options
{
	size_t max_blocks_per_chunk = 0;
	size_t largest_required_pool_block = 0;
};
//@}
#endif

} // inline namespace cpp2017;

//! \ingroup YBase_replacement_extensions
//@{
/*!
\brief 和 pmr::new_delete_resource 返回的资源具有一致效果的存储资源。
\note 不保证 pmr::new_delete_resource 的动态类型是这个类型。
\since build 930

可用于在 pmr::new_delete_resource 返回值指向的对象不确定生存期使用的存储资源类型。
*/
class YB_API new_delete_resource_t : public memory_resource
{
public:
	YB_ALLOCATOR YB_ATTR(alloc_align(3), alloc_size(2)) YB_ATTR_returns_nonnull
		void*
	do_allocate(size_t, size_t) override;

	void
	do_deallocate(void* p, size_t bytes, size_t alignment) ynothrow override;

	YB_ATTR_nodiscard yimpl(YB_STATELESS) bool
	do_is_equal(const memory_resource&) const ynothrow override;
};


//! \since build 863
//@{
/*!
\brief 修改池选项的默认值。
\post 参数中的数据成员不含 0 值。
\note 每区块最大块数限制为 \c size_t((sizeof(size_t) * CHAR_BIT) << 16U) 。
\note 最大块分配大小限制为 \c size_t((sizeof(size_t) * CHAR_BIT) << 18U) 。
*/
YB_API void
adjust_pool_options(pool_options&);


/*!
\brief 资源池。
\warning 非虚析构。
*/
class YB_API resource_pool : private noncopyable
{
private:
	//! \brief 块类型。
	class chunk_t;
	//! \brief 块标识类型。
	using id_t = size_t;
	//! \brief 块存储对类型。
	using chunk_pr_t = std::pair<id_t, chunk_t>;
	//! \since build 864
	//@{
	//! \build 块集合类型。
	using chunks_t = list<chunk_pr_t, polymorphic_allocator<chunk_pr_t>>;
	//! \build 块集合迭代器类型。
	using chunks_iter_t = chunks_t::iterator;
	//@}
	//! \brief 保存在块末尾的元数据类型。
	struct block_meta_t
	{
		//! \invariant <tt>i_chunk != chunks.end()</tt> 。
		chunks_iter_t i_chunk;
	};

	/*!
	\brief 块中最大区块数。
	\since build 863
	*/
	size_t max_blocks_per_chunk;
	//! \brief 被池所有的块。
	chunks_t chunks;
	//! \since build 864
	/*!
	\brief 待分配贮藏迭代器。
	\since build 864
	*/
	chunks_iter_t i_stashed;
	/*!
	\brief 后备块迭代器。
	\since build 941
	*/
	chunks_iter_t i_backup;
	//! \brief 下一可用的块中分配块的容量。
	size_t next_capacity;
	/*!
	\brief 分配块大小。
	\invariant <tt>block_size > 0</tt> 。
	*/
	size_t block_size;
	//! \since build 863
	//@{
	//! \brief 附加数据。
	size_t extra_data;

public:
	//! \brief 最小区块大小。
	static yconstexpr const size_t min_block_size = sizeof(block_meta_t) + 1;
	//@}
	//! \since build 887
	//@{
	//! \brief 块中的默认区块容量。
	static yconstexpr const size_t default_capacity = yimpl(4);

	static_assert(default_capacity > 1, "Invalid default value found.");

	/*!
	\brief 构造：使用上级存储资源、块中最大区块数、区块大小、附加数据和起始区块容量。
	\pre 断言：区块大小不小于 \c resource_pool::min_block_size 。
	\pre 断言：区块大小不小于 \c resource_pool::min_block_size 。
	\pre 断言：起始区块容量大于 1 。
	\sa get_extra_data
	\sa size_for_capacity

	通过参数指定的上级存储资源和块属性构造块。
	附加数据是可选的。使用以 2 为底的区块大小的对数可加速对数分布的池的查询过程。
	*/
	resource_pool(memory_resource&, size_t, size_t, size_t = 0,
		size_t = default_capacity) ynothrowv;
	//! \since build 867
	resource_pool(resource_pool&&) ynothrow;
	//@}
	//! \since build 863
	~resource_pool();

	/*!
	\pre 断言：分配器相等。
	\since build 867
	*/
	resource_pool&
	operator=(resource_pool&&) ynothrowv;

private:
	//! \pre 参数指向池中块的首字节。
	YB_ATTR_nodiscard YB_PURE static block_meta_t&
	access_meta(void* p, size_t blk_size) ynothrow
	{
		// NOTE: The types of the data members in %block_meta_t shall be decayed
		//	to avoid need of call to %std::launder which is not available before
		//	ISO C++17.
		static_assert(is_decayed<id_t>(), "Invalid type found.");

		yconstraint(p);

		const auto p_meta(static_cast<block_meta_t*>(static_cast<void*>(
			static_cast<byte*>(p) + blk_size - sizeof(block_meta_t))));

		return *static_cast<block_meta_t*>(
			yaligned(p_meta, yalignof(block_meta_t)));
	}

public:
	//! \sa i_stashed
	YB_ALLOCATOR YB_ATTR_returns_nonnull void*
	allocate();

	/*!
	\brief 调整分配空间大小以适应块。
	\since build 863
	*/
	YB_ATTR_nodiscard YB_STATELESS static yconstfn size_t
	adjust_for_block(size_t bytes, size_t alignment) ynothrow
	{
		return ystdex::max(bytes + sizeof(block_meta_t), alignment);
	}

	/*!
	\brief 清除指针并释放所有块。
	\since build 864
	*/
	void
	clear() ynothrow;

	void
	deallocate(void*) ynothrowv;

	//! \since build 845
	YB_ATTR_nodiscard YB_PURE size_t
	get_block_size() const ynothrow
	{
		return block_size;
	}

	//! \since build 887
	YB_ATTR_nodiscard YB_PURE size_t
	get_capacity() const ynothrow
	{
		return next_capacity;
	}

	YB_ATTR_nodiscard YB_PURE size_t
	get_extra_data() const ynothrow
	{
		return extra_data;
	}

	/*!
	\brief 设置下一个分配块使用的区块容量。
	\pre 断言：区块容量大于 1 。
	\since build 887
	*/
	void
	set_capacity(size_t cap) ynothrowv
	{
		yconstraint(cap > 1);
		next_capacity = cap;
	}

	//! \since build 863
	YB_ATTR_nodiscard YB_PURE memory_resource&
	upstream() const ynothrowv
	{
		auto p(chunks.get_allocator().resource());

		yassume(p);
		return *p;
	}
};


/*!
\brief 超出池内分配的资源映射。
\warning 非虚析构。
*/
class YB_API oversized_map
	: private yimpl(noncopyable), private yimpl(nonmovable)
{
public:
	using mapped_type = std::pair<size_t, size_t>;
	using map_type = std::unordered_map<void*, mapped_type, std::hash<void*>,
		equal_to<>, polymorphic_allocator<std::pair<void* const,
		mapped_type>>>;

private:
	map_type entries;

public:
	oversized_map(memory_resource& mem_rsrc)
		: entries(&mem_rsrc)
	{}
	~oversized_map();

	YB_ATTR_nodiscard YB_PURE memory_resource&
	upstream() ynothrowv
	{
		const auto p_upstream(entries.get_allocator().resource());

		yassume(p_upstream);
		return *p_upstream;
	}

	YB_ALLOCATOR YB_ATTR(alloc_align(3), alloc_size(2)) YB_ATTR_returns_nonnull
		void*
	allocate(size_t, size_t);

	void
	deallocate(void*, size_t, size_t) yimpl(ynothrowv);

	void
	release() ynothrow;
};
//@}


/*!
\brief 池资源。
\since build 843

接口同 ISO C++17 的 std::pmr::unsynchronized_pool_resource ，
	但保证能在上游的分配器分配的区块为空时去配。
*/
class YB_API pool_resource : public memory_resource
{
private:
	using pools_t
		= std::vector<resource_pool, polymorphic_allocator<resource_pool>>;

	pool_options saved_options;
	//! \since build 863
	oversized_map oversized;
	pools_t pools;

public:
	/*!
	\note 实现定义：参见 adjust_pool_options 的调整的值。
	\sa adjust_pool_options
	*/
	//@{
	pool_resource() ynothrow
		: pool_resource(pool_options(), get_default_resource())
	{}
	//! \pre 断言：指针参数非空。
	YB_NONNULL(3)
	pool_resource(const pool_options&, memory_resource*) ynothrow;
	//! \pre 间接断言：指针参数非空。
	explicit
	pool_resource(memory_resource* upstream)
		: pool_resource(pool_options(), upstream)
	{}
	explicit
	pool_resource(const pool_options& opts)
		: pool_resource(opts, get_default_resource())
	{}
	//@}
	~pool_resource() override;

	void
	release() yimpl(ynothrow);

	YB_ATTR_nodiscard YB_ATTR_returns_nonnull YB_PURE memory_resource*
	upstream_resource() const yimpl(ynothrow)
	{
		return pools.get_allocator().resource();
	}

	YB_ATTR_nodiscard YB_PURE pool_options
	options() const yimpl(ynothrow)
	{
		return saved_options;
	}

protected:
	YB_ALLOCATOR YB_ATTR(alloc_align(3), alloc_size(2)) YB_ATTR_returns_nonnull
		void*
	do_allocate(size_t, size_t) override;

	void
	do_deallocate(void*, size_t, size_t) yimpl(ynothrowv) override;

	YB_ATTR_nodiscard yimpl(YB_STATELESS) bool
	do_is_equal(const memory_resource&) const ynothrow override;

private:
	YB_ATTR_nodiscard YB_PURE std::pair<pools_t::iterator, size_t>
	find_pool(size_t, size_t) ynothrow;

	YB_ATTR_nodiscard YB_PURE bool
	pool_exists(const std::pair<pools_t::iterator, size_t>&) ynothrow;

	YB_ATTR_nodiscard YB_PURE memory_resource&
	upstream() ynothrowv
	{
		const auto p_upstream(upstream_resource());

		yassume(p_upstream);
		return *p_upstream;
	}
};
//@}

inline namespace cpp2017
{

// XXX: %std::pmr::synchronized_pool_resource and
//	%std::pmr::unsynchronized_pool_resource can be monotonic until %release or
//	destruction. Although they are not both in %std implementation of Microsoft
//	VC++ 15.8.2 and in %ystdex where both implement deallocation of upstream
//	blocks in %do_deallocate, this is not specified by ISO C++.

#if YB_Has_memory_resource == 1
using std::pmr::synchronized_pool_resource;
using std::pmr::unsynchronized_pool_resource;
using std::pmr::monotonic_buffer_resource;
#else
//! \ingroup YBase_replacement_features
//@{
//! \since build 845
class YB_API synchronized_pool_resource : yimpl(public pool_resource)
{
private:
	using mutex = YB_Impl_mutex_ns::mutex;
	template<typename _tMutex>
	using lock_guard = YB_Impl_mutex_ns::lock_guard<_tMutex>;

	mutable mutex mtx{};

public:
	yimpl(using) pool_resource::pool_resource;
	//! \brief 虚析构：类定义外默认实现。
	~synchronized_pool_resource() override;

	void
	release() yimpl(ynothrow)
	{
		lock_guard<mutex> gd(mtx);

		pool_resource::release();
	}

protected:
	YB_ALLOCATOR YB_ATTR(alloc_align(3), alloc_size(2)) YB_ATTR_returns_nonnull
		void*
	do_allocate(size_t bytes, size_t alignment) override
	{
		lock_guard<mutex> gd(mtx);

		return pool_resource::do_allocate(bytes, alignment);
	}

	void
	do_deallocate(void* p, size_t bytes, size_t alignment) yimpl(ynothrowv)
		override
	{
		lock_guard<mutex> gd(mtx);

		pool_resource::do_deallocate(p, bytes, alignment);
	}
};


yimpl(using) unsynchronized_pool_resource = pool_resource;


//! \since build 845
class YB_API monotonic_buffer_resource : public memory_resource,
	private yimpl(noncopyable), private yimpl(nonmovable)
{
private:
	struct chunks_t;
	memory_resource* upstream_rsrc;
	void* current_buffer = {};
	size_t next_buffer_size;
	size_t space_available = 0;
	ystdex::aligned_storage_t<16> chunks_data{};
	//! \invariant <tt>&pun.get() == &chunks_data</tt>
	pun_ref<chunks_t> pun;

public:
	//! \note 实现定义：未指定时，初始大小为不小于 4 * sizeof(size_t) 的定值。
	//@{
	monotonic_buffer_resource()
		: monotonic_buffer_resource(get_default_resource())
	{}
	explicit
	monotonic_buffer_resource(memory_resource*) yimpl(ynothrow);
	monotonic_buffer_resource(size_t, memory_resource*) yimpl(ynothrow);
	monotonic_buffer_resource(void*, size_t, memory_resource*) yimpl(ynothrow);
	explicit
	monotonic_buffer_resource(size_t initial_size) yimpl(ynothrow)
		: monotonic_buffer_resource(initial_size, get_default_resource())
	{}
	monotonic_buffer_resource(void* buffer, size_t buffer_size) yimpl(ynothrow)
		: monotonic_buffer_resource(buffer, buffer_size, get_default_resource())
	{}
	//@}
	~monotonic_buffer_resource() override;

	void
	release() yimpl(ynothrow);

	YB_ATTR_nodiscard YB_ATTR_returns_nonnull YB_PURE memory_resource*
	upstream_resource() const yimpl(ynothrow)
	{
		return upstream_rsrc;
	}

protected:
	//! \note 实现定义：增长因子为 2 。
	YB_ALLOCATOR YB_ATTR(alloc_align(3), alloc_size(2)) YB_ATTR_returns_nonnull
		void*
	do_allocate(size_t, size_t) override;

	void
	do_deallocate(void*, size_t, size_t) override
	{}

	YB_ATTR_nodiscard yimpl(YB_STATELESS) bool
	do_is_equal(const memory_resource&) const ynothrow override;
};
//@}

#	undef YB_Impl_mutex_ns

#endif

} // inline namespace cpp2017;

} // namespace pmr;

/*!
\relates pmr::polymorphic_allocator
\since build 926
*/
template<typename _type>
struct is_bitwise_swappable<pmr::polymorphic_allocator<_type>> : true_
{};

} // namespace ystdex;

#endif

