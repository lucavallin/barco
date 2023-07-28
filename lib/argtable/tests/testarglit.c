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

void test_arglit_basic_001(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "--help", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 2);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, help->count == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_002(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-cDd", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 2);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_003(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-cdDd", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 3);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_004(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-CDd", "--delta", "--delta", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 4);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_005(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "--delta", "-cD", "-b", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 2);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_006(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-D", "-B", "--delta", "-C", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 2);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_007(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-D", "-B", "--delta", "-C", "--hello", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertTrue(tc, b->count == 1);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 2);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_008(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-D", "-B", "--delta", "-C", "--world", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0)
        arg_print_errors(stdout, end, argv[0]);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertTrue(tc, b->count == 1);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 2);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_009(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-c", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 0);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_010(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-D", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 2);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 1);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_011(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-CD", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 1);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_012(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-Dd", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 0);
    CuAssertTrue(tc, d->count == 2);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_013(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-cddddd", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 4);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_014(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-ccddd", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 3);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_015(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-C", "-d", "-D", "--delta", "-b", "-B", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 3);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_016(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-C", "-d", "-D", "--delta", "--hello", "--world", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 1);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 3);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_arglit_basic_017(CuTest* tc) {
    struct arg_lit* a = arg_lit0(NULL, "hello,world", "either --hello or --world or none");
    struct arg_lit* b = arg_lit0("bB", NULL, "either -b or -B or none");
    struct arg_lit* c = arg_lit1("cC", NULL, "either -c or -C");
    struct arg_lit* d = arg_litn("dD", "delta", 2, 4, "-d|-D|--delta 2..4 occurences");
    struct arg_lit* help = arg_lit0(NULL, "help", "print this help and exit");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, d, help, end};
    int nerrors;

    char* argv[] = {"program", "-C", "-d", "-D", "--delta", "--hello", "X", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 1);
    CuAssertTrue(tc, a->count == 1);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertTrue(tc, d->count == 3);
    CuAssertTrue(tc, help->count == 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

CuSuite* get_arglit_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_arglit_basic_001);
    SUITE_ADD_TEST(suite, test_arglit_basic_002);
    SUITE_ADD_TEST(suite, test_arglit_basic_003);
    SUITE_ADD_TEST(suite, test_arglit_basic_004);
    SUITE_ADD_TEST(suite, test_arglit_basic_005);
    SUITE_ADD_TEST(suite, test_arglit_basic_006);
    SUITE_ADD_TEST(suite, test_arglit_basic_007);
    SUITE_ADD_TEST(suite, test_arglit_basic_008);
    SUITE_ADD_TEST(suite, test_arglit_basic_009);
    SUITE_ADD_TEST(suite, test_arglit_basic_010);
    SUITE_ADD_TEST(suite, test_arglit_basic_011);
    SUITE_ADD_TEST(suite, test_arglit_basic_012);
    SUITE_ADD_TEST(suite, test_arglit_basic_013);
    SUITE_ADD_TEST(suite, test_arglit_basic_014);
    SUITE_ADD_TEST(suite, test_arglit_basic_015);
    SUITE_ADD_TEST(suite, test_arglit_basic_016);
    SUITE_ADD_TEST(suite, test_arglit_basic_017);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
