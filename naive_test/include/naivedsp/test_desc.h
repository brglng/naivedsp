#ifndef __NAIVE_TEST_DESC_H__
#define __NAIVE_TEST_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/memory.h"
#include "naivedsp/typedefs.h"

typedef enum {
    NAIVE_TEST_CASE_DESC_VALUE_INT,
    NAIVE_TEST_CASE_DESC_VALUE_I32,
    NAIVE_TEST_CASE_DESC_VALUE_U32,
    NAIVE_TEST_CASE_DESC_VALUE_F32,
    NAIVE_TEST_CASE_DESC_VALUE_BOOL,
    NAIVE_TEST_CASE_DESC_VALUE_STR,
} NaiveTestCaseDescValueType;

typedef struct {
    NaiveTestCaseDescValueType type;
    union {
        int             int_;
        NaiveI32          i32;
        NaiveF32          f32;
        NaiveBool         bool_;
        NAIVE_CONST char  *str;
    } value;
} NaiveTestCaseDescValue;

NAIVE_INLINE NaiveTestCaseDescValue naive_test_case_desc_value_int(int int_)
{
    NaiveTestCaseDescValue value;
    value.type = NAIVE_TEST_CASE_DESC_VALUE_INT;
    value.value.int_ = int_;
    return value;
}

NAIVE_INLINE NaiveTestCaseDescValue naive_test_case_desc_value_i32(NaiveI32 i32)
{
    NaiveTestCaseDescValue value;
    value.type = NAIVE_TEST_CASE_DESC_VALUE_I32;
    value.value.i32 = i32;
    return value;
}

NAIVE_INLINE NaiveTestCaseDescValue naive_test_case_desc_value_f32(NaiveF32 f32)
{
    NaiveTestCaseDescValue value;
    value.type = NAIVE_TEST_CASE_DESC_VALUE_F32;
    value.value.f32 = f32;
    return value;
}

NAIVE_INLINE NaiveTestCaseDescValue naive_test_case_desc_value_bool(NaiveBool bool_)
{
    NaiveTestCaseDescValue value;
    value.type = NAIVE_TEST_CASE_DESC_VALUE_BOOL;
    value.value.bool_ = bool_;
    return value;
}

NAIVE_INLINE NaiveTestCaseDescValue naive_test_case_desc_value_str(NAIVE_CONST char *str)
{
    NaiveTestCaseDescValue value;
    value.type = NAIVE_TEST_CASE_DESC_VALUE_STR;
    value.value.str = str;
    return value;
}

enum {
    NAIVE_TEST_CASE_DESC_KEY_INPUT = 0,
    NAIVE_TEST_CASE_DESC_KEY_OUTPUT,
    NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE,
    NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE,
    NAIVE_TEST_CASE_DESC_KEY_USER_BEGIN,
};

typedef struct {
    int                     key;
    NaiveTestCaseDescValue    value;
} NaiveTestCaseDescKeyValue;

typedef struct {
    NAIVE_CONST char              *name;
    NaiveI32                      cap;
    NaiveI32                      len;
    NaiveTestCaseDescKeyValue     *keyvals;
} NaiveTestCaseDesc;

NaiveResult naive_test_case_desc_init(NaiveTestCaseDesc *self, NaiveAllocFunc *alloc, void *allocator, NaiveI32 cap, NAIVE_CONST char *name);

NaiveResult naive_test_case_desc_set(NaiveTestCaseDesc *self, int key, NaiveTestCaseDescValue value);
NaiveResult naive_test_case_desc_set_int(NaiveTestCaseDesc *self, int key, int value);
NaiveResult naive_test_case_desc_set_i32(NaiveTestCaseDesc *self, int key, NaiveI32 value);
NaiveResult naive_test_case_desc_set_f32(NaiveTestCaseDesc *self, int key, NaiveF32 value);
NaiveResult naive_test_case_desc_set_bool(NaiveTestCaseDesc *self, int key, NaiveBool value);
NaiveResult naive_test_case_desc_set_str(NaiveTestCaseDesc *self, int key, NAIVE_CONST char *value);

NAIVE_CONST NaiveTestCaseDescValue *naive_test_case_desc_get(NAIVE_CONST NaiveTestCaseDesc *self, int key);

int             naive_test_case_desc_get_int(NAIVE_CONST NaiveTestCaseDesc *self, int key);
NaiveI32          naive_test_case_desc_get_i32(NAIVE_CONST NaiveTestCaseDesc *self, int key);
NaiveF32          naive_test_case_desc_get_f32(NAIVE_CONST NaiveTestCaseDesc *self, int key);
NaiveBool         naive_test_case_desc_get_bool(NAIVE_CONST NaiveTestCaseDesc *self, int key);
NAIVE_CONST char* naive_test_case_desc_get_str(NAIVE_CONST NaiveTestCaseDesc *self, int key);

typedef struct {
    NaiveI32              cap;
    NaiveI32              len;
    NaiveTestCaseDesc     *cases;
} NaiveTestDesc;

NaiveResult naive_test_desc_init(NaiveTestDesc *self, NaiveAllocFunc *alloc, void *allocator, NaiveI32 cap);
NaiveResult naive_test_desc_add_case(NaiveTestDesc *self, NaiveTestCaseDesc **p_case);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_TEST_DESC_H__ */
