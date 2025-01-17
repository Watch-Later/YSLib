#!/usr/bin/env bash
# (C) 2014-2018, 2020-2022 FrankHB.
# Build script for SHBuild.

set -e

: "${SHBuild_ToolDir:=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)}"
# XXX: Some options for stage 1 SHBuild are fixed. Provide more separated
#	options in future?
# XXX: Following variables are internal.
# shellcheck disable=2034
CXXFLAGS_OPT_UseAssert=true
# shellcheck disable=2034
C_CXXFLAGS_GC=' '
# shellcheck disable=2034
LDFLAGS_GC=' '
# shellcheck disable=2034
LDFLAGS_STRIP=' '
# NOTE: '-Og -g' is not supported. See $2020-09 @ %Documentation::Workflow.
# shellcheck disable=2034
CXXFLAGS_OPT_DBG='-O0 -g'
# XXX: This avoids clash between '-Og' with error 'Optimization level must be
#	between 0 and 3' caused by options passed to the linker from Clang++ driver.
#	See also https://bugs.llvm.org/show_bug.cgi?id=38305. Anyway LTO is not make
#	it easy to debug as the intentional '-Og' or '-O0' here, so simply disable
#	it.
# shellcheck disable=2034
LDFLAGS_IMPL_OPT=' '
# shellcheck source=./SHBuild-bootstrap.sh
. "$SHBuild_ToolDir/SHBuild-bootstrap.sh" # for SHBuild_Host_OS,
#	SHBuild_Host_Arch, SHBuild_Pushd, SHBuild_BaseDir, SHBuild_Puts, CXXFLAGS,
#	LDFLAGS, INCLUDES, LIBS, SHBuild_Popd;

: "${SHBuild_Output:=SHBuild}"

# XXX: After MSYS2 enabling ASLR by default, x86_64 binutils with g++ is buggy.
#	See https://www.msys2.org/news/#2021-01-31-aslr-enabled-by-default,
#	https://github.com/msys2/MINGW-packages/issues/6986,
#	https://github.com/msys2/MINGW-packages/issues/7023,
#	and https://sourceware.org/bugzilla/show_bug.cgi?id=26659. Here is a
#	workaround to the issue.
# XXX: This is now fixed by https://github.com/msys2/MINGW-packages/pull/8259.
# TODO: Detect precise version?
# XXX: There are some other bugs not resolved for ld.bfd on PE targets:
#	https://sourceware.org/bugzilla/show_bug.cgi?id=11539,
#	https://sourceware.org/bugzilla/show_bug.cgi?id=19803.
if [[ "$SHBuild_Host_OS" == 'Win32' && "$SHBuild_Host_Arch" == 'x86_64' \
	&& "$SHBuild_CXX_Style_" == 'G++' ]]; then
#	LDFLAGS_WKRD_="$(SHBuild_CheckCompiler "$CXX" \
#		'int main(){}' -Wl,--default-image-base-low '' \
#		-xc++ -Wl,--default-image-base-low)"
	# XXX: The linker (ld) from mingw-w64-x86_64-binutils 2.38-3 versions does
	#	not work otherwise. And mingw-w64-x86_64-lld-14.0.4-1 does not work with
	#	LTO here.
	LDFLAGS_WKRD_='-fuse-ld=lld'
else
	LDFLAGS_WKRD_=
fi

SHBuild_Pushd .
cd "$SHBuild_BaseDir"

SHBuild_Puts "Building ..."

# Precompiled header is not used here because it does not work well with
#	external %CXXFLAGS_OPT_DBG. It is also not used frequently like in stage 2.
#	Even it is needed, it should be better separated with the stage 2 option.
#	When needed, uncomment the following command (where
#	%SHBuild_S1_InitializePCH is from %SHBuild_ToolDir/SHBuild-bootstrap.sh),
#	to provide %SHBuild_IncPCH.
# SHBuild_S1_InitializePCH

# Note '-fwhole-program' should not be used because there
#	do exist multiple translation units when linking with YSLib source,
#	otherwise there would be unresolved reference to names with external
#	linkage which had been optimized away.
# XXX: %SHBuild_Verbose_ is external.
# shellcheck disable=2154
if [[ "$SHBuild_Verbose_" != '' ]]; then
	# XXX: Value of several variables may contain whitespaces.
	# shellcheck disable=2086
	SHBuild_Puts "$CXX" Main.cpp -o"$SHBuild_Output" $CXXFLAGS $LDFLAGS \
		$LDFLAGS_WKRD_ $SHBuild_IncPCH $INCLUDES $LIBS
fi
# XXX: Value of several variables may contain whitespaces.
# shellcheck disable=2086
"$CXX" Main.cpp -o"$SHBuild_Output" $CXXFLAGS $LDFLAGS $LDFLAGS_WKRD_ \
	$SHBuild_IncPCH $INCLUDES $LIBS

SHBuild_Popd
SHBuild_Puts "Done."

