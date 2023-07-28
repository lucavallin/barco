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
#include "argtable3_private.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4204)
#pragma warning(disable : 4996)
#endif

static unsigned int hash_key(const void* key) {
    char* str = (char*)key;
    int c;
    unsigned int hash = 5381;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static int equal_keys(const void* key1, const void* key2) {
    char* k1 = (char*)key1;
    char* k2 = (char*)key2;
    return (0 == strcmp(k1, k2));
}

void test_arghashtable_basic_001(CuTest* tc) {
    arg_hashtable_t* h = arg_hashtable_create(32, hash_key, equal_keys);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, arg_hashtable_count(h), 0);

    arg_hashtable_destroy(h, 1);
}

void test_arghashtable_basic_002(CuTest* tc) {
    arg_hashtable_t* h = arg_hashtable_create(32, hash_key, equal_keys);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, arg_hashtable_count(h), 0);

    char* key_1 = "k1";
    char* k_1 = (char*)malloc(strlen(key_1) + 1);
    memset(k_1, 0, strlen(key_1) + 1);
    strncpy(k_1, key_1, strlen(key_1));

    char* value_1 = "v1";
    char* v_1 = (char*)malloc(strlen(value_1) + 1);
    memset(v_1, 0, strlen(value_1) + 1);
    strncpy(v_1, value_1, strlen(value_1));

    arg_hashtable_insert(h, k_1, v_1);
    CuAssertIntEquals(tc, 1, arg_hashtable_count(h));

    arg_hashtable_itr_t* itr = arg_hashtable_itr_create(h);
    CuAssertTrue(tc, itr != 0);
    CuAssertPtrEquals(tc, k_1, arg_hashtable_itr_key(itr));
    CuAssertTrue(tc, strcmp((char*)arg_hashtable_itr_key(itr), key_1) == 0);
    CuAssertPtrEquals(tc, v_1, arg_hashtable_itr_value(itr));
    CuAssertTrue(tc, strcmp((char*)arg_hashtable_itr_value(itr), value_1) == 0);

    arg_hashtable_itr_destroy(itr);
    arg_hashtable_destroy(h, 1);
}

void test_arghashtable_basic_003(CuTest* tc) {
    arg_hashtable_t* h = arg_hashtable_create(32, hash_key, equal_keys);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, arg_hashtable_count(h), 0);

    char* key_1 = "k1";
    char* k_1 = (char*)malloc(strlen(key_1) + 1);
    memset(k_1, 0, strlen(key_1) + 1);
    strncpy(k_1, key_1, strlen(key_1));

    char* value_1 = "v1";
    char* v_1 = (char*)malloc(strlen(value_1) + 1);
    memset(v_1, 0, strlen(value_1) + 1);
    strncpy(v_1, value_1, strlen(value_1));

    arg_hashtable_insert(h, k_1, v_1);
    CuAssertIntEquals(tc, 1, arg_hashtable_count(h));

    char* key_2 = "k2";
    char* k_2 = (char*)malloc(strlen(key_2) + 1);
    memset(k_2, 0, strlen(key_2) + 1);
    strncpy(k_2, key_2, strlen(key_2));

    char* value_2 = "v2";
    char* v_2 = (char*)malloc(strlen(value_2) + 1);
    memset(v_2, 0, strlen(value_2) + 1);
    strncpy(v_2, value_2, strlen(value_2));

    arg_hashtable_insert(h, k_2, v_2);
    CuAssertIntEquals(tc, 2, arg_hashtable_count(h));

    arg_hashtable_itr_t* itr = arg_hashtable_itr_create(h);
    CuAssertTrue(tc, itr != 0);

    int ret = arg_hashtable_itr_advance(itr);
    CuAssertTrue(tc, ret != 0);

    ret = arg_hashtable_itr_advance(itr);
    CuAssertTrue(tc, ret == 0);

    arg_hashtable_itr_destroy(itr);
    arg_hashtable_destroy(h, 1);
}

void test_arghashtable_basic_004(CuTest* tc) {
    arg_hashtable_t* h = arg_hashtable_create(32, hash_key, equal_keys);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, arg_hashtable_count(h), 0);

    char* key_1 = "k1";
    char* k_1 = (char*)malloc(strlen(key_1) + 1);
    memset(k_1, 0, strlen(key_1) + 1);
    strncpy(k_1, key_1, strlen(key_1));

    char* value_1 = "v1";
    char* v_1 = (char*)malloc(strlen(value_1) + 1);
    memset(v_1, 0, strlen(value_1) + 1);
    strncpy(v_1, value_1, strlen(value_1));

    arg_hashtable_insert(h, k_1, v_1);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, 1, arg_hashtable_count(h));

    arg_hashtable_itr_t* itr = arg_hashtable_itr_create(h);
    int ret = arg_hashtable_itr_remove(itr);
    CuAssertTrue(tc, ret == 0);
    CuAssertIntEquals(tc, 0, arg_hashtable_count(h));

    arg_hashtable_itr_destroy(itr);
    arg_hashtable_destroy(h, 1);
}

void test_arghashtable_basic_005(CuTest* tc) {
    arg_hashtable_t* h = arg_hashtable_create(3, hash_key, equal_keys);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, arg_hashtable_count(h), 0);

    char* key_1 = "k1";
    char* k_1 = (char*)malloc(strlen(key_1) + 1);
    memset(k_1, 0, strlen(key_1) + 1);
    strncpy(k_1, key_1, strlen(key_1));

    char* value_1 = "v1";
    char* v_1 = (char*)malloc(strlen(value_1) + 1);
    memset(v_1, 0, strlen(value_1) + 1);
    strncpy(v_1, value_1, strlen(value_1));

    arg_hashtable_insert(h, k_1, v_1);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, 1, arg_hashtable_count(h));

    arg_hashtable_remove(h, k_1);
    CuAssertIntEquals(tc, 0, arg_hashtable_count(h));

    arg_hashtable_destroy(h, 1);
}

void test_arghashtable_basic_006(CuTest* tc) {
    arg_hashtable_t* h = arg_hashtable_create(32, hash_key, equal_keys);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, arg_hashtable_count(h), 0);

    char* key_1 = "k1";
    char* k_1 = (char*)malloc(strlen(key_1) + 1);
    memset(k_1, 0, strlen(key_1) + 1);
    strncpy(k_1, key_1, strlen(key_1));

    char* value_1 = "v1";
    char* v_1 = (char*)malloc(strlen(value_1) + 1);
    memset(v_1, 0, strlen(value_1) + 1);
    strncpy(v_1, value_1, strlen(value_1));

    arg_hashtable_insert(h, k_1, v_1);
    CuAssertTrue(tc, arg_hashtable_count(h) == 1);

    char* vv = (char*)arg_hashtable_search(h, k_1);
    CuAssertTrue(tc, strcmp(vv, v_1) == 0);

    arg_hashtable_destroy(h, 1);
}

void test_arghashtable_basic_007(CuTest* tc) {
    arg_hashtable_t* h = arg_hashtable_create(32, hash_key, equal_keys);
    CuAssertTrue(tc, h != 0);
    CuAssertIntEquals(tc, arg_hashtable_count(h), 0);

    char* key_1 = "k1";
    char* k_1 = (char*)malloc(strlen(key_1) + 1);
    memset(k_1, 0, strlen(key_1) + 1);
    strncpy(k_1, key_1, strlen(key_1));

    char* value_1 = "v1";
    char* v_1 = (char*)malloc(strlen(value_1) + 1);
    memset(v_1, 0, strlen(value_1) + 1);
    strncpy(v_1, value_1, strlen(value_1));

    arg_hashtable_insert(h, k_1, v_1);
    CuAssertIntEquals(tc, 1, arg_hashtable_count(h));

    char* key_2 = "k2";
    char* k_2 = (char*)malloc(strlen(key_2) + 1);
    memset(k_2, 0, strlen(key_2) + 1);
    strncpy(k_2, key_2, strlen(key_2));

    char* value_2 = "v2";
    char* v_2 = (char*)malloc(strlen(value_2) + 1);
    memset(v_2, 0, strlen(value_2) + 1);
    strncpy(v_2, value_2, strlen(value_2));

    arg_hashtable_insert(h, k_2, v_2);
    CuAssertIntEquals(tc, 2, arg_hashtable_count(h));

    arg_hashtable_itr_t itr;
    int ret = arg_hashtable_itr_search(&itr, h, k_1);
    CuAssertTrue(tc, ret != 0);
    CuAssertPtrEquals(tc, k_1, arg_hashtable_itr_key(&itr));
    CuAssertPtrEquals(tc, v_1, arg_hashtable_itr_value(&itr));
    CuAssertTrue(tc, strcmp((char*)arg_hashtable_itr_key(&itr), k_1) == 0);
    CuAssertTrue(tc, strcmp((char*)arg_hashtable_itr_value(&itr), v_1) == 0);

    arg_hashtable_destroy(h, 1);
}

CuSuite* get_arghashtable_testsuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_arghashtable_basic_001);
    SUITE_ADD_TEST(suite, test_arghashtable_basic_002);
    SUITE_ADD_TEST(suite, test_arghashtable_basic_003);
    SUITE_ADD_TEST(suite, test_arghashtable_basic_004);
    SUITE_ADD_TEST(suite, test_arghashtable_basic_005);
    SUITE_ADD_TEST(suite, test_arghashtable_basic_006);
    SUITE_ADD_TEST(suite, test_arghashtable_basic_007);
    return suite;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
