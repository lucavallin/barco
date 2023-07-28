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

void test_argint_basic_001(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;

    char* argv[] = {"program", "0", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->ival[0], 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 0);
    CuAssertTrue(tc, f->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_002(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;

    char* argv[] = {"program", "1", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 0);
    CuAssertTrue(tc, f->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_003(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertTrue(tc, b->count == 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 0);
    CuAssertTrue(tc, f->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_004(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;

    char* argv[] = {"program", "5", "7", "9", "-d", "-21", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->ival[0], 5);
    CuAssertTrue(tc, b->count == 1);
    CuAssertIntEquals(tc, b->ival[0], 7);
    CuAssertTrue(tc, c->count == 1);
    CuAssertIntEquals(tc, c->ival[0], 9);
    CuAssertTrue(tc, d->count == 1);
    CuAssertIntEquals(tc, d->ival[0], -21);
    CuAssertTrue(tc, e->count == 0);
    CuAssertTrue(tc, f->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_005(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;

    char* argv[] = {"program", "-d", "1", "-D2", "--delta", "3", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 3);
    CuAssertIntEquals(tc, d->ival[0], 1);
    CuAssertIntEquals(tc, d->ival[1], 2);
    CuAssertIntEquals(tc, d->ival[2], 3);
    CuAssertTrue(tc, e->count == 0);
    CuAssertTrue(tc, f->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_006(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", "4", "--eps", "-7", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertTrue(tc, b->count == 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertTrue(tc, c->count == 1);
    CuAssertIntEquals(tc, c->ival[0], 4);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 1);
    CuAssertIntEquals(tc, e->ival[0], -7);
    CuAssertTrue(tc, f->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_007(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", "4", "--eqn", "-7", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertTrue(tc, b->count == 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertTrue(tc, c->count == 1);
    CuAssertIntEquals(tc, c->ival[0], 4);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 1);
    CuAssertIntEquals(tc, e->ival[0], -7);
    CuAssertTrue(tc, f->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_008(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;

    char* argv[] = {"program", "1", "2", "3", "-D4", "--eps", "-10", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertTrue(tc, b->count == 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertTrue(tc, c->count == 1);
    CuAssertIntEquals(tc, c->ival[0], 3);
    CuAssertTrue(tc, d->count == 1);
    CuAssertIntEquals(tc, d->ival[0], 4);
    CuAssertTrue(tc, e->count == 1);
    CuAssertIntEquals(tc, e->ival[0], -10);
    CuAssertTrue(tc, f->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_009(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "-f", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, e->count == 0);
    CuAssertTrue(tc, f->count == 1);
    CuAssertIntEquals(tc, f->ival[0], -1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_010(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "-f", "1", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 1);
    CuAssertIntEquals(tc, f->ival[0], -1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_011(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "-f", "2", "--filler", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 2);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 2);
    CuAssertIntEquals(tc, f->ival[0], -1);
    CuAssertIntEquals(tc, f->ival[1], -1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_012(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "-f", "1", "--filler=2", "-f", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 3);
    CuAssertIntEquals(tc, f->ival[0], -1);
    CuAssertIntEquals(tc, f->ival[1], 2);
    CuAssertIntEquals(tc, f->ival[0], -1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_013(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0x0", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0x0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_014(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0x0", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0x0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_015(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0x10", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0x10);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_016(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0x10", "0x32", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0x10);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 0x32);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_017(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0x5", "0xA", "0xF", "-d", "-0x1E", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0x5);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 0xA);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 0xF);
    CuAssertIntEquals(tc, d->count, 1);
    CuAssertIntEquals(tc, d->ival[0], -0x1E);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_018(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "-d", "0xab", "-D0x09", "--delta", "0x02e", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 3);
    CuAssertIntEquals(tc, d->ival[0], 0xab);
    CuAssertIntEquals(tc, d->ival[1], 0x09);
    CuAssertIntEquals(tc, d->ival[2], 0x02e);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_019(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0o0", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_020(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0o10", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 010);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_021(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0o67", "0O23", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 067);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 023);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_022(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0o5", "0O0", "0x1", "-d", "-0o6", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 05);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 0);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 0x1);
    CuAssertIntEquals(tc, d->count, 1);
    CuAssertIntEquals(tc, d->ival[0], -06);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_023(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "-d", "0o012", "-D0o0777", "--delta", "0o56", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 3);
    CuAssertIntEquals(tc, d->ival[0], 012);
    CuAssertIntEquals(tc, d->ival[1], 0777);
    CuAssertIntEquals(tc, d->ival[2], 056);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_024(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0B0", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_025(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0B0", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_026(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0b10", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 2);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_027(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0B10110", "0b111001", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 22);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 57);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_028(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0B10110", "0b111001", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 22);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 57);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_029(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0b101001", "0b101", "0b00101010101", "-d", "0B110000011", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 41);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 5);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 341);
    CuAssertIntEquals(tc, d->count, 1);
    CuAssertIntEquals(tc, d->ival[0], 387);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_030(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "-d", "0b101", "-D0B11", "--delta", "0b11011", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 3);
    CuAssertIntEquals(tc, d->ival[0], 5);
    CuAssertIntEquals(tc, d->ival[1], 3);
    CuAssertIntEquals(tc, d->ival[2], 27);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_031(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "11", "0x11", "0o11", "-D0b11", "--eps", "-0o50", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 11);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 0x11);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 011);
    CuAssertIntEquals(tc, d->count, 1);
    CuAssertIntEquals(tc, d->ival[0], 3);
    CuAssertIntEquals(tc, e->count, 1);
    CuAssertIntEquals(tc, e->ival[0], -050);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_032(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1KB", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1024);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_033(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1MB", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1024 * 1024);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_034(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1GB", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1024 * 1024 * 1024);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_035(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0x5KB", "0xAMB", "0x1GB", "-d", "-0x40A01400", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 0);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 0x5 * 1024);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 0xA * 1024 * 1024);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 0x1 * 1024 * 1024 * 1024);
    CuAssertIntEquals(tc, d->count, 1);
    CuAssertIntEquals(tc, d->ival[0], -0x40A01400);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_036(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_037(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "2", "3", "4", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 3);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_038(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "2", "3", "-d1", "-d2", "-d3", "-d4", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 3);
    CuAssertIntEquals(tc, d->count, 3);
    CuAssertIntEquals(tc, d->ival[0], 1);
    CuAssertIntEquals(tc, d->ival[1], 2);
    CuAssertIntEquals(tc, d->ival[2], 3);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_039(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "2", "3", "-d1", "-d2", "-d3", "-d", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 3);
    CuAssertIntEquals(tc, d->count, 3);
    CuAssertIntEquals(tc, d->ival[0], 1);
    CuAssertIntEquals(tc, d->ival[1], 2);
    CuAssertIntEquals(tc, d->ival[2], 3);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_040(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "2", "3", "-d1", "-d2", "-d", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 3);
    CuAssertIntEquals(tc, d->count, 2);
    CuAssertIntEquals(tc, d->ival[0], 1);
    CuAssertIntEquals(tc, d->ival[1], 2);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_041(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "2", "3", "-d1", "-d", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 3);
    CuAssertIntEquals(tc, d->count, 1);
    CuAssertIntEquals(tc, d->ival[0], 1);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_042(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "2", "3", "-d", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 3);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_043(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "2", "3", "--eps", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_044(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1", "2", "3", "--eps", "3", "--eqn", "6", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 1);
    CuAssertIntEquals(tc, b->count, 1);
    CuAssertIntEquals(tc, b->ival[0], 2);
    CuAssertIntEquals(tc, c->count, 1);
    CuAssertIntEquals(tc, c->ival[0], 3);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 1);
    CuAssertIntEquals(tc, e->ival[0], 3);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_045(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "hello", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_046(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1.234", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 0);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_047(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "4", "hello", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 4);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_048(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "5", "1.234", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);
    CuAssertIntEquals(tc, a->count, 1);
    CuAssertIntEquals(tc, a->ival[0], 5);
    CuAssertIntEquals(tc, b->count, 0);
    CuAssertIntEquals(tc, c->count, 0);
    CuAssertIntEquals(tc, d->count, 0);
    CuAssertIntEquals(tc, e->count, 0);
    CuAssertIntEquals(tc, f->count, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_049(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "-f", "2", "--filler=", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 2);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_050(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0x0g", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_051(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0o08", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_052(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "0b02", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_053(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1000GB", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argint_basic_054(CuTest* tc) {
    struct arg_int* a = arg_int1(NULL, NULL, "a", "a is <int>");
    struct arg_int* b = arg_int0(NULL, NULL, "b", "b is <int>");
    struct arg_int* c = arg_int0(NULL, NULL, "c", "c is <int>");
    struct arg_int* d = arg_intn("dD", "delta", "<int>", 0, 3, "d can occur 0..3 times");
    struct arg_int* e = arg_int0(NULL, "eps,eqn", "<int>", "eps is optional");
    struct arg_int* f = arg_intn("fF", "filler", "<int>", 0, 3, "f can occur 0..3 times");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, e, f, end};
    int nerrors;
    int i;

    char* argv[] = {"program", "1GBH", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i = 0; i < f->hdr.maxcount; i++)
        f->ival[i] = -1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertIntEquals(tc, nerrors, 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

CuSuite* get_argint_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_argint_basic_001);
    SUITE_ADD_TEST(suite, test_argint_basic_002);
    SUITE_ADD_TEST(suite, test_argint_basic_003);
    SUITE_ADD_TEST(suite, test_argint_basic_004);
    SUITE_ADD_TEST(suite, test_argint_basic_005);
    SUITE_ADD_TEST(suite, test_argint_basic_006);
    SUITE_ADD_TEST(suite, test_argint_basic_007);
    SUITE_ADD_TEST(suite, test_argint_basic_008);
    SUITE_ADD_TEST(suite, test_argint_basic_009);
    SUITE_ADD_TEST(suite, test_argint_basic_010);
    SUITE_ADD_TEST(suite, test_argint_basic_011);
    SUITE_ADD_TEST(suite, test_argint_basic_012);
    SUITE_ADD_TEST(suite, test_argint_basic_013);
    SUITE_ADD_TEST(suite, test_argint_basic_014);
    SUITE_ADD_TEST(suite, test_argint_basic_015);
    SUITE_ADD_TEST(suite, test_argint_basic_016);
    SUITE_ADD_TEST(suite, test_argint_basic_017);
    SUITE_ADD_TEST(suite, test_argint_basic_018);
    SUITE_ADD_TEST(suite, test_argint_basic_019);
    SUITE_ADD_TEST(suite, test_argint_basic_020);
    SUITE_ADD_TEST(suite, test_argint_basic_021);
    SUITE_ADD_TEST(suite, test_argint_basic_022);
    SUITE_ADD_TEST(suite, test_argint_basic_023);
    SUITE_ADD_TEST(suite, test_argint_basic_024);
    SUITE_ADD_TEST(suite, test_argint_basic_025);
    SUITE_ADD_TEST(suite, test_argint_basic_026);
    SUITE_ADD_TEST(suite, test_argint_basic_027);
    SUITE_ADD_TEST(suite, test_argint_basic_028);
    SUITE_ADD_TEST(suite, test_argint_basic_029);
    SUITE_ADD_TEST(suite, test_argint_basic_030);
    SUITE_ADD_TEST(suite, test_argint_basic_031);
    SUITE_ADD_TEST(suite, test_argint_basic_032);
    SUITE_ADD_TEST(suite, test_argint_basic_033);
    SUITE_ADD_TEST(suite, test_argint_basic_034);
    SUITE_ADD_TEST(suite, test_argint_basic_035);
    SUITE_ADD_TEST(suite, test_argint_basic_036);
    SUITE_ADD_TEST(suite, test_argint_basic_037);
    SUITE_ADD_TEST(suite, test_argint_basic_038);
    SUITE_ADD_TEST(suite, test_argint_basic_039);
    SUITE_ADD_TEST(suite, test_argint_basic_040);
    SUITE_ADD_TEST(suite, test_argint_basic_041);
    SUITE_ADD_TEST(suite, test_argint_basic_042);
    SUITE_ADD_TEST(suite, test_argint_basic_043);
    SUITE_ADD_TEST(suite, test_argint_basic_044);
    SUITE_ADD_TEST(suite, test_argint_basic_045);
    SUITE_ADD_TEST(suite, test_argint_basic_046);
    SUITE_ADD_TEST(suite, test_argint_basic_047);
    SUITE_ADD_TEST(suite, test_argint_basic_048);
    SUITE_ADD_TEST(suite, test_argint_basic_049);
    SUITE_ADD_TEST(suite, test_argint_basic_050);
    SUITE_ADD_TEST(suite, test_argint_basic_051);
    SUITE_ADD_TEST(suite, test_argint_basic_052);
    SUITE_ADD_TEST(suite, test_argint_basic_053);
    SUITE_ADD_TEST(suite, test_argint_basic_054);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
