# simple-language-compiling-to-c

A simple proof of concept of compiling a very samll language into C code.

## Why compile to C

By compiling to C, we can then use GCC or Clang to compile it, and heavily optimise it. 

### Pros

- Low effort needed
- Fast output
- Able to write runtime in C

### Cons

- Depends on GCC or Clang
- Cumbersome compilation

## The language

The language is designed to be aa simple as possible.
It contains functions, integers, strings, arrays and thats basically it.
It being to simple, sped up development time by alot, but also greatly limited the usability.

You can see some examples of the language in the `examples` folder.

### Syntax highlighting

I have made a VS Code plugin for syntax highlighting (mostly just keyword highlighting). Just copy the `poc-language-syntax` folder into the VS Code extension folder. On linux that would be `/home/simon/.vscode/extensions`, idk on Wondows or MacOS.

### Purity

The language is designed to be so simple, that all solutions are pure without sideeffects.
This is done quite simply by not haveing any features providing state.
There are only immutable function parameters and return values.


### Inline functions

Because the implementation used direct function pointers (snippet 1), instead of struct/function pairs (snippet 2), inline functions are not supported.
Although more verbose, the later allows for things like inline functions like (snippet 3).
This probably doesn't make sense, but this is important to know, if you're makinng high-level language compiling to low-level. Amd with a bit of polymorphism, this can become very powerful.

```c
// snippet 1, C
Value* my_parseable(Value* a, Value* b)
{
    return add(a, b);
}

int my_user(Value* (*parseable)())
{
    int a = int_value(5);
    int b = int_value(4);
    return parseable(a, b);
}

void caller()
{
    my_user(my_parseable);
}
```

```c
// snippet 2, C
struct Parseable {
    Value* (*execute)();
};

struct MyParseable {
    Value* (*execute)();
    Value* a;
};

Value* MyParseable_execute(struct MyParseable* self, Value* b)
{
    return add(self->a, b);
}

int my_user(struct Parseable* parseable)
{
    int b = int_value(4);
    return parseable(b);
}

void caller()
{
    struct MyParseable* my_parseable = {
        .execute = MyParseable_execute,
        .a = int_value(5),
    };
    my_user((Parseable*) my_parseable);
}
```

```
// snippet 3, <simple-langeuage-...>
func my_user(parseable)
  return parseable(6)
end

func caller(a)
  my_user((b) => add(a, b))
end
```


## Instruktions

Have Yarn and GCC installed.

1. Install dependencies: `yarn`
2. Specify file in `Makefile`
3. Build: `make`
4. Run: `./program`
