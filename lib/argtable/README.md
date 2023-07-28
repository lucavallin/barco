[![Build Status](https://travis-ci.org/argtable/argtable3.svg?branch=master)](https://travis-ci.org/argtable/argtable3)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)


Introduction of Argtable3
=========================

**Argtable3** is an open source ANSI C library that parses GNU-style
command-line options with the `getopt` library. It simplifies command-line
parsing by defining a declarative-style API that you can use to specify what
your command-line syntax looks like. Argtable3 will automatically generate
consistent error handling logic and textual descriptions of the command line
syntax, which are essential but tedious to implement for a robust CLI program.


Quick Start
-----------

You can embed the amalgamation source files in your projects, add Argtable3 as a
dependency in the vcpkg manifest, install Argtable3 as a system-wide CMake
package, or build the library from release archives.

### Embed Amalgamation Source Files

> We no longer provide the amalgamation source files (`argtable3.c` and
> `argtable3.h`) in the repository. You can get the amalgamation distribution
> either from the release page (`argtable-<version>-amalgamation.(zip|tar.gz)`),
> or generate the distribution yourself by using the generator under the `tools`
> directory:
>
> 1. Navigate to the `tools` directory.
> 2. Run `./build dist`, which will generate the distribution under the `<ROOT>/dist`
>    directory.

Add `argtable3.c` and `argtable3.h` from the amalgamation distribution to your
projects. This is the simplest and recommended way to use Argtable3: it not only
removes the hassle of building the library, but also allows compilers to do
better inter-procedure optimization.


### Install for a Single Project with vcpkg Manifest

[vcpkg](https://vcpkg.io) is an open source C/C++ package manager based on
CMake, and it supports certain stable releases of Argtable3. To add the library
to your CMake project, it's recommended to add vcpkg as a submodule to your
project repo and use it to manage project dependencies. All libraries installed
in this way can only be consumed by the project and won't impact other projects
in the system.

If your project is under `D:/projects/demo` and the vcpkg submodule is under
`D:/projects/demo/deps/vcpkg`, first you need to add Argtable3 to the manifest,
`D:/projects/demo/vcpkg.json`:
```
{
    "name": "demo",
    "version": "0.0.1",
    "dependencies": [
        {
            "name": "argtable3",
            "version>=": "3.2.1"
        }
    ],
    "builtin-baseline": "92b42c4c680defe94f1665a847d04ded890f372e"
}
```

To add Argtable3 to your CMake scripts, you need to integrate the local vcpkg to
CMake by setting the `CMAKE_TOOLCHAIN_FILE` variable. You also need to link to
the static VC runtime (`/MT` or `/MTd`) if you want to use the static library
version of Argtable3:
```
cmake_minimum_required(VERSION 3.18)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/deps/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(versionstest)

add_executable(main main.cpp)

find_package(Argtable3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE argtable3::argtable3)

if(VCPKG_TARGET_TRIPLET STREQUAL "x64-windows-static")
  set_property(TARGET main PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
```

Now you can run `cmake` to install Argtable3, configure and generate build
scripts, and build the project:
```
$ mkdir build
$ cd build
$ cmake .. -DVCPKG_TARGET_TRIPLET=x64-windows-static
$ cmake --build .
```

### Install for All Projects with vcpkg

If you want to make Argtable3 available for all projects in the system, you can
clone vcpkg to any directory and install packages there. Assuming vcpkg has been
cloned in `D:/dev/vcpkg` and the directory has been added to `PATH`, you can
install the static library version of Argtable3 in `D:/dev/vcpkg/installed`:
```
$ vcpkg install argtable3:x64-windows-static
```

Since each developer may clone vcpkg in a different place, it may not be
appropriate to specify the `CMAKE_TOOLCHAIN_FILE` variable in `CMakeLists.txt`.
Therefore, you should remove setting the `CMAKE_TOOLCHAIN_FILE` variable in the
`CMakeLists.txt` example above, and set the variable in the command line:
```
$ mkdir build
$ cd build
$ cmake .. -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_TOOLCHAIN_FILE=D:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
$ cmake --build .
```


### Build from Release Archives or Source

If none of the methods above suits your needs, or if you want to help developing
Argtable3, you can always build from archives on the release page or from the
repository.

* If you use GCC (Linux, MacOSX, MinGW, Cygwin), run:

  ```
  $ mkdir build
  $ cd build
  $ cmake -DCMAKE_BUILD_TYPE=Debug ..
  $ make
  $ make test
  ```

  Makefile-based generators in CMake only support one configuration at a time,
  so you need to specify `CMAKE_BUILD_TYPE` to `Debug`, `Release`, `MinSizeRel`,
  or `RelWithDebInfo`. To build multiple configurations, you need to create a
  build directory for each configuraiton.

  Since v3.2.1, CMake scripts will check `BUILD_SHARED_LIBS` and build either
  the static library or the dynamic library at a time. `BUILD_SHARED_LIBS` is
  `OFF` by default, so if you want to build the dynamic library, you have to set
  `BUILD_SHARED_LIBS` to `ON` explicitly:

  ```
  $ cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON ..
  ```

  To cleanup, run `make clean` or remove the build directory:

  ```
  $ rm -rf build
  ```

* If you use Microsoft Visual C++ compiler, run:

  ```
  $ mkdir build
  $ cd build
  $ cmake -G "Visual Studio 15 2017 Win64" ..
  $ cmake --build . --config Debug
  $ ctest -C Debug
  ```

  You can also use Visual Studio 2017 IDE to open the generated solution. To
  cleanup, just remove the `build` directory.


To build a tagged version, go to the project root directory, and use the
`Makefile` in the project root folder to check out the specified version:

  ```
  $ make taglist
  Available TAGs:
  v3.1.1.432a160
  $ make co TAG=v3.1.1.432a160
  $ cd .tags/v3.1.1.432a160
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ make test
  ```

You will find the shared library (or Windows DLL), static library, and the
amalgamation distribution under the build directory.


Documentation
-------------

To learn how to use the Argtable3 API, you can see the documentation on the web
site, study examples in the `examples` directory, or even check the unit tests
in the `tests` directory.

To build a local copy of the documentation, you need to install the following
tools:

* [Sphinx](https://www.sphinx-doc.org): A documentation generator based on the
  reStructuredText markup format.
* [Read the Docs Sphinx Theme](https://sphinx-rtd-theme.readthedocs.io): A
  Sphinx theme designed to look modern and be mobile-friendly.
* [Breathe](https://breathe.readthedocs.io): A bridge between the Sphinx and
  Doxygen documentation systems.
* [Doxygen](http://www.doxygen.nl/): A documentation generator for C/C++
  sources.

Go to the `docs` directory and run the `doxygen` command to generate Doxygen XML
output, which will be saved in the `docs/xml` directory:

```
$ doxygen
```

Run the `make` batch script and you will see the documentation in the
`docs/_build/html` directory.

```
$ make html
```


Unit Tests
----------

Argtable3 is a BSD-licensed open source library, so you can modify the library
anyway you want. However, before committing your code to your own repository or
the Argtable3 official repository, please make sure your changes won't cause any
compiler warning and can pass the unit tests included in the distribution.

To build and test each configuration (`Debug`, `Release`, `MinSizeRel`,
`RelWithDebInfo`), you can run CMake and CTest on all supported platforms:

```
$ mkdir build_debug && cd build_debug
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ cmake --build . --config Debug
$ ctest -C Debug

$ cd .. && mkdir build_release && cd build_release
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ cmake --build . --config Release
$ ctest -C Release

$ cd .. && mkdir build_minsizerel && cd build_minsizerel
$ cmake -DCMAKE_BUILD_TYPE=MinSizeRel ..
$ cmake --build . --config MinSizeRel
$ ctest -C MinSizeRel

$ cd .. && mkdir build_relwithdebinfo && cd build_relwithdebinfo
$ cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
$ cmake --build . --config RelWithDebInfo
$ ctest -C RelWithDebInfo
```

If you see the following screenshot, you know that some unit tests are broken:

```
$ make test
Running tests...
Test project ~/Projects/argtable3/build-gcc-release
    Start 1: test_shared
1/4 Test #1: test_shared ......................***Failed    0.07 sec
    Start 2: test_static
2/4 Test #2: test_static ......................***Failed    0.13 sec
    Start 3: test_src
3/4 Test #3: test_src .........................***Failed    0.13 sec
    Start 4: test_amalgamation
4/4 Test #4: test_amalgamation ................***Failed    0.14 sec

0% tests passed, 4 tests failed out of 4

Total Test time (real) =   0.48 sec

The following tests FAILED:
          1 - test_shared (Failed)
          2 - test_static (Failed)
          3 - test_src (Failed)
          4 - test_amalgamation (Failed)
Errors while running CTest
make: *** [Makefile:97: test] Error 8
```

To understand which unit tests are broken, you need to run the failed test
programs (based on CuTest) directly:

```
$ ./tests/test_shared
.......................................................................................
...................................................................F.............

There was 1 failure:
1) test_argdstr_basic_001: ~/Projects/argtable3/tests/testargdstr.c:51: assert failed

!!!FAILURES!!!
Runs: 168 Passes: 167 Fails: 1
```


Memory Issue Detection with ASan and Valgrind
---------------------------------------------

In order to prevent common memory issues in C, such as memory leak and buffer
overflow, we should use [ASan
(AddressSanitizer)](https://en.wikipedia.org/wiki/AddressSanitizer) and
[Valgrind](https://en.wikipedia.org/wiki/Valgrind) to detect as many
memory-related problems as possible before committing our code.

To use ASan, we need to add `-fsanitize=address` to the `CFLAGS` variable when
we run `cmake` to build the **Debug** version. We should use the Debug version
because CMake will add `-g` to `CFLAGS` and prevent optimizing the code, so we
can see accurate file names and line numbers in ASan error messages. After
building the code, set the `CTEST_OUTPUT_ON_FAILURE` variable to `1` to output
error messages when we run unit tests:

```
$ mkdir build
$ cd build
$ CFLAGS="-fsanitize=address" cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
$ CTEST_OUTPUT_ON_FAILURE=1 make test
Running tests...
Test project /home/tomghuang/Projects/argtable3/build
    Start 1: test_shared
1/4 Test #1: test_shared ......................   Passed    3.45 sec
    Start 2: test_static
2/4 Test #2: test_static ......................   Passed    3.31 sec
    Start 3: test_src
3/4 Test #3: test_src .........................   Passed    3.06 sec
    Start 4: test_amalgamation
4/4 Test #4: test_amalgamation ................   Passed    3.29 sec

100% tests passed, 0 tests failed out of 4

Total Test time (real) =  13.12 sec
```

To use Valgrind, just use `valgrind` to run the unit test programs:

```
$ valgrind ./tests/test_src
==23290== Memcheck, a memory error detector
==23290== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==23290== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==23290== Command: ./test_src
==23290==
......................................................................................
..................................................................................

OK (168 tests)

==23290==
==23290== HEAP SUMMARY:
==23290==     in use at exit: 0 bytes in 0 blocks
==23290==   total heap usage: 102,085 allocs, 102,085 frees, 5,589,475 bytes allocated
==23290==
==23290== All heap blocks were freed -- no leaks are possible
==23290==
==23290== For counts of detected and suppressed errors, rerun with: -v
==23290== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```


Authors
-------

Argtable is Copyright (C) 1998-2001,2003-2011 Stewart Heitmann.
Parts are Copyright (C) 1989-1994, 1996-1999, 2001, 2003
  Free Software Foundation, Inc.

Argtable was written by Stewart Heitmann <sheitmann@users.sourceforge.net>

Argtable is now maintained by Tom G. Huang <tomghuang@gmail.com>
The project homepage of argtable 3.x is http://www.argtable.org
The project homepage of argtable 2.x is http://argtable.sourceforge.net/

Here is a list of contributors who have helped to improve argtable:

- **Nina Clemson**: Editing the original argtable-1.0 documentation.
- **Livio Bertacco**: For bug fixes and the argtable-2.x Visual C++ Makefiles.
- **Justin Dearing**: For bug fixes and Windows DLL support, plus code support for the Open Watcom compiler and help with the Mac OS X configuration.
- **Asa Packer**: Contributing bug fixes and upgrades to the Visual C++ Makefiles.
- **Danilo Cicerone**: For the Italian translation of "Introduction to Argtable-2x" on http://www.digitazero.org.
- **Uli Fouquet**: For configuration patches and documentation related to cross-compiling argtable from Unix to Windows, as well as providing the arg_print_glossary_gnu function.
- **Shachar Shemesh**: For Debian package integration and kick-starting the migration to automake/autoconf.
- **Jasper Lievisse Adriaanse**:  Maintaining the argtable package in OpenBSD ports.
- **Ulrich Mohr**: For bug fixes relating to Texas Instrument DSP platforms.
- **John Vickers**: For bug fixes relating to Solaris/Motorola platforms.
- **Steve O'Neil**: For bug fixes relating to Solaris/Motorola platforms.
- **Lori A. Pritchett-Sheats**: Fixing a makefile bug relating to "make dist".
- **Paolo Bormida**: For instructions on building argtable with date and regex support on Windows.
- **Michel Valin**: For bug fixes relating to the configure scripts on IBM AIX platforms and instructions on compiling the example code under AIX.
- **Steve Christensen**: Providing prebuilt packages for SPARC/Solaris and x86/Solaris platforms on www.sunfreeware.com.
- **Jess Portnoy**: Reworking the rpm package and integrating argtable into Fedora Linux.
- **Michael Brown**: Incorporating support for pkg-config into the autoconf scripts.
- **Alexander Lindert**: For extensions to the parser to support hex, octal and binary integer formats as well as KB/MB/GB suffixes.
- **Rob Zaborowski**: Providing build configuration files for CMake.
- **Moczik Gabor**: For bug fixes relating to the parsing of filepaths and filename extensions.
