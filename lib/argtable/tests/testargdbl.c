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

#include <float.h>
#include <string.h>

#include "CuTest.h"
#include "argtable3.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4204)
#endif

void test_argdbl_basic_001(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "0", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertDblEquals(tc, a->dval[0], 0, DBL_EPSILON);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_002(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1.234", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertDblEquals(tc, a->dval[0], 1.234, DBL_EPSILON);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_003(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1.8", "2.3", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertDblEquals(tc, a->dval[0], 1.8, DBL_EPSILON);
    CuAssertTrue(tc, b->count == 1);
    CuAssertDblEquals(tc, b->dval[0], 2.3, DBL_EPSILON);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_004(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "5", "7", "9", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertDblEquals(tc, a->dval[0], 5, DBL_EPSILON);
    CuAssertTrue(tc, b->count == 1);
    CuAssertDblEquals(tc, b->dval[0], 7, DBL_EPSILON);
    CuAssertTrue(tc, c->count == 1);
    CuAssertDblEquals(tc, c->dval[0], 9, DBL_EPSILON);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_005(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1.9998", "-d", "13e-1", "-D", "17e-1", "--delta", "36e-1", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertDblEquals(tc, a->dval[0], 1.9998, DBL_EPSILON);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 3);
    CuAssertDblEquals(tc, d->dval[0], 13e-1, DBL_EPSILON);
    CuAssertDblEquals(tc, d->dval[1], 17e-1, DBL_EPSILON);
    CuAssertDblEquals(tc, d->dval[2], 36e-1, DBL_EPSILON);
    CuAssertTrue(tc, e->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_006(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1.2", "2.3", "4.5", "--eps", "8.3456789", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertDblEquals(tc, a->dval[0], 1.2, DBL_EPSILON);
    CuAssertTrue(tc, b->count == 1);
    CuAssertDblEquals(tc, b->dval[0], 2.3, DBL_EPSILON);
    CuAssertTrue(tc, c->count == 1);
    CuAssertDblEquals(tc, c->dval[0], 4.5, DBL_EPSILON);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 1);
    CuAssertDblEquals(tc, e->dval[0], 8.3456789, DBL_EPSILON);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_007(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1.2", "2.3", "4.5", "--eqn", "8.3456789", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertDblEquals(tc, a->dval[0], 1.2, DBL_EPSILON);
    CuAssertTrue(tc, b->count == 1);
    CuAssertDblEquals(tc, b->dval[0], 2.3, DBL_EPSILON);
    CuAssertTrue(tc, c->count == 1);
    CuAssertDblEquals(tc, c->dval[0], 4.5, DBL_EPSILON);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 1);
    CuAssertDblEquals(tc, e->dval[0], 8.3456789, DBL_EPSILON);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_008(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1.2", "2.3", "4.5", "--eqn", "8.345", "-D", "0.234", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 0);

    CuAssertTrue(tc, a->count == 1);
    CuAssertDblEquals(tc, a->dval[0], 1.2, DBL_EPSILON);
    CuAssertTrue(tc, b->count == 1);
    CuAssertDblEquals(tc, b->dval[0], 2.3, DBL_EPSILON);
    CuAssertTrue(tc, c->count == 1);
    CuAssertDblEquals(tc, c->dval[0], 4.5, DBL_EPSILON);
    CuAssertTrue(tc, d->count == 1);
    CuAssertDblEquals(tc, d->dval[0], 0.234, DBL_EPSILON);
    CuAssertTrue(tc, e->count == 1);
    CuAssertDblEquals(tc, e->dval[0], 8.345, DBL_EPSILON);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_009(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", "3", "4", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_010(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", "3", "4", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_011(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", "3", "-d1", "-d2", "-d3", "-d4", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_012(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", "3", "--eps", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_013(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", "3", "--eps", "3", "--eqn", "6", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_014(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "hello", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdbl_basic_015(CuTest* tc) {
    struct arg_dbl* a = arg_dbl1(NULL, NULL, "a", "a is <double>");
    struct arg_dbl* b = arg_dbl0(NULL, NULL, "b", "b is <double>");
    struct arg_dbl* c = arg_dbl0(NULL, NULL, "c", "c is <double>");
    struct arg_dbl* d = arg_dbln("dD", "delta", "<double>", 0, 3, "d can occur 0..3 times");
    struct arg_dbl* e = arg_dbl0(NULL, "eps,eqn", "<double>", "eps is optional");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, end};
    int nerrors;

    char* argv[] = {"program", "4", "hello", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

CuSuite* get_argdbl_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_argdbl_basic_001);
    SUITE_ADD_TEST(suite, test_argdbl_basic_002);
    SUITE_ADD_TEST(suite, test_argdbl_basic_003);
    SUITE_ADD_TEST(suite, test_argdbl_basic_004);
    SUITE_ADD_TEST(suite, test_argdbl_basic_005);
    SUITE_ADD_TEST(suite, test_argdbl_basic_006);
    SUITE_ADD_TEST(suite, test_argdbl_basic_007);
    SUITE_ADD_TEST(suite, test_argdbl_basic_008);
    SUITE_ADD_TEST(suite, test_argdbl_basic_009);
    SUITE_ADD_TEST(suite, test_argdbl_basic_010);
    SUITE_ADD_TEST(suite, test_argdbl_basic_011);
    SUITE_ADD_TEST(suite, test_argdbl_basic_012);
    SUITE_ADD_TEST(suite, test_argdbl_basic_013);
    SUITE_ADD_TEST(suite, test_argdbl_basic_014);
    SUITE_ADD_TEST(suite, test_argdbl_basic_015);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
