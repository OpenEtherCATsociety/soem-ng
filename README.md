soem-ng
=======

Prerequisites for all platforms
===============================

 * CMake 2.8.0 or later
 * GTest

Windows
=======

 * WinPCAP developer's pack
 * Visual Studio 2013 or later

Start a developer command prompt, then:

 $ mkdir build
 $ cd build
 $ cmake .. -DPCAP_ROOT_DIR=p:/WpdPack/ -DGTEST_ROOT=p:/gtest-1.7.0
 $ msbuild SOEM.sln

To build NSIS installer:

 $ set PATH=%PATH%;p:\nsis
 $ msbuild PACKAGE.vcxproj

Cygwin
======

 * WinPCAP developer's pack
 * GCC 4.6 or later

 $ mkdir build
 $ cd build
 $ cmake .. -DPCAP_ROOT_DIR=~/WpdPack -DGTEST_ROOT=~/gtest-1.7.0
 $ make

Linux
=====

 * GCC 4.6 or later

 $ mkdir build
 $ cd build
 $ cmake .. -DPCAP_ROOT_DIR=~/WpdPack -DGTEST_ROOT=~/gtest-1.7.0
 $ make

rt-kernel
=========

 * CMake 3.1.2
 * Workbench 2014.1 or later

Start a command prompt, then:

 $ mkdir build
 $ cd build
 $ export RTK=/p/rt-kernel-src
 $ export BSP=integrator
 $ cmake -DCMAKE_TOOLCHAIN_FILE=/p/cmake/toolchain/rt-kernel-arm9e.cmake .. -G "Unix Makefiles" -DGTEST_ROOT=/p/gtest-1.7.0
 $ make

Substitute BSP and toolchain file as appropriate.
