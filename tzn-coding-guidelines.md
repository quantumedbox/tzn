# Respecting standard C
- Follow C 89/90 guidelines as strictly as possible
- Hail the SEI CERT C Coding Standard
- Prefix every source with "t" and suffix with ".h" or ".c"
- Always check nullity of pointers and function pointers by `if (ptr) { /* Your stuff */}`
- Last enum value should not be followed by comma, C89 doesn't prohibit so

# Coping with non-standard C
- All source / header files should have names with 8 symbols at most, which leaves 3 symbols after `t` prefix and file extension. Even though 89/90 guidelines specify that identifiers could be at least 31 characters in isn't the fact on pre 89/90 compilers. Because of that all identifier should have 8 characters at most which leaves 5 to 6 significant chars after `t` or `T_` prefix

# Respecting styling
  - Function definitions should look like:
    ```c
    static
    void
    tMyFunction(T_U8* mem)
    {
      T_ASSERT(mem, "NULL encountered");
    }
    ```
  - Declaration should look like:
    ```c
    T_BOOL void tMyFunc(const char* always_say_what_parameter_is);
    ```
  - Following `_` in macro/function/variable names is reserved, `_H` is used for header guards
  - Each indentation level is signaled by 2 spaces
  - Macros identifiers in style of `T_MACRO`
  - *TODO* Standardize type declarations
  - Functions identifiers in style of `tMyFunc`
  - External variables in style of `tMyVar`
  - Enum value names should be prefixed with `tX` where X is specific prefix picked for all values of current definition
  - Internal, auto variables, and function parameters in style of `myvar`, without prefixing
  - As device implementations are exception to conventional way of structuring C projects, global variables, functions and statics should be prefixed with device to which they belong, for example with `tTrmSz` 

# Respecting compilers
  - CC65 treats integer constants bigger than `0x7FFF` as longs unless you specify they as `U`, so always use `0xFFFFU` instead of `0xFFFF`. Result of literal operation is treated as longs too, so, better make every value maked as usigned
  - CC65 is not always smart enough to see that cases with `i++` don't actually use intermediate value, so use `++i` as rvalue instead of `i++` unless you need it
  - CC65 doesn't recognize `inline`, you need to manually inline functions if you need it, possibly with macro functions
  - CC65 doesn't do intermediates well because of integer promotions. It's better to make intermediates for `if` and array accessing by `[]`, so that compiler would have better understanding when it should use `unsigned char` instead of `int`
  - We can't rely on LTO as legacy compilers do not provide this, so, critical code should be included in other sources as much as possible
  - I saw instance where `(void)literal` was producing binary even tho it wasn't of any use, better check for unused memory with such cases

# Respecting hardware
  - Stack is precious, especially on 6502 based architectures, try to minimize its usage, and reuse variables if you can, any recursion should be eliminated. Parameters should passed by globals, and not the stack
  - Multiplication and division isn't always present in instruction sets and thus has to be emulated in nested loops, which is extremely slow. It's possible to make lookup tables for commonly used operations, for example for multiplying powers of 2

# Optimizing
  - C always defines enum with underlying type of `int`, and only some few compilers give control over size of fields. Because of that it's better to define enums with macros
