################################################################################
# This file is part of the argtable3 library.
#
# Copyright (C) 2016-2021 Tom G. Huang
# <tomghuang@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of STEWART HEITMANN nor the  names of its contributors
#       may be used to endorse or promote products derived from this software
#       without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL STEWART HEITMANN BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################

if(ARGTABLE3_ENABLE_ARG_REX_DEBUG)
  add_definitions(-DARG_REX_DEBUG)
endif()

if(NOT ARGTABLE3_REPLACE_GETOPT)
  add_definitions(-DARG_REPLACE_GETOPT=0)
endif()

if(ARGTABLE3_LONG_ONLY)
  add_definitions(-DARG_LONG_ONLY)
endif()

file(GLOB EXAMPLES_SOURCES RELATIVE ${PROJECT_SOURCE_DIR}/examples *.c)

if(UNIX)
  set(ARGTABLE3_EXTRA_LIBS m)
endif()

foreach(examples_src ${EXAMPLES_SOURCES})
  string(REPLACE ".c" "" examplename ${examples_src})
  add_executable(${examplename} ${PROJECT_SOURCE_DIR}/examples/${examples_src})
  target_include_directories(${examplename} PRIVATE ${PROJECT_SOURCE_DIR}/src)
  target_link_libraries(${examplename} argtable3 ${ARGTABLE3_EXTRA_LIBS})
endforeach()
