
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
