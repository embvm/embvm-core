# Developer Guide

**Table of Contents**

1. [Sub-documents](#sub-documents)
2. [Adding a New Submodule](#adding-a-new-submodule)
3. [Logging Guidelines](#logging-guidelines)
4. [Meson Guidelines](#meson-guidelines)
    1. [Anti-patterns to Avoid](#anti-patterns-to-avoid)
5. [Running Sanitizers](#running-sanitizers)
5. [Naming Guidelines](#naming-guidelines)
6. [Use Clear Expression](#use-clear-expressions)
7. [Use Symmetry](#use-symmetry)
8. [Use Suitable Comments](#use-suitable-comments)
9. [Postpone Optimization](#postpone-optimization)
10. [Use Atomic Initialization](#use-atomic-initialization)
11. [Prefer Immutable Instances](#prefer-immutable-instances)
12. [Manage Program Inputs](#manage-program-inputs)
13. [Avoid using primitive types for physical quantities](#avoid-using-primitive-types-for-physical-quantities)
14. [Compare floating-point values correctly](#compare-floating-point-values-correctly)
15. [Reduce Conditional Logic](#reduce-conditional-logic)
10. [References](#references)

## Sort

1. Use RAII wherever resource management is performed in the program.
2. Use destructors only for releasing resources.
3. Do not start cancellation cascade if releasing resources fails.
4. Do not stop cancellation cascade, unless you are sure that subsequent operations do not depend on the canceled ones.
5. Make sure that any mutating operation when fails leaves the objects in the state where they can be safely destroyed and reset.
6. Apply the above advice regardless of what technique you use for handling errors.

## Sub-documents

Our developer guidelines have been split up into sub-categories:

* [C++ Guidelines](cpp_guidelines.md)
* [Documentation Guidelines](documentation.md)
* [Framework Error Model](error_model.md)
* [Namespace Registry](namespaces.xlsx)
* [Refactoring Guidelines](refactoring.md)

For more information on using the framework constructs, see [Using the Framework](UsingTheFramework/using_the_framework.md).

## Adding a New Submodule

When adding a new submodule to the project, the following steps will need to be performed:

1. Update [clang-format-all.sh](../../tools/clang-format-all.sh) to exclude the submodule directory
2. Update [clang-format-patch.sh](../../tools/clang-format-patch.sh) to exclude the submodule directory
3. Update [complexity_check.sh](../../tools/complexity_check.sh) to exclude the submodule directory
1. Update [cppcheck.sh](../../tools/cppcheck.sh) to exclude the submodule directory
1. Update [cppcheck_xml.sh](../../tools/cppcheck_xml.sh) to exclude the submodule directory

## Adding a New Static Library Build Target

When creating static library or executable targets, you need to specify `libc_header_include` as an `include_directories` entry and `libcxx_header_include_dep` as a `dependencies` entry:

```
nrf52840 = static_library('nrf52840',
    [
        'nrf52840.cpp',
        '../mdk/gcc_startup_nrf52840.S',
        '../mdk/system_nrf52840.c',
    ],
    include_directories: [
        nordic_include_dirs,
        cmsis_corem_include,
        inc_framework,
        libc_header_include, # <----- libc header include
    ],
    c_args: [
        '-DCONFIG_GPIO_AS_PINRESET',
        '-DNRF52840_XXAA',
    ],
    cpp_args: [
        '-DNRF52840_XXAA',
        '-DDEBUG_ASSERT_NO_STDIO',
    ],
    dependencies: [
        nrf_common_drivers_dep, 
        libcxx_header_include_dep, # <----- libcpp header dep
        arm_dep,
    ],
    native: false
)
```


## Logging Guidelines

Log statements shall not be checked into framework code outside of demonstration platforms and applications. This means *no* log statements in the framework core source and framework utilities.

Log statements can be added for debugging purposes, but should be removed before changes are checked in to master.

Logging can be checked in to user platforms and applications. We recommend that all such logging should be fully disable-able. Logic must not stay in even if the log prints are disabled.

## Meson Guidelines

For more information on the [Meson](https://mesonbuild.com) build system, see the following links:

* [Meson Tutorial](https://mesonbuild.com/Tutorial.html)
* [Meson Usage Guide](https://mesonbuild.com/Manual.html)
* [Meson Reference Manual](https://mesonbuild.com/Reference-manual.html)

### Anti-Patterns to Avoid

* Do not embed full file paths in the tree. Relative paths work fine, meson will take care of it
* Do not use `target_machine` when cross-compiling. You should pretty much always use `host_machine`:
	* `build_machine` is the desktop or laptop computer you are using for your development
	* `host_machine` is the low powered IoT device, ARM board or equivalent that will run your program
* Avoid manual dependency lists with custom targets or run_targets
	* This happens often when defining custom targets or run targets that operate on the output of other custom targets.
	* In Meson, outputs carry dependency information with them. If you use the output of one target as an input to another, Meson will automatically set up all the dependencies.
   
## Running Sanitizers

Meson supports various sanitizers through built-in options:

```
meson buildresults --debug -Db_sanitize=address
meson buildresults --debug -Db_sanitize=thread
meson buildresults --debug -Db_sanitize=undefined
```

We recommend using them to check your host code. If you're not sure where to start, enable a sanitizer and run the unit tests:

```
make test-framework
```

You can use `lldb` or `gdb` on the resulting image for a better debugging experience.

Helpful sanitizer references:

* [Sanitizer Wiki](https://github.com/google/sanitizers/wiki)
* [ThreadSanitizer Manual](https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual)
* [ThreadSanitizer Report Format](https://github.com/google/sanitizers/wiki/ThreadSanitizerReportFormat)
* [ThreadSanitizer: About Races](https://github.com/google/sanitizers/wiki/ThreadSanitizerAboutRaces)
* [ThreadSanitizer: Popular Data Races](https://github.com/google/sanitizers/wiki/ThreadSanitizerPopularDataRaces)
* [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer)
* [AddressSanitizer: Container Overflow](https://github.com/google/sanitizers/wiki/AddressSanitizerContainerOverflow)
* [MemorySanitizer](https://clang.llvm.org/docs/MemorySanitizer.html)

## Naming Guidelines

**Use Names that Denote Purpose**

Purpose is necessary for understanding source code. A descriptive name is a name that clearly describes a purpose. Names should be long enough to clearly describe an element's purpose. Prefer common names and terms over unusual ones.

Each part of a compound name should follow the rules of straightforward names. The name components should appear in logical, left-to-right English reading order. `MoveTrayLeft()` is better than `TrayMoveLeft()`.

Unusual or domain-specific terms, acronyms, and abbreviations should be defined in the project glossary and used consistently in the source code

Verify that your names are not misspelled!

**When Choosing Names, Avoid:**

* Misspelled names
* Similar names:
    * Bad: `item` & `items`
    * Good:`itemList` & `selectedItem`
* Names with non-alphabetic characters
    * Digits can be confuseing
    * What is the difference between variables score1, score2, and score3? Better names!
* Unfamliar or unusual names
    * Long or uncommon names (alabaster, determinant)
    * Unusual cryptic names (pilar, betwixt)
    * Fake names (fizzbin, blurp)
    * Slang or puns (tunaGuitar, dork, farmOut)
    * Domain-specific names SHOULD be used because they are familiar to your project sponsors, users, and domian experts.
        * Define these terms in your glossary.
* Poor compound names
* Meaningless parts of compound names are fluff and should be avoided:
    * Utility or Util
    * Assistant
    * Helper (every source code element helps something)
    * Manager
    * Controller
    * Administrator
    * What kind of managing or controlling is hpapening? These terms are usually too vague to use.
* Redundant names should be avoided
    * `Matrix::clear` NOT `Matrix::ClearMatrix`
* Acronyms or abbreviations
    * They are easy to misinterpret
    * Why do we use `int i = 0;`?
        * Fortran: variable with name starting with `i .. n` was default integer type
    * Avoid acronyms or abbreviations unless they're used in the problem domain or common in everyday writing (min & max)
* Cryptic name codes
    * These names are gibberish to the uninformed. They are only slightly clearer to the informed.
    * ncd = non-comforming-data.
        * How would you know?
        * ex: `ncdAddress`
    * This means *no Hungarian notation*

## Use Clear Expressions

The best code is easily understandable code. We use the following guidelines in developing the framework:

* [Avoid Magic Numbers](#avoid-magic-numbers)
* [Avoid Cryptic Expressions](#avoid-cryptic-expressions)
* Prefer native syntax to aliases
* [Avoid Anonymous Procedures](#avoid-anonymous-procedures)
* [Avoid procedure chaining](#avoid-procedure-chaining)
* [Avoid multiple statements per line](#avoid-multiple-statements-per-line)
* [Prefer positive logic to negative logic](#prefer-positive-logic-to-negative-logic)
* [Use indentation to explain program logic](#use-indentation-to-explain-program-logic)
* [Improve readbility using whitespace](#improve-readability-using-whitespace)
* [Beware Blind Consistency](#beware-blind-consistency)

### Avoid Magic Numbers

Associate a symbolic name with the value. Named symbols representing magic numbers can be `enum`, `const` value, `#define`, or `constexpr`.

An uncomplicated way to improve existing code: replace every magic number you find with a named symbol.

### Avoid Cryptic Expressions

Expressions can appear cryptic by being too short or too long. Shorter expressions are preferred, but only if they are clear

### Avoid Anonymous Procedures

Anonymous procedures are those that have no name, such as lambdas. They are useful, but can be difficult to understand.

If your anonymous procedure has more than two statements, use a named procedure.

Do not chain anonymous functions.

### Avoid Procedure Chaining

Procedure chaining is when one procedure after another is called without interruption.

```
container.get().doAThing().
```

Chained procedures are like a run-on sentence in conventional writing. Readbility suffers because the author is trying to be too concise.

### Avoid Multiple Statements Per Line

We read source-code top-down because that's the way single-line statements are executed. As a result, it's easy to overlook multiple statements on the same line.

This doesn't mean you can't declare more than one variable per line.

### Prefer positive logic to negative logic

Most people find negative logic harder to understand than positive logic. This is especially true when multiple conditions are involved.

Prefer positive names to negative ones: `IsFull()` is better than `IsNotFull()`.

### Use indentation to explain program logic

Developers don't always indent consistently. Inconsistent indentation is better than none - but it can be distracting and misleading.

Left-align curly braces so you can see there's a matching pair. This also adds vertical whitespace and makes subsequent statement groups easier to read.

### Improve readbility using whitespace

Readablity helps readers understand the program operation. Whitespace, line length, page position all affect readability.

Whitespace can be viewed as "negative space". Too little whitespace makes source code dense, cluttered, and hard to understand. Would you be able to process a dense book for hours on end? No!

When we read source code, we want the logic to be clear and direct. We want related statements grouped together. We want unrelated chunks separated from one another.

### Beware blind consistency

Consistency can reduce the amount of information you need to remember.

Even so, consistency isn't a standalone virtue - it's only a magnifier. If you consistently pay your bills on time, it's good for your credit rating. If you're a consistent liar, it's bad for your relationships. Consistency makes good actions better or bad actions worse.

Consistency reduces flexibility, which can be undesirable. The balance between consistency and flexbility should be evaluated on a case-by-case basis.

#### Names and Types

Use names and types consistently.

If you use the name `Clear()` to clear values in one module, then use the name `Clear()` everywhere values are cleared.

If you use the `short type`for a specific value in one module, then use the `short` type everywhere that value is used. This includes variables, function parameters, return types, etc.

#### Namespaces

Namespaces let you group types or sub-namespaces. A namespace normally represents a category, and each type has a purpose.

For example, this is a category, not a purpose:

```
System.Math // (class)
```
Math should be a namespace, and each operation should be a separate class.

```
Math.Sine(x) -> Math::Sine.compute(x)`
```

Each class should conform to the single responsibility prinple.

## Use Symmetry

Symmetry refers to pleasing proportion, arrangement, or balance. It implies uniform structure or behavior, reducing the amount of information you need to remember. Unfortunately, developers often overlook symmetry.

### Parameter Names

Program operations are often symmetrical. For instance, you might wite data to a file, then read it back later. Symmetrical procedure names make this clearer. Naming pairs are useful for public interfaces.

The following name pairs are nearly universal:

* on/off
* start/stop
* enable/disable
* up/down
* left/right
* open/close
* read/write
* get/set
* empty/full
* push/pop
* create/destroy

Just because a symmetrical name exist, doesn't mean both operations should be implemented. A good design will normally have far more getters than setters, for instance.

Good API pairs:

* `Motor::Start()`, `Motor::Stop()`
* `start_motor()`, `stop_motor()`
* `StartMotor()`, `StopMotor()`

Bad pairs:

* `Motor::start(), motor::disable() // no symmetry`
* `start_motor, Stop_motor // inconsistent caps`
* `StartMotor, stop_motor // inconsistent format`

### Parameter Order

Symmetry can be applied to parameter order. Try to use the same parameter order

Consider the C `string.h` functions. In each procedure, the first parameter is the destination string. In all but one procedure, the second parameter is the source string. The parameter order is consistent and follows the right-to-left semantics of assignment orders.

Bad symmetry can be seen in the file APIs. For `fprintf()` and `fscanf()`, the `FILE* stream` argument is first. For `fputs()` and `fgets()`, the `FILE *stream` argument is last! The APIs could be improved by always using `FILE* stream` as the first parameter.

## Use suitable comments

A comment is source code metadata, and it can serve one of two broad purposes. It can document a [programming interface](#interface-comments) or it can help [explain implementation details](#implementation-comments).

Good comments explain the code and are updated when the code is modified. Trivial comments add useless clutter. Obsolete comments are useless and confusing.

Though self-explanatory code is often feasible, language semantics limit your explainations. For example, if your code has an emtpy `catch` clause to ignore file access errors, how can you make it self-explanatory? In this situation, a comment can explain the *purpose* of the empty `catch` clause.

Comments should immediately precede the statements they describe.

### Interface Comments

Interface comments provide details about the purpose of types, procedures, parameters, return values, and function effects. These comments are most useful for public interfaces, but they can also be supplied for private interfaces.

Objections to in-source interface comments are superseded by the value of the API documentation or IDE prompts.

Interface comments also act as section dividers, preventing procedures from visually blending together.

### Implementation Comments

Implementation comments describe algorithms or other operational details. They are best used to explain unusual logic or instructions. They are also used to document the purpose behind a particular approach, especially if it is not straigtforward.

Trivial and unnecessary comments add clutter.

It is better to use assertions, not comments, to document constraints. If the assertion isn't feasible, a sensible comment can be used.

Proper comment placement improves readability:

* Comments should always be located just ahead of, or to the right of, the statement they explain
* Never place comments beneath the statement or in the middle of it
* Avoid vertical whitespace between a statement and its comment, as that disconnects them visually

## Postpone Optimization

As Jerry Fitzpatrick says:

> Most project sponsors would rather have a stable program delivered on-time than a slightly faster and more efficient program delivered late.

Bug prevention and writing quality code are better metrics for development. Processors operate at incredible speeds. It's better to implement features directly and clearly. Once they are released, we can focus on optmizing features that affect users negatively.

> Let's lose the obsession with software performance, and realize that performance is only one of many characteristics we're accountable for - and seldom the most important one. --Josh Lane

Efficiency and performance are only problems if the requirements haven't been met. Optimization usually reduces source code clarity, so it isn't justified for small gains in efficiency or performance. Our first priorities should be correctness, clarity, and modest flexibility.

Optimization guidelines from Jerry Fitzpatrick:

* Never optimize unless you need to
* Ignore optimization until the code works correctly
* Use measurement tools, not guesses, to guide optimization
* Focus first on algorithms, not code tweaks
* Data type selection in strongly-typed languages can be a form of premature optimization - you start thinking about size, bit-width, signedness, etc.
    * Start small - use the type consistently - switch when you need to

## Use atomic initialization

The phrase "atomic initialization" means to completely initialize an instance prior to use. Every instance member is set to either a sepcific or a efault value, putting the instance into a known state.

What we don't want is instances that are initialized little-by-little. They start life partially initialized, then you need to call additional functions before use.

**Incremental initialization almost always indicates a poor design.** Incremental initialization is clumsy, error prone, and opposes encapsulation.

Incrememntal initialization has several problems:

1. The instance must have public data members or setters, otherwise it can't be initialized incrementally
2. Even if those members are intended only for initialiation, they can be used at any time for any reason
3. The benefits of encapulsation are lost
4. When an instance is intended to be initialized incrementally, it's rarely clear which data members and procedures should be used for initialization.
5. Worse yet, initialization may need to occur in a specific order
6. A bigger problem is that no one knows whether the instance is fully initialized
    * Client code must somehow determine whether it's initialized or not
    * To check initialization, the code must check the value of one or more members
    * That requires additional conditional logic, making the program larger and more error-prone

Together, these ambiguities make it difficult for client code to initialize the instance correctly.

Sometimes developers will add an `isInitialized` flag for client code to use. This makes the checks simpler and less error-prone, but conditional logic is still neeeded to check the flag. Getting the instance properly initialized also remains a problem.

Atomic initalization avoids all of these problems. Data members and setter procedures can be kept private, preserving encapsulation. Iniitalization values and order dependencies are handled in one place. There's no need for clients to perform initialization checks, because they can be sure that the instance is always fully initialized.

With OO languages, a constructor normally performs initialization. Overloaded constructors are an appropriate choice if an instance can be initialized in several ways. However, each constructor should ensure that the instance is fully initialized.

 With traditional programming languages, a factory-like design pattern should be used.

 To quote Jerry Fitzpatrick:

 > On occasion, I've seen classes with an `Initialize()` procedure that is expected to be called by the client code. This is just sloppy. Initialization is the constructor's purpose, and its guaranteed to be called when the instance is created. By contrast, there's no guarantee that client code will call the `Initialize` procedure, or know when to call it.

Instances should always use atomic initialization to avoid bugs and keep the implementation clear. Atomic initialization is also a pre-requisite for immutable instances.

## Prefer immutable instances

An instance is immutable if its state can't be changed after initialization. Immutability is primarily a feature of functional and OO languages. Immutability is the ultimate form of encapsulation. The data members of the instance are not only hidden, but their values can't be changed after initialization.

Immutability is enabled by the language and enforced by the compiler. Immutable instances can be changed by writing directly to memory or using reflection, though it isn't a wise action.

To develop an immutable type in OO language:

* You must use atomic initialization
* You must not have any public settters or public mutable data members
* Any procedure that wuld normally change the state of the instance should return a modified instance instead

Regardless of visibility, immutable instances are totally harmless if they:

* Are correctly initialized
* Don't change the state of another instance (directly or indirectly)
* Aren't made mutable by casting or other means

> Immutability provides safety in that no problems arise if the immutable objects are shared.
> - Liskov and Guttag

Benefits of immutability:

* An instance can be copied without creating a new instance
    * Instead, the instance can be "copied" by copying a reference to the existing instance
    * Because the reference is smaller than the instance, memory usage is reduced and perforamnce is increased
* Concurrent threads or processes can share immutable instances without the need for locking
* We are far less concerned about the coupling of immutable instances.

Every time you use an immutable instance, you automatically prevent bugs. For this reason, it's best to create immutable types whenever possible. Even if the type has procedures that might slow performance, keep it immutable until its performance has been proven to affect users.

Not every type should be immutable. For example, control class instances have frequent state changes.

## Manage program inputs

Our golden rule is: **Validate inputs only at the boundary of your program.**

Validation code increases program size and reduces performance, so it's important to judge the reliability of the source accurately. To make your program robust, you should validate every input from unreliable data sources.

Every data source (file, database, web serice, etc.) controlled exclusively by your team should be considered reliable. With some exceptions, data sources shared with other groups can't be considered reliable, since you don't have full control over them. This is especially true of an external system whose interface is non-standard, poorly documented, or error prone.

You should always validate data in just one place: the point where your program obtains the data. By checking the values at this boundary, you can stop invalid data from entering your program. With validated input data, there's no need to add checks anywhere else.

Prevent input errors from occurring, instead of reporting them.

## Avoid using primitive types for physical quantities

A physical quantity always has two elements to it: a magnitude and a unit designation. For example:

* 4.2 meters
* 112.7 Pascals

Do not represent them with native types, such as `int`. There is no way to ensure that quantities are combined in the proper way. Bugs will leak in.

We recommend using the `units` library provided with the framework. You can also make custom data types for each physical quantity, if needed.

```
constexpr auto distance = 5_m;
constexpr auto area = 2_m * 2_m;   // area == 4_sq_m

class myClass
{
  public:
    myClass() : m_speed(make_unit<miles_per_hour_t>(100)) {}
  private:
    miles_per_hour_t m_speed;
};

meter_t distance_m(10);
meter_t distance(10_m);
```

The `units` library provides support for defining custom units with the `UNIT_ADD` macro:

```
UNIT_ADD(length, foot, feet, ft, unit<std::ratio<381, 1250>, meters>)
```

More information can be found in the [`units` library documentation](http://nholthaus.github.io/units/).

## Compare floating-point values correctly

Precision is the primary concern when comparing floating point values. A computed result can't be more precise than its least precise input. Using a high-precision data type can help preserve precision, but it can't increase it.

Here are guidelines for specifying precision:

* Any digit besides zero is always significant
* Trailing zeroes are significant if they appear after the decimal point
* leading zeroes are never significant
* Trailing zeroes before the decimal point may or may not be signicant, making the precision indeterminate

Most microprocessors use IEEE 754 standard, which states:
* `float` = 7 significant digits of precision
* `double` = 15 significant digits of precision

Use an epsilon for comparing floating point values. Because precision is a relative measure, floating-point values shouldn't be compared using absolute values.

For example, prefer this:

```
if(fabsf(x - t[i].f) < FLT_EPSILON))
{
    // ...
}
```

over:

```
if(x == t[i].f)
{
    // ...
}
```

## Reduce Conditional Logic

As the number of conditional statements increases, the number of program states increases so rapidly that testing every combination becomes impossible. Conditional logic also makes programs more error-prone and harder to understand.

We normally think of conditional logic in terms of `if`/`else` blocks and `switch` statements. There are subtler types of conditional logic:

* `try`/`catch` blocks represent a Boolean condition
* `#if`/`#elif`/`#else` preprocessor directives introduce compile-time conditionals

Some conditional logic is unavoidable, but you can reduce the amount of it by using the following schemes:

* Avoid sanity checks
* Use assertions, not conditions, for validity checks
* Use [context and polymorphism](#context-and-polymorphism)

The cost of conditional logic far outweighs any benefit. Adding conditional logic to detect a condition that can't or shouldn't happen is an expensive waste of time. It just makes the program larger, more complicated, and once again increases the risk of bugs. If the procedure doesn't work, then fix it. If you can't fix it, then write a new procedure that works correctly. Don't include sanity checks because it *might not work*.

### Context and polymorphism

It's quite common to see the same conditional logic repeated in many parts of the code. Recurring logic tends to be error prone, especially when the logic needs to be updated due to requirements changes.

At first, centralizing the logic in a single procedure seems like a suitable alternative. Unfortunately, that's not generally feasible because different procedures are called at each of the different conditional logic locations.

The best way to overcome the problem is to establish a context, then call procedures based on that context.

When using an object-oriented language, you can avoid recurring conditions by using polymorphism to establish a context. First, you define a family of related classes. At run-time, you instantiate one of the classes to establish the context. Then you call procedures of that class, performing actions that are appropriate for that instance (context). This is also known as the [State Pattern](../patterns/state.md).

## References

* [ABI Register Cheat Sheet](reference/abi_register_cheatsheet.md)
* [Meson Usage Guide](https://mesonbuild.com/Manual.html)
* [Meson Reference Manual](https://mesonbuild.com/Reference-manual.html)
* [`units` Library Documentation](http://nholthaus.github.io/units/)
