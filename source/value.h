#ifndef VALUE_H
#define VALUE_H

typedef enum
{ NONE = 0, INT, FLOAT, STRING, ARRAY }
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
            double float_value;
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

void fail(const char* msg);
void assert(int condition, const char* msg);

Value* none_value();
Value* int_value(int value);
Value* float_value(double value);
Value* string_value(char* value);
Value* array_value(Value** values);

#endif