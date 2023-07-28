#include <assert.h>
#include <math.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CuTest.h"

/*-------------------------------------------------------------------------*
 * CuStr
 *-------------------------------------------------------------------------*/

char* CuStrAlloc(size_t size) {
    char* newStr = (char*)malloc(sizeof(char) * (size));
    return newStr;
}

char* CuStrCopy(const char* old) {
    size_t len = strlen(old);
    char* newStr = CuStrAlloc(len + 1);
#if (defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)) || (defined(__STDC_SECURE_LIB__) && defined(__STDC_WANT_SECURE_LIB__))
    strcpy_s(newStr, len + 1, old);
#else
    strcpy(newStr, old);
#endif

    return newStr;
}

/*-------------------------------------------------------------------------*
 * CuString
 *-------------------------------------------------------------------------*/

void CuStringInit(CuString* str) {
    str->length = 0;
    str->size = STRING_MAX;
    str->buffer = (char*)malloc(sizeof(char) * str->size);
    str->buffer[0] = '\0';
}

CuString* CuStringNew(void) {
    CuString* str = (CuString*)malloc(sizeof(CuString));
    str->length = 0;
    str->size = STRING_MAX;
    str->buffer = (char*)malloc(sizeof(char) * str->size);
    str->buffer[0] = '\0';
    return str;
}

void CuStringDelete(CuString* str) {
    if (!str)
        return;
    free(str->buffer);
    free(str);
}

void CuStringResize(CuString* str, size_t newSize) {
    str->buffer = (char*)realloc(str->buffer, sizeof(char) * newSize);
    str->size = newSize;
}

void CuStringAppend(CuString* str, const char* text) {
    size_t length;

    if (text == NULL) {
        text = "NULL";
    }

    length = strlen(text);
    if (str->length + length + 1 >= str->size)
        CuStringResize(str, str->length + length + 1 + STRING_INC);
    str->length += length;
#if (defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)) || (defined(__STDC_SECURE_LIB__) && defined(__STDC_WANT_SECURE_LIB__))
    strcat_s(str->buffer, str->size, text);
#else
    strcat(str->buffer, text);
#endif
}

void CuStringAppendChar(CuString* str, char ch) {
    char text[2];
    text[0] = ch;
    text[1] = '\0';
    CuStringAppend(str, text);
}

void CuStringAppendFormat(CuString* str, const char* format, ...) {
    va_list argp;
    char buf[HUGE_STRING_LEN];
    va_start(argp, format);
#if (defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)) || (defined(__STDC_SECURE_LIB__) && defined(__STDC_WANT_SECURE_LIB__))
    vsprintf_s(buf, sizeof(buf), format, argp);
#else
    vsprintf(buf, format, argp);
#endif

    va_end(argp);
    CuStringAppend(str, buf);
}

void CuStringInsert(CuString* str, const char* text, int pos) {
    size_t length = strlen(text);
    if ((size_t)pos > str->length)
        pos = (int)str->length;
    if (str->length + length + 1 >= str->size)
        CuStringResize(str, str->length + length + 1 + STRING_INC);
    memmove(str->buffer + pos + length, str->buffer + pos, (str->length - pos) + 1);
    str->length += length;
    memcpy(str->buffer + pos, text, length);
}

/*-------------------------------------------------------------------------*
 * CuTest
 *-------------------------------------------------------------------------*/

void CuTestInit(CuTest* t, const char* name, TestFunction function) {
    t->name = CuStrCopy(name);
    t->failed = 0;
    t->ran = 0;
    t->message = NULL;
    t->function = function;
    t->jumpBuf = NULL;
}

CuTest* CuTestNew(const char* name, TestFunction function) {
    CuTest* tc = CU_ALLOC(CuTest);
    CuTestInit(tc, name, function);
    return tc;
}

void CuTestDelete(CuTest* t) {
    if (!t)
        return;
    free(t->name);
    free(t);
}

void CuTestRun(CuTest* tc) {
    jmp_buf buf;
    tc->jumpBuf = &buf;
    if (setjmp(buf) == 0) {
        tc->ran = 1;
        (tc->function)(tc);
    }
    tc->jumpBuf = 0;
}

static void CuFailInternal(CuTest* tc, const char* file, int line, CuString* string) {
    char buf[HUGE_STRING_LEN];

#if (defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)) || (defined(__STDC_SECURE_LIB__) && defined(__STDC_WANT_SECURE_LIB__))
    sprintf_s(buf, sizeof(buf), "%s:%d: ", file, line);
#else
    sprintf(buf, "%s:%d: ", file, line);
#endif
    CuStringInsert(string, buf, 0);

    tc->failed = 1;
    tc->message = string->buffer;
    if (tc->jumpBuf != 0)
        longjmp(*(tc->jumpBuf), 0);
}

void CuFail_Line(CuTest* tc, const char* file, int line, const char* message2, const char* message) {
    CuString string;

    CuStringInit(&string);
    if (message2 != NULL) {
        CuStringAppend(&string, message2);
        CuStringAppend(&string, ": ");
    }
    CuStringAppend(&string, message);
    CuFailInternal(tc, file, line, &string);
}

void CuAssert_Line(CuTest* tc, const char* file, int line, const char* message, int condition) {
    if (condition)
        return;
    CuFail_Line(tc, file, line, NULL, message);
}

void CuAssertStrEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message, const char* expected, const char* actual) {
    CuString string;
    if ((expected == NULL && actual == NULL) || (expected != NULL && actual != NULL && strcmp(expected, actual) == 0)) {
        return;
    }

    CuStringInit(&string);
    if (message != NULL) {
        CuStringAppend(&string, message);
        CuStringAppend(&string, ": ");
    }
    CuStringAppend(&string, "expected <");
    CuStringAppend(&string, expected);
    CuStringAppend(&string, "> but was <");
    CuStringAppend(&string, actual);
    CuStringAppend(&string, ">");
    CuFailInternal(tc, file, line, &string);
}

void CuAssertIntEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message, int expected, int actual) {
    char buf[STRING_MAX];
    if (expected == actual)
        return;
#if (defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)) || (defined(__STDC_SECURE_LIB__) && defined(__STDC_WANT_SECURE_LIB__))
    sprintf_s(buf, sizeof(buf), "expected <%d> but was <%d>", expected, actual);
#else
    sprintf(buf, "expected <%d> but was <%d>", expected, actual);
#endif
    CuFail_Line(tc, file, line, message, buf);
}

void CuAssertDblEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message, double expected, double actual, double delta) {
    char buf[STRING_MAX];
    if (fabs(expected - actual) <= delta)
        return;
#if (defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)) || (defined(__STDC_SECURE_LIB__) && defined(__STDC_WANT_SECURE_LIB__))
    sprintf_s(buf, sizeof(buf), "expected <%f> but was <%f>", expected, actual);
#else
    sprintf(buf, "expected <%f> but was <%f>", expected, actual);
#endif
    CuFail_Line(tc, file, line, message, buf);
}

void CuAssertPtrEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message, void* expected, void* actual) {
    char buf[STRING_MAX];
    if (expected == actual)
        return;
#if (defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)) || (defined(__STDC_SECURE_LIB__) && defined(__STDC_WANT_SECURE_LIB__))
    sprintf_s(buf, sizeof(buf), "expected pointer <0x%p> but was <0x%p>", expected, actual);
#else
    sprintf(buf, "expected pointer <0x%p> but was <0x%p>", expected, actual);
#endif
    CuFail_Line(tc, file, line, message, buf);
}

/*-------------------------------------------------------------------------*
 * CuSuite
 *-------------------------------------------------------------------------*/

void CuSuiteInit(CuSuite* testSuite) {
    testSuite->count = 0;
    testSuite->failCount = 0;
    memset(testSuite->list, 0, sizeof(testSuite->list));
}

CuSuite* CuSuiteNew(void) {
    CuSuite* testSuite = CU_ALLOC(CuSuite);
    CuSuiteInit(testSuite);
    return testSuite;
}

void CuSuiteDelete(CuSuite* testSuite) {
    unsigned int n;
    for (n = 0; n < MAX_TEST_CASES; n++) {
        if (testSuite->list[n]) {
            CuTestDelete(testSuite->list[n]);
        }
    }
    free(testSuite);
}

void CuSuiteAdd(CuSuite* testSuite, CuTest* testCase) {
    assert(testSuite->count < MAX_TEST_CASES);
    testSuite->list[testSuite->count] = testCase;
    testSuite->count++;
}

void CuSuiteAddSuite(CuSuite* testSuite, CuSuite* testSuite2) {
    int i;
    for (i = 0; i < testSuite2->count; ++i) {
        CuTest* testCase = testSuite2->list[i];
        CuSuiteAdd(testSuite, testCase);
    }
    free(testSuite2);
}

void CuSuiteRun(CuSuite* testSuite) {
    int i;
    for (i = 0; i < testSuite->count; ++i) {
        CuTest* testCase = testSuite->list[i];
        CuTestRun(testCase);
        if (testCase->failed) {
            testSuite->failCount += 1;
        }
    }
}

void CuSuiteSummary(CuSuite* testSuite, CuString* summary) {
    int i;
    for (i = 0; i < testSuite->count; ++i) {
        CuTest* testCase = testSuite->list[i];
        CuStringAppend(summary, testCase->failed ? "F" : ".");
    }
    CuStringAppend(summary, "\n\n");
}

void CuSuiteDetails(CuSuite* testSuite, CuString* details) {
    int i;
    int failCount = 0;

    if (testSuite->failCount == 0) {
        int passCount = testSuite->count - testSuite->failCount;
        const char* testWord = passCount == 1 ? "test" : "tests";
        CuStringAppendFormat(details, "OK (%d %s)\n", passCount, testWord);
    } else {
        if (testSuite->failCount == 1)
            CuStringAppend(details, "There was 1 failure:\n");
        else
            CuStringAppendFormat(details, "There were %d failures:\n", testSuite->failCount);

        for (i = 0; i < testSuite->count; ++i) {
            CuTest* testCase = testSuite->list[i];
            if (testCase->failed) {
                failCount++;
                CuStringAppendFormat(details, "%d) %s: %s\n", failCount, testCase->name, testCase->message);
            }
        }
        CuStringAppend(details, "\n!!!FAILURES!!!\n");

        CuStringAppendFormat(details, "Runs: %d ", testSuite->count);
        CuStringAppendFormat(details, "Passes: %d ", testSuite->count - testSuite->failCount);
        CuStringAppendFormat(details, "Fails: %d\n", testSuite->failCount);
    }
}
