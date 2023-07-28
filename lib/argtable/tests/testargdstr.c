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

void test_argdstr_basic_001(CuTest* tc) {
    arg_dstr_t ds = arg_dstr_create();

    arg_dstr_set(ds, "hello ", ARG_DSTR_VOLATILE);
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(ds), "hello ") == 0);

    arg_dstr_cat(ds, "world");
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(ds), "hello world") == 0);

    arg_dstr_destroy(ds);
}

void test_argdstr_basic_002(CuTest* tc) {
    arg_dstr_t ds = arg_dstr_create();

    arg_dstr_set(ds, "hello world", ARG_DSTR_VOLATILE);
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(ds), "hello world") == 0);

    arg_dstr_reset(ds);
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(ds), "") == 0);

    arg_dstr_set(ds, "good", ARG_DSTR_VOLATILE);
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(ds), "good") == 0);

    arg_dstr_destroy(ds);
}

void test_argdstr_basic_003(CuTest* tc) {
    arg_dstr_t ds = arg_dstr_create();
    arg_dstr_cat(ds, "hello world");
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(ds), "hello world") == 0);

    arg_dstr_destroy(ds);
}

void test_argdstr_basic_004(CuTest* tc) {
    arg_dstr_t ds = arg_dstr_create();
    arg_dstr_catf(ds, "%s %d", "hello world", 1);
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(ds), "hello world 1") == 0);

    arg_dstr_destroy(ds);
}

void test_argdstr_basic_005(CuTest* tc) {
    arg_dstr_t ds = arg_dstr_create();
    arg_dstr_catf(ds, "%d.", 1);
    arg_dstr_catf(ds, "%d.", 2);
    arg_dstr_catf(ds, "%d.", 3);
    arg_dstr_cat(ds, "456");
    CuAssertTrue(tc, strcmp(arg_dstr_cstr(ds), "1.2.3.456") == 0);

    arg_dstr_destroy(ds);
}

void test_argdstr_basic_006(CuTest* tc) {
    int i;

    arg_dstr_t ds = arg_dstr_create();
    for (i = 0; i < 100000; i++) {
        arg_dstr_catf(ds, "%s", "1234567890");
    }
    CuAssertTrue(tc, strlen(arg_dstr_cstr(ds)) == 1000000);

    arg_dstr_destroy(ds);
}

CuSuite* get_argdstr_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_argdstr_basic_001);
    SUITE_ADD_TEST(suite, test_argdstr_basic_002);
    SUITE_ADD_TEST(suite, test_argdstr_basic_003);
    SUITE_ADD_TEST(suite, test_argdstr_basic_004);
    SUITE_ADD_TEST(suite, test_argdstr_basic_005);
    SUITE_ADD_TEST(suite, test_argdstr_basic_006);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
