#!/usr/bin/env bash
# (C) 2014-2022 FrankHB.
# Common options script to build projects in the shell.

# NOTE: This is mainly for stage 1 SHBuild bootstrap and the test. The options
#	here should be equivalent to %SHBuild-YSLib-common.txt. See the user
#	document for details.

set -e

: "${SHBuild_ToolDir:=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)}"
# shellcheck source=./SHBuild-common-toolchain.sh
. "$SHBuild_ToolDir/SHBuild-common-toolchain.sh" # for
#	SHBuid-common-toolchain.sh, CXX;

# XXX: %SHBuild_Debug is external.
# shellcheck disable=2154
if [[ "$SHBuild_Debug" != '' ]]; then
	CXXFLAGS_OPT_DBG='-O0 -g -D_GLIBCXX_DEBUG_PEDANTIC'
	LDFLAGS_OPT_DBG=' '
fi

: "${C_CXXFLAGS_GC:=-fdata-sections -ffunction-sections}"

# XXX: %SHBuild_Host_OS is external.
# shellcheck disable=2154
if [[ "$SHBuild_Host_OS" == 'OS_X' ]]; then
	: "${LDFLAGS_GC:="-Wl,--dead-strip"}"
else
	: "${LDFLAGS_GC:="-Wl,--gc-sections"}"
	#: "${LDFLAGS_GC:="-Wl,--gc-sections -Wl,--print-gc-sections"}"
fi

SHBuild_CXX_Style_=$(SHBuild_CheckCXX "$CXX")

# NOTE: See %SHBuild_CheckC.
if [[ $SHBuild_CXX_Style_ != '' ]] \
	&& ! echo 'int main(){}' | "$CXX" -pipe -xc++ \
		-o"$SHBuild_Env_TempDir/null" "$C_CXXFLAGS_GC" "$LDFLAGS_GC" \
		- 2> /dev/null; then
	C_CXXFLAGS_GC=''
	LDFLAGS_GC=''
fi
# XXX: %SHBuild_Host_OS is external.
# shellcheck disable=2154
if [[ "$SHBuild_Host_OS" != 'Win32' ]]; then
	: "${C_CXXFLAGS_PIC:=-fPIC -fno-semantic-interposition}"
fi

# NOTE: See https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html#Link-Options,
#	also https://clang.llvm.org/docs/ClangCommandLineReference.html#linker-flags.
: "${LDFLAGS_STRIP:=-s}"

if [[ "$SHBuild_Host_OS" != 'Win32' ]]; then
	: "${C_CXXFLAGS_EXT="-D_POSIX_C_SOURCE=200809L"}"
fi
: "${C_CXXFLAGS_COMMON:= \
"-pipe $C_CXXFLAGS_GC $C_CXXFLAGS_ARCH -pedantic-errors $C_CXXFLAGS_EXT"}"
: "${C_CXXFLAGS_OPT_LV:=-O3}"
: "${C_CXXFLAGS_WARNING:="-Wall \
-Wcast-align \
-Wdeprecated \
-Wdeprecated-declarations \
-Wextra \
-Wfloat-equal \
-Wformat=2 \
-Winvalid-pch \
-Wmissing-declarations \
-Wmissing-include-dirs \
-Wmultichar \
-Wno-format-nonliteral \
-Wredundant-decls \
-Wshadow \
-Wsign-conversion"}"

# The call of %SHBuild_CheckCXX should have initialized %SHBuild_Env_TempDir.
# TODO: Impl without pthread?
if "$CXX" -dumpspecs 2>& 1 | grep mthreads: > /dev/null; then
	CXXFLAGS_IMPL_COMMON_THRD_='-mthreads'
	LDFLAGS_IMPL_COMMON_THRD_='-mthreads'
elif echo 'int main(){}' | "$CXX" -pipe -xc++ \
	-o"$SHBuild_Env_TempDir/null" -Werror -mthreads - 2> /dev/null; then
	if echo 'int main(){}' | "$CXX" -pipe -xc++ \
		-o"$SHBuild_Env_TempDir/null" -c -Werror -mthreads - 2> /dev/null; then
		CXXFLAGS_IMPL_COMMON_THRD_='-mthreads'
	else
		CXXFLAGS_IMPL_COMMON_THRD_='-D_MT'
	fi
	LDFLAGS_IMPL_COMMON_THRD_='-mthreads'
elif "$CXX" -dumpspecs 2>& 1 | grep no-pthread: > /dev/null; then
	CXXFLAGS_IMPL_COMMON_THRD_=''
	LDFLAGS_IMPL_COMMON_THRD_=''
elif echo 'int main(){}' | "$CXX" -pipe -xc++ \
	-o"$SHBuild_Env_TempDir/null" -pthread - 2> /dev/null; then
	CXXFLAGS_IMPL_COMMON_THRD_='-pthread'
	LDFLAGS_IMPL_COMMON_THRD_='-pthread'
else
	CXXFLAGS_IMPL_COMMON_THRD_='-pthread'
	LDFLAGS_IMPL_COMMON_THRD_='-pthread'
fi

# NOTE: The compiler should be specified earlier than this line to
#	automatically determine if these values should be used.
if [[ $SHBuild_CXX_Style_ == "Clang++" ]]; then
	: "${C_CXXFLAGS_COMMON_IMPL_:=-fno-merge-all-constants}"
	: "${CXXFLAGS_IMPL_WARNING:="-Wno-deprecated-register \
-Wno-mismatched-tags \
-Wno-missing-braces \
-Wshorten-64-to-32 \
-Wweak-vtables"}"
	LDFLAGS_IMPL_USE_LLD_=
	# XXX: This is the only choice to support LTO on MinGW32 now, but not
	#	ready to work.
	# TODO: Expose this option?
	#	LDFLAGS_IMPL_USE_LLD_=true
	#: "${CXXFLAGS_IMPL_OPT:=-flto}"
	#: "${LDFLAGS_IMPL_OPT:=-flto}"
	# XXX: LTO is disabled by default for compatibility to the prebuilt
	#	binaries (by G++).
elif [[ $SHBuild_CXX_Style_ == "G++" ]]; then
	: "${C_CXXFLAGS_IMPL_WARNING:="-Wdouble-promotion \
-Wlogical-op \
-Wsuggest-attribute=const \
-Wsuggest-attribute=noreturn \
-Wsuggest-attribute=pure \
-Wtrampolines"}"
	: "${CXXFLAGS_IMPL_WARNING:="-Wconditionally-supported \
-Wno-noexcept-type \
-Wstrict-null-sentinel \
-Wsuggest-final-types \
-Wsuggest-final-methods \
-Wzero-as-null-pointer-constant"}"
	# XXX: %SHBuild_Host_OS and %SHBuild_host_Arch are external.
	# shellcheck disable=2154
	# XXX: Only i686 MinGW32 uses old GCC not supporting -flto=auto now.
	if [[ "$SHBuild_Host_OS" == 'Win32' && "$SHBuild_Host_Arch" == 'i686' ]]; \
then
		LTO_=-flto=jobserver
	else
		LTO_=-flto=auto
	fi
	: "${CXXFLAGS_IMPL_OPT:="-fexpensive-optimizations \
$LTO_ \
-fno-enforce-eh-specs"}"
	# XXX: %SHBuild_Host_OS is external.
	# shellcheck disable=2154
	if [[ "$SHBuild_Host_OS" == 'Win32' ]]; then
		# XXX: Workarond for LTO bug on MinGW. See
		#	https://sourceware.org/bugzilla/show_bug.cgi?id=12762.
		: "${LDFLAGS_IMPL_OPT:="-fexpensive-optimizations $LTO_ \
-Wl,-allow-multiple-definition"}"
	else
		: "${LDFLAGS_IMPL_OPT:=-fexpensive-optimizations $LTO_}"
	fi
fi

: "${CFLAGS_STD:=-std=c11}"
: "${CFLAGS_WARNING:=" \
$C_CXXFLAGS_WARNING \
$C_CXXFLAGS_IMPL_WARNING"}"

: "${CXXFLAGS_IMPL_COMMON:="$CXXFLAGS_IMPL_COMMON_THRD_ \
	-U__GXX_MERGED_TYPEINFO_NAMES -D__GXX_MERGED_TYPEINFO_NAMES=1"}"

: "${CXXFLAGS_STD:=-std=c++11}"
: "${CXXFLAGS_WARNING:=" $CFLAGS_WARNING \
	-Wctor-dtor-privacy \
-Wnon-virtual-dtor \
-Woverloaded-virtual \
-Wsign-promo \
$CXXFLAGS_IMPL_WARNING"}"
# XXX: %CXXFLAGS_OPT_UseAssert is external.
# shellcheck disable=2154
if [[ "$CXXFLAGS_OPT_UseAssert" == '' ]]; then
	: "${CXXFLAGS_OPT_DBG:=" \
$C_CXXFLAGS_OPT_LV -DNDEBUG \
$CXXFLAGS_IMPL_OPT \
-fomit-frame-pointer"}"
else
	: "${CXXFLAGS_OPT_DBG:=" \
$C_CXXFLAGS_OPT_LV \
$CXXFLAGS_IMPL_OPT \
-fomit-frame-pointer"}"
fi

# XXX: Rename %CXXFLAGS_OPT_DBG to CFLAGS_OPT_DBG or C_CXXFLAGS_OPT_DBG?
: "${CFLAGS:="$CFLAGS_STD $C_CXXFLAGS_PIC $C_CXXFLAGS_COMMON $CFLAGS_WARNING \
$C_CXXFLAGS_COMMON_IMPL_ $CXXFLAGS_OPT_DBG"}"
CFLAGS="${CFLAGS//	/ }"

: "${CXXFLAGS:="$CXXFLAGS_STD $C_CXXFLAGS_PIC $C_CXXFLAGS_COMMON \
$CXXFLAGS_WARNING $CXXFLAGS_IMPL_COMMON $C_CXXFLAGS_COMMON_IMPL_ \
$CXXFLAGS_OPT_DBG"}"
CXXFLAGS="${CXXFLAGS//	/ }"

: "${LDFLAGS_OPT_DBG:="$LDFLAGS_STRIP $LDFLAGS_IMPL_OPT $LDFLAGS_GC"}"

: "${LDFLAGS:="$C_CXXFLAGS_PIC $LDFLAGS_IMPL_COMMON_THRD_ $LDFLAGS_OPT_DBG"}"
if [[ "$LDFLAGS_IMPL_USE_LLD_" != '' ]]; then
	LDFLAGS="-fuse-ld=lld $LDFLAGS"
fi
LDFLAGS="${LDFLAGS//	/ }"

