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

#include "CuTest.h"
#include "argtable3.h"

/*
    printf("tm_sec   = %d\n", c->tmval->tm_sec);
    printf("tm_min   = %d\n", c->tmval->tm_min);
    printf("tm_hour  = %d\n", c->tmval->tm_hour);
    printf("tm_mday  = %d\n", c->tmval->tm_mday);
    printf("tm_mon   = %d\n", c->tmval->tm_mon);
    printf("tm_year  = %d\n", c->tmval->tm_year);
    printf("tm_wday  = %d\n", c->tmval->tm_wday);
    printf("tm_yday  = %d\n", c->tmval->tm_yday);
    printf("tm_isdst = %d\n", c->tmval->tm_isdst);

*/

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4204)
#endif

void test_argdate_basic_001(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "23:59", "--date", "12/31/04", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, a->tmval->tm_min, 59);
    CuAssertIntEquals(tc, a->tmval->tm_hour, 23);
    CuAssertIntEquals(tc, a->tmval->tm_mday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_mon, 0);
    CuAssertIntEquals(tc, a->tmval->tm_year, 0);
    CuAssertIntEquals(tc, a->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_isdst, 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertIntEquals(tc, c->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, c->tmval->tm_min, 0);
    CuAssertIntEquals(tc, c->tmval->tm_hour, 0);
    CuAssertIntEquals(tc, c->tmval->tm_mday, 31);
    CuAssertIntEquals(tc, c->tmval->tm_mon, 11);
    CuAssertIntEquals(tc, c->tmval->tm_year, 104);
    CuAssertIntEquals(tc, c->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, c->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, c->tmval->tm_isdst, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_002(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "--date", "12/31/04", "20:15", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, a->tmval->tm_min, 15);
    CuAssertIntEquals(tc, a->tmval->tm_hour, 20);
    CuAssertIntEquals(tc, a->tmval->tm_mday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_mon, 0);
    CuAssertIntEquals(tc, a->tmval->tm_year, 0);
    CuAssertIntEquals(tc, a->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_isdst, 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 1);
    CuAssertIntEquals(tc, c->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, c->tmval->tm_min, 0);
    CuAssertIntEquals(tc, c->tmval->tm_hour, 0);
    CuAssertIntEquals(tc, c->tmval->tm_mday, 31);
    CuAssertIntEquals(tc, c->tmval->tm_mon, 11);
    CuAssertIntEquals(tc, c->tmval->tm_year, 104);
    CuAssertIntEquals(tc, c->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, c->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, c->tmval->tm_isdst, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_003(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "--date", "12/31/04", "20:15", "--date", "06/07/84", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, a->tmval->tm_min, 15);
    CuAssertIntEquals(tc, a->tmval->tm_hour, 20);
    CuAssertIntEquals(tc, a->tmval->tm_mday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_mon, 0);
    CuAssertIntEquals(tc, a->tmval->tm_year, 0);
    CuAssertIntEquals(tc, a->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_isdst, 0);
    CuAssertTrue(tc, b->count == 0);
    CuAssertTrue(tc, c->count == 2);
    CuAssertIntEquals(tc, c->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, c->tmval->tm_min, 0);
    CuAssertIntEquals(tc, c->tmval->tm_hour, 0);
    CuAssertIntEquals(tc, c->tmval->tm_mday, 31);
    CuAssertIntEquals(tc, c->tmval->tm_mon, 11);
    CuAssertIntEquals(tc, c->tmval->tm_year, 104);
    CuAssertIntEquals(tc, c->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, c->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, c->tmval->tm_isdst, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_sec, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_min, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_hour, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_mday, 7);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_mon, 5);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_year, 84);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_wday, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_yday, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_isdst, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_004(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "--date", "12/31/04", "20:15", "-b", "1982-11-28", "--date", "06/07/84", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);

    CuAssertTrue(tc, nerrors == 0);
    CuAssertTrue(tc, a->count == 1);
    CuAssertIntEquals(tc, a->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, a->tmval->tm_min, 15);
    CuAssertIntEquals(tc, a->tmval->tm_hour, 20);
    CuAssertIntEquals(tc, a->tmval->tm_mday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_mon, 0);
    CuAssertIntEquals(tc, a->tmval->tm_year, 0);
    CuAssertIntEquals(tc, a->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, a->tmval->tm_isdst, 0);
    CuAssertTrue(tc, b->count == 1);
    CuAssertIntEquals(tc, b->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, b->tmval->tm_min, 0);
    CuAssertIntEquals(tc, b->tmval->tm_hour, 0);
    CuAssertIntEquals(tc, b->tmval->tm_mday, 28);
    CuAssertIntEquals(tc, b->tmval->tm_mon, 10);
    CuAssertIntEquals(tc, b->tmval->tm_year, 82);
    CuAssertIntEquals(tc, b->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, b->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, b->tmval->tm_isdst, 0);
    CuAssertTrue(tc, c->count == 2);
    CuAssertIntEquals(tc, c->tmval->tm_sec, 0);
    CuAssertIntEquals(tc, c->tmval->tm_min, 0);
    CuAssertIntEquals(tc, c->tmval->tm_hour, 0);
    CuAssertIntEquals(tc, c->tmval->tm_mday, 31);
    CuAssertIntEquals(tc, c->tmval->tm_mon, 11);
    CuAssertIntEquals(tc, c->tmval->tm_year, 104);
    CuAssertIntEquals(tc, c->tmval->tm_wday, 0);
    CuAssertIntEquals(tc, c->tmval->tm_yday, 0);
    CuAssertIntEquals(tc, c->tmval->tm_isdst, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_sec, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_min, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_hour, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_mday, 7);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_mon, 5);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_year, 84);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_wday, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_yday, 0);
    CuAssertIntEquals(tc, (c->tmval + 1)->tm_isdst, 0);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_005(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 2);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_006(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "25:59", "--date", "12/31/04", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_007(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "23:59", "--date", "12/32/04", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_008(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "23:59", "--date", "12/31/04", "22:58", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_009(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "--date", "12/31/04", "20:15", "--date", "26/07/84", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void test_argdate_basic_010(CuTest* tc) {
    struct arg_date* a = arg_date1(NULL, NULL, "%H:%M", NULL, "time 23:59");
    struct arg_date* b = arg_date0("b", NULL, "%Y-%m-%d", NULL, "date YYYY-MM-DD");
    struct arg_date* c = arg_daten(NULL, "date", "%D", NULL, 1, 2, "MM/DD/YY");
    struct arg_end* end = arg_end(20);
    void* argtable[] = {a, b, c, end};
    int nerrors;

    char* argv[] = {"program", "-b", "1982-11-28", "-b", "1976-11-11", "--date", "12/07/84", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    CuAssertTrue(tc, arg_nullcheck(argtable) == 0);

    nerrors = arg_parse(argc, argv, argtable);
    CuAssertTrue(tc, nerrors == 1);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

CuSuite* get_argdate_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_argdate_basic_001);
    SUITE_ADD_TEST(suite, test_argdate_basic_002);
    SUITE_ADD_TEST(suite, test_argdate_basic_003);
    SUITE_ADD_TEST(suite, test_argdate_basic_004);
    SUITE_ADD_TEST(suite, test_argdate_basic_005);
    SUITE_ADD_TEST(suite, test_argdate_basic_006);
    SUITE_ADD_TEST(suite, test_argdate_basic_007);
    SUITE_ADD_TEST(suite, test_argdate_basic_008);
    SUITE_ADD_TEST(suite, test_argdate_basic_009);
    SUITE_ADD_TEST(suite, test_argdate_basic_010);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
