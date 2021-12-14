
int main()
{
    Value* v = _main();
    if (v != NULL && v->type == INT)
       return v->int_value;
}
