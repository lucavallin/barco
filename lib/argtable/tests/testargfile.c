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

void test_argfile_basic_001(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "foo.bar", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "foo.bar");
    CuAssertStrEquals(tc, a->basename[0], "foo.bar");
    CuAssertStrEquals(tc, a->extension[0], ".bar");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_002(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/foo.bar", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/foo.bar");
    CuAssertStrEquals(tc, a->basename[0], "foo.bar");
    CuAssertStrEquals(tc, a->extension[0], ".bar");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_003(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "./foo.bar", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "./foo.bar");
    CuAssertStrEquals(tc, a->basename[0], "foo.bar");
    CuAssertStrEquals(tc, a->extension[0], ".bar");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_004(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "././foo.bar", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "././foo.bar");
    CuAssertStrEquals(tc, a->basename[0], "foo.bar");
    CuAssertStrEquals(tc, a->extension[0], ".bar");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_005(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "./././foo.bar", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "./././foo.bar");
    CuAssertStrEquals(tc, a->basename[0], "foo.bar");
    CuAssertStrEquals(tc, a->extension[0], ".bar");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_006(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "../foo.bar", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "../foo.bar");
    CuAssertStrEquals(tc, a->basename[0], "foo.bar");
    CuAssertStrEquals(tc, a->extension[0], ".bar");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_007(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "../../foo.bar", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "../../foo.bar");
    CuAssertStrEquals(tc, a->basename[0], "foo.bar");
    CuAssertStrEquals(tc, a->extension[0], ".bar");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_008(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "foo");
    CuAssertStrEquals(tc, a->basename[0], "foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_009(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/foo");
    CuAssertStrEquals(tc, a->basename[0], "foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_010(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "./foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "./foo");
    CuAssertStrEquals(tc, a->basename[0], "foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_011(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "././foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "././foo");
    CuAssertStrEquals(tc, a->basename[0], "foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_012(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "./././foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "./././foo");
    CuAssertStrEquals(tc, a->basename[0], "foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_013(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "../foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "../foo");
    CuAssertStrEquals(tc, a->basename[0], "foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_014(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "../../foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "../../foo");
    CuAssertStrEquals(tc, a->basename[0], "foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_015(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", ".foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], ".foo");
    CuAssertStrEquals(tc, a->basename[0], ".foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_016(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/.foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/.foo");
    CuAssertStrEquals(tc, a->basename[0], ".foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_017(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "./.foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "./.foo");
    CuAssertStrEquals(tc, a->basename[0], ".foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_018(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "../.foo", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "../.foo");
    CuAssertStrEquals(tc, a->basename[0], ".foo");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_019(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "foo.", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "foo.");
    CuAssertStrEquals(tc, a->basename[0], "foo.");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_020(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/foo.", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/foo.");
    CuAssertStrEquals(tc, a->basename[0], "foo.");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_021(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "./foo.", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "./foo.");
    CuAssertStrEquals(tc, a->basename[0], "foo.");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_022(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "../foo.", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "../foo.");
    CuAssertStrEquals(tc, a->basename[0], "foo.");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_023(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/.foo.", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/.foo.");
    CuAssertStrEquals(tc, a->basename[0], ".foo.");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_024(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/.foo.c", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/.foo.c");
    CuAssertStrEquals(tc, a->basename[0], ".foo.c");
    CuAssertStrEquals(tc, a->extension[0], ".c");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_025(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/.foo..b.c", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/.foo..b.c");
    CuAssertStrEquals(tc, a->basename[0], ".foo..b.c");
    CuAssertStrEquals(tc, a->extension[0], ".c");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_026(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_027(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", ".", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], ".");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_028(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "..", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "..");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_029(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/.", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/.");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_030(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "/..", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "/..");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_031(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "./", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "./");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_032(CuTest* tc) {
    struct arg_file* a = arg_file1(NULL, NULL, "<file>", "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "../", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "../");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

#ifdef WIN32
void test_argfile_basic_033(CuTest* tc) {
    struct arg_file* a = arg_filen(NULL, NULL, "<file>", 0, 3, "filename to test");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, end};
    int nerrors;

    char* argv[] = {"program", "C:\\test folder\\", "C:\\test folder2", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 2);
    CuAssertStrEquals(tc, a->filename[0], "C:\\test folder\\");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");
    CuAssertStrEquals(tc, a->filename[1], "C:\\test folder2");
    CuAssertStrEquals(tc, a->basename[1], "test folder2");
    CuAssertStrEquals(tc, a->extension[1], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argfile_basic_034(CuTest* tc) {
    struct arg_file* a = arg_filen(NULL, NULL, "<file>", 1, 1, "path a");
    struct arg_file* b = arg_filen(NULL, NULL, "<file>", 1, 1, "path b");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, end};
    int nerrors;

    char* argv[] = {"program", "C:\\test folder\\", "C:\\test folder2", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);
    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertStrEquals(tc, a->filename[0], "C:\\test folder\\");
    CuAssertStrEquals(tc, a->basename[0], "");
    CuAssertStrEquals(tc, a->extension[0], "");
    CuAssertTrue(tc, b->count == 1);
    CuAssertStrEquals(tc, b->filename[0], "C:\\test folder2");
    CuAssertStrEquals(tc, b->basename[0], "test folder2");
    CuAssertStrEquals(tc, b->extension[0], "");

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}
#endif /* #ifdef WIN32 */

CuSuite* get_argfile_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_argfile_basic_001);
    SUITE_ADD_TEST(suite, test_argfile_basic_002);
    SUITE_ADD_TEST(suite, test_argfile_basic_003);
    SUITE_ADD_TEST(suite, test_argfile_basic_004);
    SUITE_ADD_TEST(suite, test_argfile_basic_005);
    SUITE_ADD_TEST(suite, test_argfile_basic_006);
    SUITE_ADD_TEST(suite, test_argfile_basic_007);
    SUITE_ADD_TEST(suite, test_argfile_basic_008);
    SUITE_ADD_TEST(suite, test_argfile_basic_009);
    SUITE_ADD_TEST(suite, test_argfile_basic_010);
    SUITE_ADD_TEST(suite, test_argfile_basic_011);
    SUITE_ADD_TEST(suite, test_argfile_basic_012);
    SUITE_ADD_TEST(suite, test_argfile_basic_013);
    SUITE_ADD_TEST(suite, test_argfile_basic_014);
    SUITE_ADD_TEST(suite, test_argfile_basic_015);
    SUITE_ADD_TEST(suite, test_argfile_basic_016);
    SUITE_ADD_TEST(suite, test_argfile_basic_017);
    SUITE_ADD_TEST(suite, test_argfile_basic_018);
    SUITE_ADD_TEST(suite, test_argfile_basic_019);
    SUITE_ADD_TEST(suite, test_argfile_basic_020);
    SUITE_ADD_TEST(suite, test_argfile_basic_021);
    SUITE_ADD_TEST(suite, test_argfile_basic_022);
    SUITE_ADD_TEST(suite, test_argfile_basic_023);
    SUITE_ADD_TEST(suite, test_argfile_basic_024);
    SUITE_ADD_TEST(suite, test_argfile_basic_025);
    SUITE_ADD_TEST(suite, test_argfile_basic_026);
    SUITE_ADD_TEST(suite, test_argfile_basic_027);
    SUITE_ADD_TEST(suite, test_argfile_basic_028);
    SUITE_ADD_TEST(suite, test_argfile_basic_029);
    SUITE_ADD_TEST(suite, test_argfile_basic_030);
    SUITE_ADD_TEST(suite, test_argfile_basic_031);
    SUITE_ADD_TEST(suite, test_argfile_basic_032);
#ifdef WIN32
    SUITE_ADD_TEST(suite, test_argfile_basic_033);
    SUITE_ADD_TEST(suite, test_argfile_basic_034);
#endif /* #ifdef WIN32 */

    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
