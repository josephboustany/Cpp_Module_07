# C++ Module 5 — Project Rules & Conventions

---

## Context
- Student at 42 school, working through the C++ module series (Modules 0–4 already completed).
- Working directory (Module 5, exceptions — history below): `/home/joseph/Workspace/42/Core_Projects/Cpp_Modules/Cpp_Module_5/Claude_Code`
- Working directory (Module 06, casts — history below): `/home/joseph/data/Core_Projects/Cpp_Modules/CPP_Module_06/Claude_Code`
- Working directory (Module 07, templates — current): `/sgoinfre/jboustan/Core/Cpp_Modules/Cpp_Module_07/Claude_Code`
- Module 5 topic: repetition and exceptions (`throw`/`try`/`catch`, custom exception classes, `std::exception`).
- Module 06 topic: C++ casts (`static_cast`, `reinterpret_cast`, `dynamic_cast`, `const_cast`).
- Module 07 topic: templates — generic function templates (`swap`/`min`/`max`, `iter`) and a class template (`Array<T>`). This file was carried forward from the Module 5/06 projects as a starting template — its house style/conventions below apply across modules, but the Context and "Completed Exercises" sections are per-module and dated; the Module 07 entries are the current work.
- Peer evaluation is a real constraint — constructor/destructor messages and canonical form are checked by peers.
- Each exercise gets an explanation file (`exNN_explained.md`) inside its directory.

---

## Learned Conventions (confirmed during this module)

- **Protected attribute naming follows the exercise spec exactly.** The underscore prefix rule applies to private members only. If the exercise PDF names a protected attribute `type`, use `type` — not `_type`. Deviating breaks peer evaluation.
- **`virtual` keyword goes in the `.hpp` declaration only — never in the `.cpp` definition.** Repeating it in the definition is a compile error.
- **Polymorphic methods print only their own content, no class-name prefix.** E.g. `makeSound()` outputs `"Woof! Woof!\n"`, not `"Dog: Woof! Woof!\n"`. Prefixing inside the method causes double-labeling when main() also prints `getType()`.
- **Include guards must be ALL_CAPS.** `#ifndef ANIMAL_HPP` — never mixed case like `ANIMAL_hpp`. The `#ifndef` and `#define` tokens must match exactly and be fully uppercase.
- **Constructor/destructor messages must match the class name casing exactly.** E.g., `"Cat destructor called\n"` not `"cat destructor called\n"`. A single wrong-case letter is visible in peer output checks.
- **East const and west const are identical.** `const std::string&` and `std::string const&` mean exactly the same thing. The subject uses east const style (`T const&`) — follow it for consistency but never treat the other as wrong.
- **Suppress unused-parameter warnings with `(void)param;`.** With `-Wextra -Werror`, an unreferenced parameter is a compile error. Write `(void)param;` as the function body when the parameter must exist in the signature but is intentionally unused (e.g. `AMateria::use(ICharacter& target)` default no-op).
- **Passing by `*` signals ownership transfer; passing by `&` signals borrow.** When a function takes a pointer, the caller hands ownership over — the callee (or the object it stores in) is responsible for `delete`. When a function takes a reference, the caller keeps ownership and must `delete` it themselves. This is a convention, not enforced by the compiler — both sides must follow the same understanding to avoid double-free or leak.
- **Custom exception classes are exempt from Orthodox Canonical Form — nothing else is.** The subject's exact wording: "exception classes do not have to be designed in Orthodox Canonical Form. However, every other class must follow it." If the exception class holds no data of its own (nested exception types with only an overridden `what()`), the compiler-generated default constructor/copy/assignment/destructor are sufficient — no need to hand-write them. But every non-exception class — including one with a `const` member like `Bureaucrat::_name` — still needs an explicit default constructor. A `const` member of class type (e.g. `const std::string`) doesn't block a default constructor from compiling; it just gets default-constructed via its own type's default ctor if the init list doesn't set it (proven: `class C { const std::string _s; public: C() {} };` compiles fine, `_s` ends up `""`). Only a `const` member of a *built-in* type (e.g. `const int`) with no initializer is a hard compile error. So "this class has a const member" is never by itself a valid reason to skip the default constructor — write one with hardcoded, known-valid default values instead (no need to re-run validation logic on values you controlled yourself).
- **`catch` by reference, never by value.** `catch (std::exception& e)` preserves virtual dispatch to the derived `what()`; `catch (std::exception e)` copies and *slices* the object down to the base part, silently losing the override.
- **Overriding `std::exception::what()` requires the exact same exception specification.** `virtual const char* what() const throw();` — the trailing `throw()` must be repeated (it's part of the signature C++98 requires to match the base declaration), even though `virtual` itself still follows the project's declaration-only rule.
- **The subject's "Read me" section (Chapter II) is wordplay, not a README requirement.** It's advice telling the student to "read [the subject]," not a mandated `README.md` deliverable — no exercise's "Files to Submit" list ever includes a README. Confirmed 2026-09-09 by rereading the actual subject text. A project-root `README.md` is still worth writing for the student's own reference (see the entry below), just not because the subject asks for one.

---

## Compilation
- Compiler: `c++` with flags `-Wall -Wextra -Werror`
- Standard: C++98 (`-std=c++98` must still compile)

---

## Naming Conventions
- Exercise directories: `ex00`, `ex01`, ..., `exn`
- Class names: UpperCamelCase (e.g., `BrickWall`)
- Files named after their class: `ClassName.hpp`, `ClassName.cpp`, `ClassName.tpp`
- Private member attributes prefixed with `_` (e.g., `_hitPoints`)
- Follow naming exactly as specified in the exercise guidelines

---

## Output
- Every output message must end with a newline character (`\n`)
- All output goes to standard output (`std::cout`)
- Constructors and destructors must always print a message (required by 42 for peer evaluation)

---

## Forbidden
- `printf()`, `*alloc()`, `free()` — use C++ alternatives
- `using namespace <ns_name>`
- `friend` keyword
- STL containers: `vector`, `list`, `map`, etc.
- STL algorithms: anything requiring `<algorithm>`
- C++11 or later features
- Boost or any external library
- Function implementations in header files (except function templates)

---

## Headers
- Each header must be self-contained (include all its own dependencies)
- All headers must have include guards (`#ifndef / #define / #endif`) to prevent double inclusion
- Include `<iostream>` in `.cpp` files only — not in headers — to avoid forcing it on every consumer

---

## Orthodox Canonical Form
Every class must implement these four special functions explicitly:

| Function                  | Triggered by                                       |
|---------------------------|----------------------------------------------------|
| Default constructor       | `ClassName a;`                                     |
| Copy constructor          | `ClassName b(a);` or `ClassName b = a;`            |
| Copy assignment operator  | `b = a;` (both already exist)                      |
| Destructor                | end of scope or `delete`                           |

- Use the **member initializer list** (`: member(value), ...`) to initialize members, not body assignment
- Copy assignment operator must have a **self-assignment guard**: `if (this != &other)`
- Copy assignment operator must return `*this` by reference (`ClassName&`)
- Initialization order follows declaration order in the class — keep the list in the same order

### OCF in derived classes
- Derived copy constructor **must** call the base copy constructor explicitly in its initializer list:
  `Derived::Derived(const Derived& other) : Base(other) { ... }`
  Without this, the base default constructor runs and base members are not copied.
- Derived copy assignment **must** call `Base::operator=(other)` in its body after the self-assignment guard.
- Derived destructor body runs first, then the base destructor runs automatically — no need to call it explicitly.

---

## Inheritance & Polymorphism

### `virtual` on methods
- Mark a method `virtual` in the base class to enable **dynamic dispatch** (runtime lookup via vtable).
- Derived classes override by declaring the same method with the same signature — the `override` keyword does not exist in C++98.
- Without `virtual`, calling through a base pointer always runs the base method regardless of the object's actual type (static dispatch).

### Virtual destructor (mandatory rule)
- **Any base class whose objects may be `delete`d through a base class pointer must have a `virtual` destructor.**
- Without it, only the base destructor fires on `delete`, skipping the derived destructor and leaking any derived resources.
- The `virtual` keyword appears only in the declaration (`.hpp`); never repeat it in the definition (`.cpp`).

### Inheritance access
- Use `public` inheritance for "is-a" relationships: `class Dog : public Animal`.
- `protected` members of the base are accessible inside derived class methods but not from outside.

### WrongAnimal pattern
- To demonstrate the necessity of `virtual`, exercises may require a parallel hierarchy (e.g. `WrongAnimal` / `WrongCat`) with no `virtual` keyword.
- The wrong hierarchy shows static dispatch and the missing derived-destructor problem side by side with the correct hierarchy.

---

## Code Style
- No enforced style, but code must be clean and readable
- Prefer C++-style standard library functions over C equivalents
- Pass objects by `const &` when only reading them; pass by value only for small primitives or intentional copies
- Use `unsigned int` for values that cannot be negative (HP, energy, damage); always guard against underflow before subtracting

---

## Directory Structure (per exercise)
```
exNN/
├── Makefile
├── <binary>          ← output binary at root
├── include/          ← all .hpp headers
├── obj/              ← .o object files (auto-created by make, never committed)
└── src/              ← all .cpp source files
```
- Compile with `-I include` so headers are included as `"ClassName.hpp"` everywhere
- `@mkdir -p obj` placed at the start of each compile rule to create `obj/` on first build
- `clean` removes object files; `fclean` also removes the binary

---

## Makefile Rules (mandatory)
- Rules: `$(NAME)`, `clean`, `fclean`, `re`, `all`
- `all` must be the first (default) target — runs on plain `make`
- No relinking when sources are unchanged
- For multi-binary projects: one rule per binary; `all` compiles all
- All source files must be **explicitly named** — no wildcards (`*.cpp`, `*.o`, etc.)

## Makefile Style
- Section comments: `# ─── SECTION NAME ───────` style, in this order:
  1. OUTPUT BINARY
  2. COMPILER & FLAGS
  3. DIRECTORIES
  4. SOURCE & OBJECT FILES
  5. DEFAULT TARGET
  6. LINK STEP
  7. COMPILE STEP(S)
  8. CLEAN RULES
  9. PHONY
- `OBJS` listed explicitly by name, **not** derived from `SRCS` via `$(SRCS:.cpp=.o)`
- One compile rule per `.cpp` file — no pattern rule `%.o: %.cpp`
- Each compile rule lists its `.cpp` **and** its `.hpp` dependencies so Make recompiles on header changes

### Makefile template
```makefile
# ─── OUTPUT BINARY ────────────────────────────────────────────────────────────
NAME    = <binary>

# ─── COMPILER & FLAGS ─────────────────────────────────────────────────────────
CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I include

# ─── DIRECTORIES ──────────────────────────────────────────────────────────────
SRCDIR  = src
OBJDIR  = obj
INCDIR  = include

# ─── SOURCE & OBJECT FILES ────────────────────────────────────────────────────
SRCS    = src/main.cpp     src/ClassName.cpp
OBJS    = obj/main.o       obj/ClassName.o

# ─── DEFAULT TARGET ───────────────────────────────────────────────────────────
all: $(NAME)

# ─── LINK STEP ────────────────────────────────────────────────────────────────
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

# ─── COMPILE STEP(S) ──────────────────────────────────────────────────────────
obj/main.o: src/main.cpp include/ClassName.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o obj/main.o

obj/ClassName.o: src/ClassName.cpp include/ClassName.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c src/ClassName.cpp -o obj/ClassName.o

# ─── CLEAN RULES ──────────────────────────────────────────────────────────────
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# ─── PHONY ────────────────────────────────────────────────────────────────────
.PHONY: all clean fclean re
```

---

## Scratch Directory
- `Scratch/` lives at the root of the working directory alongside the `exNN/` folders
- Contains one subdirectory per exercise (`Scratch/ex00/`, `Scratch/ex01/`, …) with the **same file structure** as the real exercise
- **Makefile**: fully filled (identical copy of the real exercise Makefile)
- **All other files** (`.hpp`, `.cpp`): created empty — the user fills them in as practice
- When creating a Scratch scaffold, mirror every file from the real exercise directory; never generate implementation content in the source/header files
- Each Scratch exercise also gets a `trace.md` that traces every line of `main()` to its output, annotating which variable each constructor/destructor call belongs to

---

## Explanation Files
- Each exercise gets an `exNN_explained.md` at the root of its directory
- Standard three-part structure:
  1. **What the exercise is asking** — plain-terms description of the objects and how they interact, with analogies and the full main() flow
  2. **How to think through this** — step-by-step design decisions in the order you face them from a blank file (why each choice, not just what it is)
  3. **New concepts** — concepts directly used in the solution, explained with code examples
  4. **File-by-file walkthrough** — every file in dependency-first reading order (interfaces → abstract base → concrete leaves → composites → main)
  5. **Memory ownership summary** — table of who owns each pointer after each operation
- Each exercise also gets an `exNN_files_overview.md` — a single table listing all files and one-line description of what each does, in the same reading order
- Concept clarifications may be appended inline below the relevant section when the user asks
- Do **not** include concepts from future exercises or the broader module — only what is in the current solution

---

## Completed Exercises (Module 5)

- **ex00** — Bureaucrat: `const std::string _name`, `int _grade` (1=highest, 150=lowest), full OCF — including an explicit default constructor (`_name("")`, `_grade(_gradeMax)`, no validation since the values are hardcoded and already known-valid, not caller-supplied) alongside the parameterized constructor, copy constructor, copy assignment (copies `_grade` only, leaves the const `_name` untouched), and destructor. (The original build skipped the default constructor on the mistaken belief that `const _name` made it impossible — corrected 2026-09-01 after re-reading the subject; see the Learned Conventions entry above.) Nested exception classes `Bureaucrat::GradeTooHighException` / `GradeTooLowException`, both public-inheriting `std::exception`, each overriding `virtual const char* what() const throw();` — thrown by the constructor and by `incrementGrade()`/`decrementGrade()` whenever the grade would leave `[1, 150]`. `static const int _gradeMin`/`_gradeMax` (1/150) declared in header, defined in `.cpp`. Free-function `operator<<` prints `"<name>, bureaucrat grade <grade>.\n"`. Binary: `bureaucrat`. Explanation file: `ex00/ex00_explained.md` (new-concepts section covers throw/try/catch mechanics, catch-by-reference vs slicing, `std::exception`/`what()`, the C++98 `throw()` exception specification, nested exception classes, const members forcing no default ctor and an asymmetric `operator=`, `static const int` class constants, `operator<<` as free function, stack unwinding safety during constructor validation) — **note:** its const-members-forcing-no-default-ctor claim is now known wrong per the correction above and should be fixed next time that file is touched. No files-overview or Scratch scaffold yet.
  - §2.2 (catch by reference vs value) was expanded with a side-by-side ASCII vptr
    diagram — reference case shows `e` naming the thrown object directly (vptr
    untouched, `e.what()` reaches the derived override); value case shows the copy's
    own vptr reset to `std::exception`'s vtable after slicing. Keep this diagram style
    (not plain prose) if the section is touched again — that was an explicit user
    correction.
  - §2.7 (`static const int` class members) originally had a full odr-use walkthrough
    (value-use vs odr-use, address-taking, storage) but that framing didn't land for
    the user even after expansion. It was fully replaced with a concrete
    build-then-break example: two tiny files (`add`/`math.cpp` calling into
    `main.cpp`) compiled and linked step by step, then deliberately broken (delete the
    file with the real definition) to show the compile step still succeeds while the
    link step fails with `undefined reference to 'add'` — same pattern repeated for a
    variable (`extern int counter;` vs `int counter = 5;`), then mapped onto
    `static const int _gradeMin;` (the promise, in `.hpp`) vs
    `const int Bureaucrat::_gradeMin = 1;` (the real thing, in `.cpp`). See
    [[concrete-broken-example-teaching-style]] — default to this concrete
    promise/reality build-then-break style for any future linker/compile-mechanics
    explanation in this project, rather than a terminology-first one.

- **ex01** — Form: `const std::string _name`, `bool _isSigned` (starts `false`),
  `const int _gradeToSign`, `const int _gradeToExecute` — all private, not
  protected. Grades follow the same `[1, 150]` convention as `Bureaucrat`
  (1=highest, 150=lowest); `Form` has its own private
  `static const int _gradeMin`/`_gradeMax` rather than reusing `Bureaucrat`'s,
  since those are private to `Bureaucrat`. Full OCF including an explicit
  default constructor (`_name("")`, `_isSigned(false)`,
  `_gradeToSign(_gradeMax)`, `_gradeToExecute(_gradeMax)`). Nested
  `Form::GradeTooHighException`/`GradeTooLowException`, same pattern as
  `Bureaucrat`'s. `beSigned(Bureaucrat const&)` throws `GradeTooLowException`
  if the bureaucrat's grade is numerically greater than (i.e. lower-ranked
  than) `_gradeToSign`, else sets `_isSigned = true`. `operator<<` prints
  `"<name>, form, requires grade <sign> to sign and grade <exec> to execute,
  <signed/not signed>.\n"`. `Bureaucrat` gained `signForm(Form&) const`,
  which wraps `form.beSigned(*this)` in its own `try { ... } catch
  (std::exception& e) { ... }`, printing `"<name> signed <form>\n"` on
  success or `"<name> couldn't sign <form> because <reason>.\n"` on failure.
  Circular `Form`/`Bureaucrat` header dependency resolved with forward
  declarations (`class Bureaucrat;` in `Form.hpp`, `class Form;` in
  `Bureaucrat.hpp`); each `.cpp` includes both headers directly since both
  need the other's full definition to call methods on it. Binary:
  `bureaucrat` (same as ex00, extended). Reviewed 2026-09-06 and confirmed via
  actual compile + run (not just inspection):
  - Makefile initially omitted `Form.cpp`/`Form.o` from `SRCS`/`OBJS` entirely
    (no compile rule for it either) — `main.cpp`/`Bureaucrat.cpp` compiled
    fine but the link step failed with `undefined reference to Form::...`,
    confirmed via `make re`. Fixed by mirroring the existing `Bureaucrat.o`
    rule. All three compile-rule dependency lines also only listed the
    same-named header, missing the *other* header each `.cpp` actually
    `#include`s (`main.cpp`/`Bureaucrat.cpp` both include `Form.hpp` too;
    `Form.cpp` includes `Bureaucrat.hpp` too) — fixed by appending the
    missing header to each of the three rules.
  - Stale comment above `Bureaucrat`'s default ctor still claimed "No default
    constructor" — a leftover from before the ex00 default-ctor correction
    above — removed.
  - `Form.cpp`'s parameterized constructor printed `", form created.\n"` (no
    comma) while the default ctor/copy ctor/`operator=`/destructor all print
    `", form, created.\n"` (with comma) — fixed to match.
  - `Form::GradeTooLowException::what()` returned `" grade too low"` with a
    stray leading space, unlike `GradeTooHighException`'s `"grade too high"`
    — fixed.
  - `Form::operator<<` was missing its trailing `\n` (ended `<< "."` instead
    of `<< ".\n"`) — the subject's own general output rule ("unless specified
    otherwise, every output message must end with a newline") applies here
    exactly as it does to `Bureaucrat::operator<<`, since the subject gives no
    example format for `Form`'s `operator<<` that would opt out of it. Fixed;
    confirmed via a full program run that output no longer bled across lines.
  - Checked against the 42 EvalHub grading checklist 2026-09-06: everything
    passes except `main.cpp`'s test coverage — it never constructs a
    `Bureaucrat` with an out-of-range grade, and never calls
    `incrementGrade()`/`decrementGrade()` at all, even though both are
    implemented correctly. Not yet fixed as of this writing — add both before
    defense.
  - A GDB teaching session (see [[gdb-two-terminal-tty-workflow]]) used
    `catch throw`/`catch catch` + `bt`/`up` on debug builds of the real,
    unmodified sources to show the exception search climbing from
    `Form::beSigned` to `Bureaucrat::signForm`'s `catch` and stopping there
    without ever reaching `main`. Two follow-up experiments were made
    *directly in the real files*, at the user's explicit request, each
    reverted immediately after (confirmed via `git diff --stat` showing zero
    diff against the pre-experiment state): (1) commented out `signForm`'s
    `try`/`catch` — confirmed the program then aborts (`std::terminate`/
    `SIGABRT`) the instant the grade mismatch fires, losing buffered stdout
    and skipping every destructor/test after that point; (2) with that catch
    still removed, added a `try`/`catch` around the call site in `main.cpp`
    instead — confirmed (via `bt`+`up` landing in `main`, not library
    internals) that a catch can sit anywhere up the call chain, not only in
    the immediate caller. No files-overview or Scratch scaffold yet;
    `ex01_explained.md`/`ex01_defense.md` still on hold per
    [[ex01-learn-by-trial-and-error]] until the user finishes the exercise.

- **ex02** — `Form` renamed to `AForm`, now abstract via
  `virtual void executeAction() const = 0;` (protected, called only from
  inside `execute()`). All attributes stay private in the base class, not
  protected. `AForm::execute(Bureaucrat const& executor) const` is the shared
  skeleton: throws `FormNotSignedException` if `!_isSigned`, throws
  `GradeTooLowException` if the executor's grade is numerically greater than
  `_gradeToExecute`, otherwise calls `executeAction()`. `virtual ~AForm()` is
  mandatory since concrete forms are deleted through an `AForm*`. Three
  concrete forms, each taking only a target string and hardcoding its own
  name + grades in `AForm`'s initializer list: `ShrubberyCreationForm`
  (sign 145/exec 137, writes ASCII trees to `<target>_shrubbery`),
  `RobotomyRequestForm` (sign 72/exec 45, `rand() % 2` success/failure after
  printing drilling noises), `PresidentialPardonForm` (sign 25/exec 5, prints
  the Zaphod Beeblebrox pardon message). `Bureaucrat` gained
  `executeForm(AForm const&) const`, same wrap-in-try/catch pattern as
  `signForm()`. Binary: `bureaucrat`. No files-overview, Scratch scaffold, or
  explanation file yet. Note: `ex02_me/` sitting alongside `ex02/` is a
  separate self-attempt, not part of the graded submission — excluded from
  the root `README.md`'s Project Structure.

- **ex03** — `Intern` class: no attributes of its own, but still full OCF
  (default/copy/assign/destructor all print a message even though there's
  nothing to copy). `makeForm(std::string const& formName, std::string const&
  target) const` returns a heap-allocated `AForm*` (ownership transferred to
  the caller) matching `formName`, or `NULL` with an explicit error message
  if no match is found. Avoids an if/else-if chain per the subject's explicit
  requirement by using two parallel `static` arrays —
  `_formNames[3]` (strings) and `_formCreators[3]` (an array of `static`
  factory function pointers, one `create*` per concrete form) — matched by
  index. Binary: `bureaucrat`. No files-overview, Scratch scaffold, or
  explanation file yet.

- **Root `README.md`** — written 2026-09-09 at the request of the user, one
  level up from `ex00`–`ex03` (not per-exercise), covering all four
  exercises: Description, Core Features (one subsection per exercise),
  Instructions, Running, Example Behavior (real output captured from each
  exercise's actual `main.cpp`/`.cpp` files, not invented), Project Structure,
  Technical Constraints, Key Concepts Covered, Author, License — modeled on a
  README the user had previously written for Module 4. Built by actually
  reading every header/`.cpp`/`main.cpp` in `ex00`–`ex03` first so example
  output and exception-message text are accurate, not guessed.
  `ex01_solution/` and `ex02_me/` were deliberately left out of its Project
  Structure section as alternate/scratch attempts, not graded deliverables —
  flagged to the user, not yet confirmed either way.

---

## Completed Exercises (Module 06)

- **ex00** — `ScalarConverter`: subject-mandated non-instantiable utility
  class — default/copy constructor, copy assignment, destructor all
  `private` and left undefined (never odr-used, so no definitions needed);
  the only public member is `static void convert(std::string const&)`,
  matching the subject's "only one static method" requirement exactly. All
  detection/printing helpers (`detectKind`, `isIntLiteral`,
  `isFloatingLiteral`, `isPseudoFloat`/`isPseudoDouble`, `printChar`/
  `printInt`/`printFloat`/`printDouble`) live as free functions in an
  anonymous namespace in `ScalarConverter.cpp`, not as class members, so the
  class itself stays exactly as thin as the subject asks. Pipeline: detect
  the literal's kind from its string form, parse it once into a `double`
  intermediate (`std::strtod`, authorized by name by the subject), then
  `static_cast` that `double` explicitly to `char`/`int`/`float` for the
  other three lines — every cast in the file is a `static_cast`, no C-style
  casts anywhere (grepped to confirm). Binary: `convert`.
  - Reviewed and fixed 2026-09-10 (bugs found by actually compiling and
    running the program against a wide range of inputs, not just reading the
    code):
    1. Float/double printing used the ostream **default** precision (6
       *significant* digits), so any value ≥ 1e6 or needing more digits fell
       back to scientific notation (e.g. `1000000` → `1e+06`); a hand-rolled
       "append `.0` if no dot was found" hack then produced outright
       malformed output like `1e+06.0f` since it didn't know about
       exponents. Fixed by switching both to `std::fixed <<
       std::setprecision(1)`, which forces plain decimal notation with
       exactly one fractional digit — this both eliminates scientific
       notation entirely and matches every worked example in the subject
       (`0.0f`, `42.0f`), so the old padding hack was removed outright.
    2. A decimal literal whose magnitude overflows even a `double` — e.g. a
       400-digit number — makes `strtod` silently saturate to `HUGE_VAL`
       (`+inf`), which was then printed as `float: inff` / `double: inf` as
       if the user had actually typed one of the six recognized
       pseudo-literal spellings (`-inff`, `+inff`, `nanf`, `-inf`, `+inf`,
       `nan`). That's wrong per the subject's general rule ("if a conversion
       ... overflows, display a message ... impossible") — genuine overflow
       and an explicit inf/nan literal are different cases and must print
       differently. Fixed by tracking whether the literal actually matched
       one of the six spellings; anything else that lands on infinity after
       `strtod` now prints `impossible` for both float and double, while a
       real `-inf`/`+inf`/`nan`/`nanf`/`-inff`/`+inff` input still prints
       its value normally.
    3. A finite `double` too large to fit in a `float` (e.g. a 45-digit
       number, within `double` range but past `FLT_MAX`) was narrowed via
       `static_cast<float>` with no range check — technically undefined
       behavior per C++98 (narrowing to a value the destination type can't
       represent), which happened to yield `inf` on this platform and printed
       as `inff`. Fixed by explicitly comparing against
       `std::numeric_limits<float>::max()` before the cast and printing
       `impossible` for the float line when it's exceeded, while `double`
       still prints the real (finite) value on its own line.
  - Added both new overflow scenarios (double-too-big-for-float, and
    double-too-big-for-double) as extra cases in `main.cpp`'s built-in
    no-argument test battery, alongside the existing subject-example and
    boundary cases, so they're demonstrated on every plain `./convert` run.
  - Checked against the actual 42 EvalHub "Preliminary tests" +
    "Exercise 00: Conversion of scalar types" grading checklist on
    2026-09-10: compiles clean with `-Wall -Wextra -Werror -std=c++98`; no
    forbidden functions/keywords (grepped for `printf`/`*alloc`/`free`/
    `using namespace`/`friend` — none found; `std::strtod` is explicitly
    authorized by the subject); no C-style casts anywhere; private
    ctor/copy-ctor/copy-assign/dtor plus a single public static method
    satisfy "private constructor, static method"; every conversion goes
    through explicit `static_cast`, matching "accept implicit casts for
    promotion only"; all three of the subject's worked examples (`0`,
    `nan`, `42.0f`) reproduce byte-for-byte. Passes.
  - No files-overview or Scratch scaffold yet; `ex00_explained.md` and a
    `Code Explanations.odt` already exist in `ex00/` alongside the required
    `Makefile`/`*.cpp`/`*.hpp` — extra reference material, not a grading
    risk since the subject's "Files to Submit" list doesn't forbid extras.
  - Reviewed and cleaned up 2026-09-12 (style, not behavior — rebuilt and
    re-ran after each change to confirm identical output):
    1. Replaced the anonymous namespace wrapping all the free helper
       functions with `static` on each function individually.
       Both give the same internal linkage (invisible outside
       `ScalarConverter.cpp`); `static` was chosen over the anonymous
       namespace because it's one keyword per line with no extra
       indentation/closing brace, and it's the idiom 42 peer-evaluators are
       most likely to recognize instantly without needing it explained
       during defense. The `LiteralKind` enum stays a plain top-level
       declaration since `static` only applies to functions/variables, not
       types — harmless here since nothing outside this file ever uses it.
    2. Added a one-line trailing comment beside each `#include` at the top
       of `ScalarConverter.cpp` naming what it's used for (e.g.
       `#include <cstdlib> // std::strtod`).
    3. In `isIntLiteral`, changed `if (i >= s.size())` to
       `if (i == s.size())` (the "reject a lone `+`/`-` with nothing after
       it" check). Verified equivalent, not just stylistically preferred:
       after the earlier `s.empty()` check, `s.size() >= 1` is guaranteed,
       and `i` is only ever `0` or `1`, so `i` can never exceed `s.size()`
       — `==` and `>=` are provably identical in every reachable case here.
       `==` reads as the more precise statement of the actual invariant
       ("the sign consumed the whole string"). Confirmed via rebuild plus
       a battery of edge-case runs (`+`, `-`, `42`, `-42`, `+42`, `0`,
       `-0`, `INT_MIN`, `INT_MAX`, `INT_MAX+1`, `abc`, `4.2`, `4.2f`) —
       all identical to pre-change output.

- **ex01** — `Serializer`: same subject-mandated non-instantiable pattern as
  `ScalarConverter` (ex00) — default/copy constructor, copy assignment, and
  destructor all `private` and left undefined. Its two public members are
  `static uintptr_t serialize(Data* ptr)` and
  `static Data* deserialize(uintptr_t raw)`, each a one-line
  `reinterpret_cast` (not `static_cast` — there's no numeric relationship
  between a pointer and an integer for `static_cast` to make sense of; it's
  a raw bit reinterpretation, which is exactly what `reinterpret_cast` is
  for). `uintptr_t` comes from `<stdint.h>` (the C header), not the C++11-only
  `<cstdint>`, so it stays valid under `-std=c++98`. `Data` (a required
  separate deliverable per the subject's own wording — "turn in the files of
  your Data structure") is a plain non-empty struct with three unrelated
  field types (`int id`, `std::string label`, `double weight`), kept in its
  own `Data.hpp` rather than folded into `Serializer.hpp`: `Serializer`'s
  methods don't know or care what `Data` contains, so the two stay
  decoupled, matching the "files named after their class" convention.
  `main.cpp` builds a `Data`, serializes its address, deserializes the
  result, checks `restoredPtr == originalPtr`, then reads `id`/`label`/
  `weight` back through the restored pointer to prove the whole object (not
  just the address bits) survived the round trip. Binary: `serialize`.
  Verified via actual `make re` + run: compiles clean under
  `-Wall -Wextra -Werror -std=c++98`, no forbidden functions/casts (grepped —
  only `reinterpret_cast` appears), round-trip prints
  `SUCCESS: restored pointer compares equal to the original.` with all three
  fields intact. `Serializer.hpp`'s IDE linter warning ("function definition
  for `operator=`/`~Serializer` not found") is expected noise, not a bug —
  those four members are deliberately declared-and-undefined; `make` already
  confirms the link succeeds. Explanation file: `ex01/ex01_explained.md`. No
  files-overview or Scratch scaffold yet.

- **ex02** — `Base`: public virtual destructor only, per the subject. Three
  genuinely empty classes `A`/`B`/`C` publicly inherit `Base` (no members, no
  methods, no explicitly-written special member functions — all four classes
  are exempt from OCF per the subject's own wording). Free functions (not
  class members) `Base* generate(void)`, `void identify(Base* p)`, and
  `void identify(Base& p)` live in `Identify.hpp`/`Identify.cpp`, kept
  separate from `Base`/`A`/`B`/`C` since they're operations *on* the
  hierarchy, not part of it. `generate()` seeds nothing itself — `main`
  calls `std::srand(std::time(NULL))` once — and returns `new A()`/`new B()`/
  `new C()` (chosen via `std::rand() % 3`) as a `Base*`, ownership
  transferred to the caller per the project's pointer-ownership convention.
  `identify(Base*)` chains `dynamic_cast<A*>`/`<B*>`/`<C*>` in an
  `if`/`else if`, printing the first non-NULL result. `identify(Base&)` —
  "using a pointer inside this function is forbidden," and a failed
  `dynamic_cast` to a reference type throws `std::bad_cast` rather than
  returning NULL — so each of the three `dynamic_cast<A&>`/`<B&>`/`<C&>`
  attempts is wrapped in its own `try { ... } catch (...) { }`: `catch (...)`
  matches without ever naming `std::bad_cast`, which lives in the forbidden
  `<typeinfo>` header, sidestepping the need to include it at all (`typeinfo`
  itself is never included anywhere — confirmed via grep; `dynamic_cast` is
  a core-language feature that needs no `#include` as long as
  `std::type_info`/`std::bad_cast` are never named directly).
  `main.cpp` loops 10 rounds: `generate()`, run both `identify()` overloads
  on the same object (their answers always agree, confirmed across multiple
  runs), then `delete p`. Binary: `identify`. Verified via actual `make re` +
  repeated runs: compiles clean under `-Wall -Wextra -Werror -std=c++98`; two
  back-to-back runs produced identical A/B/C sequences at first, which was
  double-checked (not assumed a bug) by adding a `sleep 1` between runs and
  confirming the sequence then differs — expected behavior of
  `srand(time(NULL))`'s 1-second resolution, not a flaw in `generate()`.
  Explanation file: `ex02/ex02_explained.md`. No files-overview or Scratch
  scaffold yet.

- **Full peer-evaluation dry run (ex00–ex02), 2026-09-13** — walked all three
  exercises against the actual 42 evaluation-sheet checklist (prerequisites +
  per-exercise questions), not just re-reading the code:
  - Prerequisites: all three Makefiles use `c++` with
    `-Wall -Wextra -Werror -std=c++98`; confirmed via fresh `make re` on each
    (zero warnings). No function bodies in any header (checked every `.hpp`
    — only class declarations/braces, no implementations). Grepped for
    forbidden functions (`*alloc`/`printf`/`free`), `using namespace`,
    `friend`, STL containers/`<algorithm>` — none found anywhere.
  - **Memory leaks — actually run under valgrind, not assumed**:
    `valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1`
    on `convert` (multiple inputs incl. the no-arg overflow battery),
    `serialize`, and `identify` (10 rounds of `new`/`delete` per run) — zero
    errors, zero leaks on all three.
  - ex00: private-ctor-plus-static-method shape confirmed; every cast in
    `ScalarConverter.cpp` is `static_cast` (grepped); the three subject
    worked examples (`0`, `nan`, `42.0f`) reproduce exactly.
  - ex01: `reinterpret_cast` used exactly twice, correct direction each way
    (`Data*`→`uintptr_t`, `uintptr_t`→`Data*`); private-ctor-plus-static-method
    shape confirmed; round-tripped struct's fields (`id`/`label`/`weight`)
    read back correctly after `deserialize()`.
  - ex02: confirmed `<typeinfo>` is never actually `#include`d anywhere
    (grepped — the string only appears inside explanatory comments);
    `identify(Base&)` uses `try`/`catch (...)` as required. One cosmetic
    note (not a failure): `identify(Base*)` checks NULL via the idiomatic
    `if (dynamic_cast<A*>(p))` rather than a literal
    `!= NULL` comparison — behaviorally identical, flagged only in case a
    stricter evaluator wants to *see* the literal comparison.
  - Procedural/repo-integrity checks: remote is
    `git@github.com:josephboustany/CPP_Module_06.git`, all commits authored
    by Joseph Boustany, `make`/`c++`/`valgrind` confirmed to resolve to real
    system binaries (no shell aliasing) — satisfies the evaluation
    guideline's repo-ownership and no-malicious-alias checks.
  - **Process note**: running `make fclean` in every exercise during this
    check deleted `ex00/convert`, which — unlike `ex01`'s/`ex02`'s binaries —
    was a *tracked* file (not covered by `.gitignore`, which only lists
    `a.out`/`*.o`/etc., not exercise binaries by name). Restored via
    `git checkout -- ex00/convert` immediately after noticing. Worth
    remembering next time a clean/rebuild pass touches `ex00`: its compiled
    binary is committed, so `fclean`+forgetting to rebuild-and-re-add (or
    `git checkout`) would leave it looking deleted in `git status`.

---

## Completed Exercises (Module 07)

- **ex00** — `whatever.hpp`: `swap`/`min`/`max` as function templates,
  defined entirely in the header (the one case where a header may contain a
  function implementation). `swap(T & a, T & b)` exchanges two references
  and returns nothing; `min`/`max` compare with `<` alone (no `<=`/`>=`/`>`,
  so `T` only needs `operator<`) and return the second argument on a tie,
  per the subject. `main.cpp` exercises all three on both `int` and
  `std::string`. Binary: `whatever`. Pre-existing before this project's
  Claude Code sessions began; reviewed 2026-09-14 against the subject and
  confirmed fully compliant — compiles clean under
  `-Wall -Wextra -Werror -std=c++98`, output matches the subject's worked
  example byte-for-byte. Only nit: the compiled binary `whatever` and
  `main.o` are tracked in git with no `.gitignore` — flagged, not yet fixed
  either way. No files-overview, Scratch scaffold, or explanation file yet.

- **ex01** — `iter.hpp`: `iter(T * array, std::size_t const len, F f)` calls
  `f` on every element of `array`, in order. Two overloads (`T *` and
  `T const *`) so overload resolution picks the right one for the array's
  actual constness — the non-const overload lets `f` take its argument by
  non-const reference and mutate elements; the const overload is the only
  one that can match a `const`-qualified array, and only allows read access.
  Length parameter is `std::size_t const len`. `F` accepts a plain function,
  an instantiated function template, or a functor — `iter` doesn't care, it
  just calls `f(array[i])`. `main.cpp` exercises it with an instantiated
  function template by non-const ref (mutates), one by const ref, a plain
  non-template function, a functor, a `const int` array, and a
  `std::string` array. Binary: `iter`. Built from scratch 2026-09-14;
  verified via `make re` + run: compiles clean, output correct. The user has
  since hand-edited `ex01/main.cpp` themselves (added a comment about `::`
  naming the global namespace) — that edit is the current state on disk, not
  something to revert. No files-overview, Scratch scaffold, or explanation
  file yet.

- **ex02** — `Array<T>`: files split `Array.hpp` (declarations only) /
  `Array.tpp` (template definitions, `#include`d at the bottom of the
  `.hpp`), matching this project's class-file-naming convention even though
  templates can't be split into a separate `.cpp`. Default constructor
  allocates `new T[0]`; the `unsigned int n` constructor allocates
  `new T[n]` then assigns `T()` to every element explicitly — `new T[n]`
  alone already default-constructs class-type elements but leaves built-in
  types (int, char, ...) with indeterminate values, so the explicit `T()`
  assignment is what actually satisfies "initialized by default" for both
  kinds of `T` (this is the point of the subject's `new int()` tip). Copy
  constructor and `operator=` both deep-copy element-by-element so mutating
  one `Array` never affects the other. `operator=` builds the entire
  replacement array first (`new T[rhs._size]` + copy loop) and only
  `delete[]`s the old `_elements` afterward, so it's self-assignment-safe by
  construction on top of the standard `if (this != &rhs)` guard — the guard
  protects against a naive "delete old first, then copy from rhs" ordering,
  which would leave `rhs._elements` dangling before the copy loop reads it
  during self-assignment. `operator[]` (both non-const and const overloads)
  takes `unsigned int index` and throws a nested
  `Array<T>::OutOfBoundsException` (inherits `std::exception`, only
  overrides `what()`, exempt from OCF as a data-less exception class) when
  `index >= _size` — this also transparently catches negative indices, since
  a negative `int` literal implicitly converts to a huge `unsigned int` at
  the call site. `size()` is `const`, takes no params. Binary: `array`.
  `main.cpp` exercises: empty default array, `Array<int>(5)` and
  `Array<std::string>(3)` default-init, copy-then-mutate independence,
  assign-then-mutate independence, self-assignment (via a pointer
  indirection — `assigned = assigned;` directly fails to compile under
  `-Werror` because clang's `-Wself-assign-overloaded` treats it as an
  error), and three out-of-bounds cases (positive index, negative index,
  index into an empty array) all caught via `catch (std::exception &)`.
  Built from scratch 2026-09-15; verified via actual `make re` + run:
  compiles clean under `-Wall -Wextra -Werror -std=c++98`;
  `valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1` — 16
  allocs, 16 frees, 0 leaks, 0 errors. No `.gitignore` in this repo (matches
  ex00's precedent above) so `ex02/array` and `ex02/main.o` are left as
  on-disk build output, uncommitted until the user stages them. A one-line
  explanatory comment was added above every function in `Array.tpp` (each
  ctor, `operator=`, dtor, both `operator[]` overloads, `size()`,
  `OutOfBoundsException::what()`) and above `printIntArray` in `main.cpp`,
  at the user's request; rebuilt after each edit to confirm still clean. No
  files-overview, Scratch scaffold, or explanation file yet.
  - `main.cpp` was replaced 2026-09-15 with a stress-test main: fills a
    750-element `Array<int>` and a parallel raw `int*` mirror with the same
    random values, copy-constructs it twice in a nested scope
    (`tmp`/`test`) to prove copying never mutates the original, diffs every
    element against the mirror afterward, triggers both
    `OutOfBoundsException` cases (`numbers[-2]`, `numbers[MAX_VAL]`) inside
    `try`/`catch`, then refills and frees the mirror. First tried outside
    the exercise folders (root-level `main.cpp`, since deleted) with
    `#include <Array.hpp>` and no `<cstdlib>`/`<ctime>` — compiled there
    only via manual `-I`/`-include` workarounds. Once moved into `ex02/`
    and built with the exercise's real Makefile (no `-I` flag), it failed
    with 4 errors: angled-bracket `<Array.hpp>` not found (needs quotes
    since there's no include path) and `srand`/`rand`/`time` undeclared.
    Fixed by changing to `#include "Array.hpp"` and adding
    `#include <cstdlib>` + `#include <ctime>`. Verified via `make re`: builds
    clean under `-Wall -Wextra -Werror -std=c++98`, runs with exit 0 and
    both expected `Array: index out of bounds` lines; valgrind — 7 allocs,
    7 frees, 0 leaks, 0 errors.

- **Root `README.md`** — written 2026-09-15 at the request of the user, one
  level up from `ex00`–`ex02` (not per-exercise), modeled directly on the
  Module 06 root README's structure: Description, Core Features (one
  subsection per exercise), Instructions, Running, Example Behavior,
  Project Structure, Technical Constraints, Key Concepts Covered, Author,
  License. Built by actually reading every header/`.cpp`/`main.cpp` in
  `ex00`–`ex02` first, then compiling and running all three binaries
  (`whatever`, `iter`, `array`) to capture real output for the Example
  Behavior section rather than inventing it.
