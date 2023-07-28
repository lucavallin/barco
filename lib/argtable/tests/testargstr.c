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

void test_argstr_basic_001(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--hello=string1", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 2);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->sval[0], "string1");
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_002(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-cstring1", "-Dstring2", "-dstring3", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "string1");
    CuAssertTrue(tc, d->count == 2);
    CuAssertStrEquals(tc, d->sval[0], "string2");
    CuAssertStrEquals(tc, d->sval[1], "string3");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_003(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Cstring1", "--delta=string2", "--delta=string3", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "string1");
    CuAssertTrue(tc, d->count == 2);
    CuAssertStrEquals(tc, d->sval[0], "string2");
    CuAssertStrEquals(tc, d->sval[1], "string3");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_004(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "--delta=string1", "-cstring2", "-Dstring3", "-bstring4", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "string4");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "string2");
    CuAssertTrue(tc, d->count == 2);
    CuAssertStrEquals(tc, d->sval[0], "string1");
    CuAssertStrEquals(tc, d->sval[1], "string3");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_005(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Dstring1", "-Bstring2", "--delta=string3", "-Cstring4", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "string2");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "string4");
    CuAssertTrue(tc, d->count == 2);
    CuAssertStrEquals(tc, d->sval[0], "string1");
    CuAssertStrEquals(tc, d->sval[1], "string3");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_006(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Dstring1", "-Bstring2", "--delta=string3", "-Cstring4", "--hello=string5", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->sval[0], "string5");
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "string2");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "string4");
    CuAssertTrue(tc, d->count == 2);
    CuAssertStrEquals(tc, d->sval[0], "string1");
    CuAssertStrEquals(tc, d->sval[1], "string3");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_007(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Dstring1", "-Bstring2", "--delta=string3", "-Cstring4", "--world=string5", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->sval[0], "string5");
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "string2");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "string4");
    CuAssertTrue(tc, d->count == 2);
    CuAssertStrEquals(tc, d->sval[0], "string1");
    CuAssertStrEquals(tc, d->sval[1], "string3");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_008(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-cstring1", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "string1");
    CuAssertTrue(tc, d->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_009(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Dstring1", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 2);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 1);
    CuAssertStrEquals(tc, d->sval[0], "string1");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_010(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Cstring1", "-Dstring2", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "string1");
    CuAssertTrue(tc, d->count == 1);
    CuAssertStrEquals(tc, d->sval[0], "string2");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_011(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Dstring1", "-dstring2", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 2);
    CuAssertStrEquals(tc, d->sval[0], "string1");
    CuAssertStrEquals(tc, d->sval[1], "string2");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_012(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-cs1", "-ds2", "-ds3", "-ds4", "-ds5", "-ds6", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "s1");
    CuAssertTrue(tc, d->count == 4);
    CuAssertStrEquals(tc, d->sval[0], "s2");
    CuAssertStrEquals(tc, d->sval[1], "s3");
    CuAssertStrEquals(tc, d->sval[2], "s4");
    CuAssertStrEquals(tc, d->sval[3], "s5");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_013(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-cs1", "-cs2", "-ds3", "-ds4", "-ds5", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "s1");
    CuAssertTrue(tc, d->count == 3);
    CuAssertStrEquals(tc, d->sval[0], "s3");
    CuAssertStrEquals(tc, d->sval[1], "s4");
    CuAssertStrEquals(tc, d->sval[2], "s5");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_014(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Cs1", "-ds2", "-Ds3", "--delta=s4", "-bs5", "-Bs6", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->sval[0], "s5");
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "s1");
    CuAssertTrue(tc, d->count == 3);
    CuAssertStrEquals(tc, d->sval[0], "s2");
    CuAssertStrEquals(tc, d->sval[1], "s3");
    CuAssertStrEquals(tc, d->sval[2], "s4");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_015(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Cs1", "-ds2", "-Ds3", "--delta=s4", "--hello=s5", "--world=s6", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->sval[0], "s5");
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "s1");
    CuAssertTrue(tc, d->count == 3);
    CuAssertStrEquals(tc, d->sval[0], "s2");
    CuAssertStrEquals(tc, d->sval[1], "s3");
    CuAssertStrEquals(tc, d->sval[2], "s4");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argstr_basic_016(CuTest* tc) {
    struct arg_str* a = arg_str0(NULL, "hello,world", "STRVAL", "either --hello or --world or none");
    struct arg_str* b = arg_str0("bB", NULL, "STRVAL", "either -b or -B or none");
    struct arg_str* c = arg_str1("cC", NULL, "STRVAL", "either -c or -C");
    struct arg_str* d = arg_strn("dD", "delta", "STRVAL", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, end};
    int nerrors;

    char* argv[] = {"program", "-Cs1", "-ds2", "-Ds3", "--delta=s4", "--hello=s5", "X", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->sval[0], "s5");
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertStrEquals(tc, c->sval[0], "s1");
    CuAssertTrue(tc, d->count == 3);
    CuAssertStrEquals(tc, d->sval[0], "s2");
    CuAssertStrEquals(tc, d->sval[1], "s3");
    CuAssertStrEquals(tc, d->sval[2], "s4");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

CuSuite* get_argstr_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_argstr_basic_001);
    SUITE_ADD_TEST(suite, test_argstr_basic_002);
    SUITE_ADD_TEST(suite, test_argstr_basic_003);
    SUITE_ADD_TEST(suite, test_argstr_basic_004);
    SUITE_ADD_TEST(suite, test_argstr_basic_005);
    SUITE_ADD_TEST(suite, test_argstr_basic_006);
    SUITE_ADD_TEST(suite, test_argstr_basic_007);
    SUITE_ADD_TEST(suite, test_argstr_basic_008);
    SUITE_ADD_TEST(suite, test_argstr_basic_009);
    SUITE_ADD_TEST(suite, test_argstr_basic_010);
    SUITE_ADD_TEST(suite, test_argstr_basic_011);
    SUITE_ADD_TEST(suite, test_argstr_basic_012);
    SUITE_ADD_TEST(suite, test_argstr_basic_013);
    SUITE_ADD_TEST(suite, test_argstr_basic_014);
    SUITE_ADD_TEST(suite, test_argstr_basic_015);
    SUITE_ADD_TEST(suite, test_argstr_basic_016);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
