/*******************************************************************************
 * This file is part of the argtable3 library.
 *
 * Copyright (C) 2013-2019 Tom G. Huang
 * <tomghuang@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of STEWART HEITMANN nor the  names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL STEWART HEITMANN BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include <string.h>
#include <time.h>

#include <stdio.h>

#include "CuTest.h"
#include "argtable3.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4204)
#endif

int cmd1_proc(int argc, char* argv[], arg_dstr_t res) {
    if (argc == 0) {
        arg_dstr_catf(res, "cmd1 fail");
        return 1;
    }

    arg_dstr_catf(res, "%d %s", argc, argv[0]);
    return 0;
}

void test_argcmd_basic_001(CuTest* tc) {
    arg_cmd_init();
    CuAssertIntEquals(tc, 0, arg_cmd_count());
    
    arg_cmd_register("cmd1", cmd1_proc, "description of cmd1");
    CuAssertIntEquals(tc, 1, arg_cmd_count());

    char* argv[] = {
            "cmd1",
            "-o",
            "file1",
    };
    int argc = 3;
    CuAssertTrue(tc, strcmp(argv[0], "cmd1") == 0);
    CuAssertTrue(tc, strcmp(argv[1], "-o") == 0);
    CuAssertTrue(tc, strcmp(argv[2], "file1") == 0);

    arg_dstr_t res = arg_dstr_create();
    int err = arg_cmd_dispatch("cmd1", argc, argv, res);
    CuAssertIntEquals(tc, 0, err);
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(res), "3 cmd1") == 0);

    arg_dstr_reset(res);
    err = arg_cmd_dispatch("cmd1", 0, NULL, res);
    CuAssertIntEquals(tc, 1, err);
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(res), "cmd1 fail") == 0);

    arg_dstr_destroy(res);
    arg_cmd_uninit();
}

CuSuite* get_argcmd_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_argcmd_basic_001);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
