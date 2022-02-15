# simple-language-compiling-to-c

A simple proof of concept of compiling a very small language into C code.

## Why compile to C

By compiling to C, we can then use GCC or Clang to compile it, which heavily optimises it.

### Pros

- Low effort needed
- Fast output
- Able to write runtime in C

### Cons

- Depends on GCC or Clang
- Cumbersome compilation

## The language

The language is designed to be as simple as possible.
It contains functions, integers, strings, arrays and that's basically it.
It being to simple, sped up development time by alot, but also greatly limited the usability.

You can see some examples of the language in the `examples` folder.

### Syntax highlighting

I have made a VS Code plugin for syntax highlighting (mostly just keyword highlighting). Just copy the `poc-language-syntax` folder into the VS Code extension folder. On Linux and Windows it is `/home/simon/.vscode/extensions` and `C:\Users\simon\.vscode\extensions` respectively, idk on MacOS, but it is likely similar to Linux.

### Purity

The language is designed to be so simple, that all solutions are pure without sideeffects.
This is accomplished simply by not having any features providing state.
There are only immutable function parameters and return values.

### Inline functions

Because the implementation used direct function pointers (snippet 1), instead of struct/function pairs (snippet 2), inline functions are not supported.
Although more verbose, the latter allows for things like inline functions like (snippet 3).
This probably doesn't make sense, but this is important to know, if you're making high-level language compiling to low-level. And with a bit of polymorphism, this can become very powerful.

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
struct Parseable { // "interface" but C-style inherited instead of polymorphism
    Value* (*execute)();
};

struct MyParseable { // "implements Parsable"
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
    return parseable->execute(b);
}

void caller()
{
    struct MyParseable* my_parseable = {
        .execute = MyParseable_execute, // taking the function pointer
        .a = int_value(5),
    };
    my_user((Parseable*) my_parseable); // casted to make C act like it supports inheritance
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

## Instructions

Have Yarn and GCC installed.

1. Install dependencies: `yarn`
2. Specify file in `Makefile`
3. Build: `make`
4. Run: `./program`
