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

#include "CuTest.h"
#include "argtable3.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4204)
#endif

void test_argrex_basic_001(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--beta", "world", "goodbye", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "world");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "goodbye");
    CuAssertTrue(tc, d->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_002(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "GoodBye", "--beta", "World", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "World");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "GoodBye");
    CuAssertTrue(tc, d->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_003(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--beta", "world", "GOODBYE", "GoodBye", "gOoDbyE", "Anything", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "world");
    CuAssertTrue(tc, c->count == 3);
    CuAssertStrEquals(tc, c->sval[0], "GOODBYE");
    CuAssertStrEquals(tc, c->sval[1], "GoodBye");
    CuAssertStrEquals(tc, c->sval[2], "gOoDbyE");
    CuAssertTrue(tc, d->count == 1);
    CuAssertStrEquals(tc, d->sval[0], "Anything");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_004(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--beta", "world", "GOODBYE", "AnyHow", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "world");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "GOODBYE");
    CuAssertTrue(tc, d->count == 1);
    CuAssertStrEquals(tc, d->sval[0], "AnyHow");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_005(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-a", "hello", "--beta", "world", "GOODBYE", "AnyHow", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->sval[0], "hello");
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "world");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "GOODBYE");
    CuAssertTrue(tc, d->count == 1);
    CuAssertStrEquals(tc, d->sval[0], "AnyHow");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_006(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--beta", "WORLD", "goodbye", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_007(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--beta", "World", "goodby", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_008(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--beta", "world", "GoodBye", "Goodbye", "gOoDbyE", "Anything", "goodbye", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_009(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--beta", "world", "GoodBye", "Goodbye", "gOoDbyE", "Anything", "Anytime", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argrex_basic_010(CuTest* tc) {
    struct arg_rex* a = arg_rex0("a", NULL, "hello", NULL, 0, "blah blah");
    struct arg_rex* b = arg_rex1(NULL, "beta", "[Ww]orld", NULL, 0, "blah blah");
    struct arg_rex* c = arg_rexn(NULL, NULL, "goodbye", NULL, 1, 5, ARG_REX_ICASE, "blah blah");
    struct arg_rex* d = arg_rex0(NULL, NULL, "any.*", NULL, ARG_REX_ICASE, "blah blah");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--beta", "world", "GoodBye", "Goodbye", "Anything", "-a", "Hello", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

CuSuite* get_argrex_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_argrex_basic_001);
    SUITE_ADD_TEST(suite, test_argrex_basic_002);
    SUITE_ADD_TEST(suite, test_argrex_basic_003);
    SUITE_ADD_TEST(suite, test_argrex_basic_004);
    SUITE_ADD_TEST(suite, test_argrex_basic_005);
    SUITE_ADD_TEST(suite, test_argrex_basic_006);
    SUITE_ADD_TEST(suite, test_argrex_basic_007);
    SUITE_ADD_TEST(suite, test_argrex_basic_008);
    SUITE_ADD_TEST(suite, test_argrex_basic_009);
    SUITE_ADD_TEST(suite, test_argrex_basic_010);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
