# C++ Module 07

*This project has been created as part of the 42 curriculum by jboustan*

## Description

**C++ Module 07** introduces C++ templates — writing a single function or
class body once and letting the compiler generate a version for whatever
type it's used with, instead of duplicating code per type. Three exercises:
a header of generic `swap`/`min`/`max` function templates, an `iter`
function template that applies any callable to every element of an array
regardless of its constness, and an `Array<T>` class template — a
bounds-checked, dynamically-allocated fixed-size array usable with any type.

---

## Core Features

### whatever — swap / min / max (ex00)

- `swap`, `min`, and `max` are function templates, defined entirely inside
  `whatever.hpp` (the only place a non-template function implementation is
  allowed to live in a header)
- `swap(T & a, T & b)` exchanges the values of two references of any type;
  no return value
- `min(T const & a, T const & b)` / `max(T const & a, T const & b)` compare
  with `<` alone (no `<=`/`>=`/`>`, so `T` only needs `operator<`) and return
  the second argument on a tie, per the subject
- Exercised in `main.cpp` with both `int` and `std::string`, proving the same
  three templates work unchanged across unrelated types
- Binary: `whatever`

### iter (ex01)

- `iter(T * array, std::size_t const len, F f)` calls `f` on every element of
  `array`, in order
- Two overloads — one taking `T *`, one taking `T const *` — so overload
  resolution picks the right one for the array's actual constness: the
  non-const overload lets `f` take its argument by non-const reference and
  mutate elements, while passing a `T const *` array can only match the
  const overload, which forces `f` to only read each element
- The `F` template parameter means `f` can be a plain function, an
  instantiated function template, or a functor (function object) — `iter`
  doesn't care, it just calls `f(array[i])`
- `main.cpp` exercises all of these on the same `int` array (an instantiated
  function template that mutates, one that only prints, and a functor), plus
  a `const int` array and a `std::string` array with a plain function
- Binary: `iter`

### Array\<T\> (ex02)

- `Array<T>` is a fixed-size, dynamically-allocated array of `T`, split into
  `Array.hpp` (declarations only) and `Array.tpp` (template definitions,
  `#include`d at the bottom of the `.hpp`) — following the project's
  class-file-naming convention even though templates can't be split into a
  separate `.cpp`
- Full Orthodox Canonical Form: default constructor (`new T[0]`, so
  `_elements` is always a valid `delete[]`-able pointer even when empty), a
  `Array(unsigned int n)` constructor that allocates `new T[n]` then
  explicitly assigns `T()` to every element (`new T[n]` alone already
  default-constructs class-type elements, but leaves built-ins like `int`
  indeterminate — the explicit `T()` assignment covers both), a copy
  constructor and copy assignment operator that both deep-copy
  element-by-element so two `Array`s never share storage, and a destructor
- `operator=` builds the entire replacement array before touching `*this`'s
  own storage and only then `delete[]`s the old one — self-assignment-safe
  by construction, on top of the standard `if (this != &rhs)` guard
- `T & operator[](unsigned int index)` and its `const` overload both throw a
  nested `Array<T>::OutOfBoundsException` (inherits `std::exception`,
  overrides only `what()`, exempt from OCF since it holds no data) when
  `index >= _size` — using `unsigned int` also transparently catches a
  negative index, since it implicitly converts to a huge unsigned value
  before reaching the check
- `size(void) const` returns the element count, per the subject's exact
  signature
- `main.cpp` exercises: an empty default array, `Array<int>(5)` and
  `Array<std::string>(3)` default-initialization, copy-then-mutate and
  assign-then-mutate independence, self-assignment, and three
  out-of-bounds cases (positive index, negative index, index into an empty
  array), all caught via `catch (std::exception &)`
- Binary: `array`

---

## Instructions

### Compilation

    cd ex00 && make   # or ex01, ex02

### Cleaning

    make clean
    make fclean
    make re

---

## Running

    cd ex00 && make && ./whatever
    cd ex01 && make && ./iter
    cd ex02 && make && ./array

---

## Example Behavior

### ex00 — whatever: swap / min / max on int and std::string

    $ ./whatever
    a = 3, b = 2
    min( a, b ) = 2
    max( a, b ) = 3
    c = chaine2, d = chaine1
    min( c, d ) = chaine1
    max( c, d ) = chaine2

### ex01 — iter: mutating and read-only callables over int, const int, and std::string arrays

    $ ./iter
    -- non-const int array --
    before: 1 2 3 4 5
    after increment: 2 3 4 5 6
    after doubling: 4 6 8 10 12

    -- const int array --
    values: 10 20 30

    -- std::string array --
    [chaine1] [chaine2] [chaine3]

### ex02 — array: default/n-ary construction, deep-copy independence, and bounds checking

    $ ./array
    -- default construction (empty array) --
    empty.size() = 0

    -- construction with n, default-initialized --
    ints (size 5): 0 0 0 0 0
    strings (size 3): [] [] []

    -- filling ints, then copy-constructing --
    ints before copy (size 5): 0 10 20 30 40
    copyOfInts (fresh copy) (size 5): 0 10 20 30 40
    -- after mutating one element in each --
    ints (size 5): 0 -1 20 30 40
    copyOfInts (size 5): 999 10 20 30 40

    -- out-of-bounds access (positive index) --
    caught exception: Array: index out of bounds

    -- out-of-bounds access (negative index) --
    caught exception: Array: index out of bounds

    -- out-of-bounds access on an empty array --
    caught exception: Array: index out of bounds

---

## Project Structure

    ex00/
      Makefile
      whatever.hpp
      main.cpp

    ex01/
      Makefile
      iter.hpp
      main.cpp

    ex02/
      Makefile
      Array.hpp
      Array.tpp
      main.cpp

---

## Technical Constraints

- **C++ Standard**: C++98
- **Compilation Flags**:

      -Wall -Wextra -Werror -std=c++98

- **Forbidden**:
  - `printf`, `*alloc`, `free`
  - `using namespace`
  - `friend`
  - STL containers (`vector`, `list`, `map`, …)
  - STL algorithms (`<algorithm>`)
  - C++11 or later features
  - Function implementations in header files, except for function templates
    (which is exactly why `whatever.hpp` and `iter.hpp` can define their
    templates directly in the header)

- **Note**: `Array<T>::OutOfBoundsException` is exempt from Orthodox
  Canonical Form as a data-less exception class overriding only `what()`;
  `Array<T>` itself still needs the full four special member functions,
  since it owns a heap allocation.

---

## Key Concepts Covered

- **Function templates**
  - `swap`, `min`, `max` (ex00) and `iter` (ex01) are each written once and
    instantiated by the compiler for whatever concrete type they're called
    with — `int`, `std::string`, or anything else supporting the operations
    the template body uses
  - A function template's full definition must be visible wherever it's
    instantiated, which is why `whatever.hpp` and `iter.hpp` implement their
    templates directly in the header rather than splitting a declaration
    into a `.cpp`

- **Template overload resolution on const-ness**
  - `iter`'s two overloads (`T *` vs `T const *`) let the compiler pick the
    version that matches whether the array argument is const or not, without
    the caller ever having to specify which one to use

- **Passing a callable as a template parameter**
  - `iter`'s `F` parameter accepts any type usable as `f(array[i])` — a
    plain function, an instantiated function template, or a functor — with
    no special-casing inside `iter` itself

- **Class templates**
  - `Array<T>` is a template class, not just a template function: every
    member function is itself a function template parameterized on the
    class's own `T`, which is why the definitions live in `Array.tpp`
    (included at the bottom of `Array.hpp`) instead of a separate `.cpp` —
    template code must be visible at the point of instantiation, and a
    `.cpp` file compiled on its own can't provide that

- **`new[]` / `delete[]` and default-initialization**
  - `new T[n]` alone already default-constructs class-type elements but
    leaves built-in types indeterminate, so `Array`'s constructor explicitly
    assigns `T()` to every element to give both kinds of `T` a defined
    default value

- **Nested exception classes**
  - Continues the Module 05 pattern: `Array<T>::OutOfBoundsException` is
    declared inside `Array<T>` itself, inherits `std::exception`, and
    overrides only `what()`

---

## Author

- **jboustan** – 42 School Student

---

## License

This project is part of the 42 curriculum.
