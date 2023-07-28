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

set(TEST_PUBLIC_SRC_FILES
  testall.c
  testarglit.c
  testargstr.c
  testargint.c
  testargdate.c
  testargdbl.c
  testargfile.c
  testargrex.c
  testargdstr.c
  testargcmd.c
  CuTest.c
)

set(TEST_SRC_FILES
  ${TEST_PUBLIC_SRC_FILES}
  testarghashtable.c
)

if(UNIX)
  set(ARGTABLE3_EXTRA_LIBS m)
endif()

if(BUILD_SHARED_LIBS)
  add_executable(test_shared ${TEST_PUBLIC_SRC_FILES})
  target_compile_definitions(test_shared PRIVATE -DARGTABLE3_TEST_PUBLIC_ONLY)
  target_include_directories(test_shared PRIVATE ${PROJECT_SOURCE_DIR}/src)
  target_link_libraries(test_shared argtable3 ${ARGTABLE3_EXTRA_LIBS})
  add_custom_command(TARGET test_shared POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:argtable3>"
    "$<TARGET_FILE_DIR:test_shared>"
  )

  add_test(NAME test_shared COMMAND "$<TARGET_FILE:test_shared>")
else()
  add_executable(test_static ${TEST_SRC_FILES})
  target_include_directories(test_static PRIVATE ${PROJECT_SOURCE_DIR}/src)
  target_link_libraries(test_static argtable3 ${ARGTABLE3_EXTRA_LIBS})

  add_test(NAME test_static COMMAND "$<TARGET_FILE:test_static>")
endif()

add_executable(test_src ${TEST_SRC_FILES} ${ARGTABLE3_SRC_FILES})
target_include_directories(test_src PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(test_src ${ARGTABLE3_EXTRA_LIBS})

add_custom_command(OUTPUT ${ARGTABLE3_AMALGAMATION_SRC_FILE}
  COMMAND "${PROJECT_SOURCE_DIR}/tools/build" dist
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools"
)

add_executable(test_amalgamation ${TEST_SRC_FILES} ${ARGTABLE3_AMALGAMATION_SRC_FILE})
target_include_directories(test_amalgamation PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(test_amalgamation ${ARGTABLE3_EXTRA_LIBS})
add_custom_command(TARGET test_amalgamation PRE_BUILD
  COMMAND "${PROJECT_SOURCE_DIR}/tools/build" dist
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools"
)

add_test(NAME test_src COMMAND "$<TARGET_FILE:test_src>")
add_test(NAME test_amalgamation COMMAND "$<TARGET_FILE:test_amalgamation>")
