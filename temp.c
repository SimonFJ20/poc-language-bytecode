#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fail(const char* msg)
{
    printf("RuntimeError: %s\n", msg);
    exit(1);
}

void check(int condition, const char* msg)
{
    if (!condition)
        fail(msg);
}

typedef enum
{ NONE = 0, INT, STRING, ARRAY }
ValueType;

typedef struct ValueStruct Value;

typedef struct ValueStruct
{
    ValueType type;
    union {
        struct {
            int int_value;
        };
        struct {
            char* string_value;
            int string_length;
        };
        struct {
            Value** array_value;
            int array_length;
        };
    };
}
Value;

Value* none_value()
{
    Value* v = (Value*) malloc(sizeof(Value));
    v->type = NONE;
    v->int_value = 0;
    return v;
}

Value* int_value(int value)
{
    Value* v = (Value*) malloc(sizeof(Value));
    v->type = INT;
    v->int_value = value;
    return v;
}

Value* string_value(char* value)
{
    Value* v = (Value*) malloc(sizeof(Value));
    v->type = STRING;
    v->string_length = strlen(value);
    size_t string_size = sizeof(char) * v->string_length;
    v->string_value = (char*) malloc(string_size);
    strncpy(v->string_value, value, string_size);
    return v;
}

Value* array_value(Value** values)
{
    Value* v = (Value*) malloc(sizeof(Value));
    v->type = ARRAY;
    for (
            v->array_length = 0;
            values[v->array_length] != NULL;
            v->array_length++
    ) {};
    v->array_value = (Value**) malloc(sizeof(Value) * v->array_length);
    for (int i = 0; i < v->array_length; i++)
        v->array_value[i] = values[i];
    v->array_value[v->array_length + 1] = NULL;
    return v;
}


Value* _add(Value* a, Value* b)
{
    check(a->type == INT, "type mismatch");
    check(b->type == INT, "type mismatch");
    return int_value(a->int_value + b->int_value);
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
        case STRING:
            printf("%s", v->string_value);
            break;
        default:
            fail("tried to print non-printable value");
    }
    return none_value();
}

Value* _at(Value* a, Value* i)
{
    check(a->type == ARRAY, "type mismatch");
    check(i->type == INT, "type mismatch");
    check(i->int_value < a->array_length, "array overflow");
    return a->array_value[i->int_value];
}

//Value* _if(Value* condition, int (*func)())
//{
 //   if (condition) (*func)()
//}

Value* _main()
{
    _print(_at(array_value((Value* []) {int_value(1), int_value(2), int_value(3), NULL}), int_value(1)));
    _print(string_value("   "));
    return NULL;
}

int main()
{
    Value* v = _main();
    if (v != NULL && v->type == INT)
       return v->int_value;
}


