#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "value.h"
#include "utils.h"

Value* _null() { return none_value(); }
Value* _false() { return int_value(0); }
Value* _true() { return int_value(1); }

#define BINARY_MATH_OPERATION(name, intint, intfloat, floatint, floatfloat) \
Value* name(Value* a, Value* b) \
{ \
    switch (a->type) { \
        case INT: \
            switch (b->type) { \
                case INT:   return intint; \
                case FLOAT: return intfloat; \
                default: fail("type mismatch"); \
            } \
        case FLOAT: \
            switch (b->type) { \
                case INT:   return floatint; \
                case FLOAT: return floatfloat; \
                default: fail("type mismatch"); \
            } \
        default: fail("type mismatch"); \
    } \
    return none_value(); \
}

BINARY_MATH_OPERATION(
    _add,
    int_value(a->int_value + b->int_value),
    float_value((double) a->int_value + b->float_value),
    float_value(a->float_value + (double) b->int_value),
    float_value(a->float_value + b->float_value)
);

BINARY_MATH_OPERATION(
    _sub,
    int_value(a->int_value + b->int_value),
    float_value((double) a->int_value + b->float_value),
    float_value(a->float_value + (double) b->int_value),
    float_value(a->float_value + b->float_value)
);

BINARY_MATH_OPERATION(
    _mul,
    int_value(a->int_value * b->int_value),
    float_value((double) a->int_value * b->float_value),
    float_value(a->float_value * (double) b->int_value),
    float_value(a->float_value * b->float_value)
);

BINARY_MATH_OPERATION(
    _div,
    int_value(a->int_value / b->int_value),
    float_value((double) a->int_value / b->float_value),
    float_value(a->float_value / (double) b->int_value),
    float_value(a->float_value / b->float_value)
);

BINARY_MATH_OPERATION(
    _mod,
    int_value(a->int_value % b->int_value),
    none_value(); fail("type mismatch"),
    none_value(); fail("type mismatch"),
    none_value(); fail("type mismatch")
);

BINARY_MATH_OPERATION(
    _pow,
    int_value((int) pow((double) a->int_value, (double) b->int_value)),
    float_value((double) a->int_value + b->float_value),
    float_value(a->float_value + (double) b->int_value),
    float_value(a->float_value + b->float_value)
);

Value* _sqrt(Value* v) {
    switch (v->type) {
        case INT:
            return float_value(sqrt((double) v->int_value));
        case FLOAT:
            return float_value(sqrt((double) v->float_value));
        default:
            fail("type mismatch");
    }
    return none_value();
}

char* int_to_string(int value) {
    char buffer[64];
    int len = snprintf(buffer, 64, "%d", value);
    char* res = (char*) malloc(sizeof(char) * len);
    strncpy(res, buffer, len);
    return res;
}

char* double_to_string(double value) {
    char buffer[64];
    int len = snprintf(buffer, 64, "%g", value);
    char* res = (char*) malloc(sizeof(char) * len);
    strncpy(res, buffer, len);
    return res;
}

Value* _string(Value* v) {
    switch (v->type) {
        case INT:
            return string_value(int_to_string(v->int_value));
        case FLOAT:
            return string_value(double_to_string(v->float_value));
        default:
            fail("type mismatch");
    }
    return none_value();
}

Value* _at(Value* a, Value* i)
{
    assert(i->type == INT, "type mismatch");
    switch (a->type) {
        case ARRAY:
            assert(i->int_value < a->array_length, "array overflow");
            return a->array_value[i->int_value];
        case STRING:
            assert(i->int_value < a->string_length, "array overflow");
            return string_value(&(a->string_value[i->int_value]));
        default:
            fail("type mismatch");
    }
    return none_value();
}

Value* _length(Value* a)
{
    switch (a->type) {
        case ARRAY:
            return int_value(a->array_length);
        case STRING:
            return int_value(a->string_length);
        default:
            fail("type mismatch");
    }
    return none_value();
}

int string_array_char_amount(Value* a)
{
    assert(a->type == ARRAY, "type mismatch");
    int amount = 0;
    for (int i = 0; i < a->array_length; i++)
    {
        Value* s = a->array_value[i];
        assert(s->type == STRING, "type mismatch");
        amount += s->string_length;
    }
    return amount;
}

Value* _join(Value* a, Value* s)
{
    assert(a->type == ARRAY, "type mismatch");
    assert(s->type == STRING, "type mismatch");

    int size = string_array_char_amount(a) + (s->string_length * a->array_length - 1);
    char* buffer = (char*) malloc(sizeof(char) * size);
    buffer[0] = '\0';

    int first = 1;

    for (int i = 0; i < a->array_length; i++) 
    {
        if (!first) strcat(buffer, s->string_value);
        first = 0;
        assert(a->array_value[i]->type == STRING, "type mismatch");
        strcat(buffer, a->array_value[i]->string_value);
    }

    Value* res = string_value(buffer);
    free(buffer);
    return res;
}

Value* _split(Value* source, Value* seperator)
{
    assert(source->type == STRING, "type mismatch");
    assert(seperator->type == STRING, "type mismatch");

    int occurences = 0;
    for (int i = 0; i < source->string_length; i++)
        if (strcmp(&source->string_value[i], seperator->string_value))
            occurences++;

    Value** substrings = (Value**) malloc(sizeof(char*) * (occurences + 1));
    int substrings_si = 0;

    char* token;
    token = strtok(source->string_value, seperator->string_value);

    while (token != NULL)
    {
        substrings[substrings_si++] = string_value(token);
        token = strtok(NULL, seperator->string_value);
    }
    substrings[substrings_si] = NULL;

    return array_value(substrings);
}

Value* _map(Value* source, Value* (*func)())
{
    assert(source->type == ARRAY, "type mismatch");
    Value* result = (Value*) malloc(sizeof(Value) * source->array_length);
    for (int i = 0; i < source->array_length; i++)
        result[i] = *func(source->array_value[i]);
    return result;
}

int evaluateToBoolean(Value* v)
{
    switch (v->type) {
        case INT:
            return v->int_value != 0;
        case FLOAT:
            return ((int) v->float_value) != 0;
        case STRING:
            return v->string_length != 0;
        case ARRAY:
            return v->array_length != 0;
        case NONE:
        default:
            return 0;
    }
}

Value* _if(Value* condition, Value* (*thenfunc)(), Value* (*elsefunc)())
{
    if (evaluateToBoolean(condition)) 
        return (*thenfunc)();
    else if (elsefunc != NULL)
        return (*elsefunc)();
    else
        return none_value();
}


Value* _print(Value* v)
{
    switch (v->type) {
        case NONE:
            printf("NONE");
            break;
        case INT:
            printf("%d", v->int_value);
            break;
        case FLOAT:
            printf("%g", v->float_value);
            break;
        case STRING:
            printf("%s", v->string_value);
            break;
        default:
            fail("tried to print non-printable value");
    }
    return none_value();
}

Value* _input(Value* prompt)
{
    assert(prompt->type == STRING, "type mismatch");
    printf("%s", prompt->string_value);
    char input[512];
    fgets(input, 512, stdin);
    return string_value(input);
}
