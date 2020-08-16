#include <string.h>
#include "naivedsp/test_desc.h"
#include "naivedsp/assert.h"

NaiveResult naive_test_case_desc_init(NaiveTestCaseDesc *self, NaiveAllocFunc *alloc, void *allocator, NaiveI32 cap, NAIVE_CONST char *name)
{
    self->name = name;
    self->cap = cap;
    self->len = 0;
    self->keyvals = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveTestCaseDescKeyValue), sizeof(NaiveTestCaseDescKeyValue) * cap);

    if (self->keyvals)
        return NAIVE_OK;
    else
        return NAIVE_ERR_NOMEM;
}

NaiveResult naive_test_case_desc_set(NaiveTestCaseDesc *self, int key, NaiveTestCaseDescValue value)
{
    NaiveTestCaseDescKeyValue *keyval = NULL;

    for (NaiveI32 i = 0; i < self->len; ++i) {
        if (self->keyvals[i].key == key) {
            keyval = &self->keyvals[i];
            break;
        }
    }

    if (!keyval) {
        if (self->len < self->cap) {
            keyval = &self->keyvals[self->len];
            keyval->key = key;
            self->len++;
        } else {
            return NAIVE_ERR_NOMEM;
        }
    }

    keyval->value = value;

    return NAIVE_OK;
}

NaiveResult naive_test_case_desc_set_int(NaiveTestCaseDesc *self, int key, int value)
{
    return naive_test_case_desc_set(self, key, naive_test_case_desc_value_int(value));
}

NaiveResult naive_test_case_desc_set_i32(NaiveTestCaseDesc *self, int key, NaiveI32 value)
{
    return naive_test_case_desc_set(self, key, naive_test_case_desc_value_i32(value));
}

NaiveResult naive_test_case_desc_set_f32(NaiveTestCaseDesc *self, int key, NaiveF32 value)
{
    return naive_test_case_desc_set(self, key, naive_test_case_desc_value_f32(value));
}

NaiveResult naive_test_case_desc_set_bool(NaiveTestCaseDesc *self, int key, NaiveBool value)
{
    return naive_test_case_desc_set(self, key, naive_test_case_desc_value_bool(value));
}

NaiveResult naive_test_case_desc_set_str(NaiveTestCaseDesc *self, int key, NAIVE_CONST char *value)
{
    return naive_test_case_desc_set(self, key, naive_test_case_desc_value_str(value));
}

NAIVE_CONST NaiveTestCaseDescValue *naive_test_case_desc_get(
    NAIVE_CONST NaiveTestCaseDesc *self, int key)
{
    for (NaiveI32 i = 0; i < self->len; ++i) {
        NAIVE_CONST NaiveTestCaseDescKeyValue *keyval = &self->keyvals[i];
        if (keyval->key == key) {
            return &keyval->value;
        }
    }
    return NULL;
}

int naive_test_case_desc_get_int(NAIVE_CONST NaiveTestCaseDesc *self, int key)
{
    NAIVE_CONST NaiveTestCaseDescValue *value = naive_test_case_desc_get(self, key);
    NAIVE_ASSERT(value && value->type == NAIVE_TEST_CASE_DESC_VALUE_INT);
    return value->value.int_;
}

NaiveI32 naive_test_case_desc_get_i32(NAIVE_CONST NaiveTestCaseDesc *self, int key)
{
    NAIVE_CONST NaiveTestCaseDescValue *value = naive_test_case_desc_get(self, key);
    NAIVE_ASSERT(value && value->type == NAIVE_TEST_CASE_DESC_VALUE_I32);
    return value->value.i32;
}

NaiveF32 naive_test_case_desc_get_f32(NAIVE_CONST NaiveTestCaseDesc *self, int key)
{
    NAIVE_CONST NaiveTestCaseDescValue *value = naive_test_case_desc_get(self, key);
    NAIVE_ASSERT(value && value->type == NAIVE_TEST_CASE_DESC_VALUE_F32);
    return value->value.f32;
}

NaiveBool naive_test_case_desc_get_bool(NAIVE_CONST NaiveTestCaseDesc *self, int key)
{
    NAIVE_CONST NaiveTestCaseDescValue *value = naive_test_case_desc_get(self, key);
    NAIVE_ASSERT(value && value->type == NAIVE_TEST_CASE_DESC_VALUE_BOOL);
    return value->value.bool_;
}

NAIVE_CONST char* naive_test_case_desc_get_str(NAIVE_CONST NaiveTestCaseDesc *self, int key)
{
    NAIVE_CONST NaiveTestCaseDescValue *value = naive_test_case_desc_get(self, key);
    NAIVE_ASSERT(value && value->type == NAIVE_TEST_CASE_DESC_VALUE_STR);
    return value->value.str;
}

NaiveResult naive_test_desc_init(NaiveTestDesc *self, NaiveAllocFunc *alloc, void *allocator, NaiveI32 cap)
{
    self->cap = cap;
    self->len = 0;
    self->cases = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveTestCaseDesc), sizeof(NaiveTestCaseDesc) * cap);
    if (self->cases)
        return NAIVE_OK;
    else
        return NAIVE_ERR_NOMEM;
}

NaiveResult naive_test_desc_add_case(NaiveTestDesc *self, NaiveTestCaseDesc **p_case)
{
    if (self->len < self->cap) {
        *p_case = &self->cases[self->len];
        self->len++;
        return NAIVE_OK;
    } else {
        *p_case = NULL;
        return NAIVE_ERR_NOMEM;
    }
}
