# C++ Guidelines

**Table of Contents**

1. [General Guidelines](#general-guidelines)
2. [C++ Class Rules](#c-class-rules)
3. [Use of Optional Returns](#use-of-optional-returns)
4. [Avoiding Exceptions](#avoiding-exceptions)
5. [Prefer STL Algorithms to Raw Loops](#prefer-stl-algorithms-to-raw-loops)
6. [Avoid `void*` in Interfaces](#avoid-void-in-interfaces)
7. [References](#references)

## General Guidelines

* Use RAII wherever resource management is performed in the program.
* Use destructors only for releasing resources.
* Don’t try to return a reference when you must return an object.
* Distinguish between prefix and postfix forms of increment and decrement operators.
* use `UINTX_C()` macros for constant initialization
* Never overload `&&`, `||`, or `,`.
* Avoid default lambda capture modes
    * We've seen benefits from requiring developers to explicitly list all of the variables used within the lambda
    * Specifically it forces examination of the lifetime of each object in cases where the lambda may be executed asynchronously
* Do not capture stack variables in a lambda by reference
    * We've observed many examples of implicit lambda captures by reference to stack variables causing lifetime problems when the calling function leaves scope.
    * One important point about C++ and lambdas is that while we’re using lambda capture ”by copy”, it still captures current object (the one referred to by `this`) by reference.
* Dynamic polymorphism works well, but may not be ideal for functions that are called at a high frequency
    * Procedural methods or static polymorphism could be better options for extremely time critical code


## C++ Class Rules

When to use a `class` vs `struct`?

* Classes for objects with operations
* `Struct`s for simpler data structures
* Your `struct`s can have a constructor, operators for conversions
	* More than that? use a class
* Do not call virtual functions in base class constructors
* Constructors should set up variables

Every C++ class should be exactly one of:

* abstract (base class in hierarchy)
* polymorphic and final (derived class in hierarchy)
* non-polymorphic, public destructor (regular class)
* non-polymorphic, protected destructor (non-polymorphic base classes)

In particular:

* Non-leaf classes should be abstract
* Non-polymorphic classes shouldn't be final
* Don't do public inheritance from classes that are not abstract or don't have a protected destructor

Great suggestions from *Effective and More Effective C++*:

* Define a copy constructor and an assignment operator for classes with dynamically allocated memory.
* Prefer initialization to assignment in constructors.
* Make destructors virtual in base classes.
* Have "operator=" return a reference to `*this`

Other notes:

* Member functions should be `const` if possible
    * This communicates that calling this method does not mutate the object
* Static constant integral members of a class are compile-time constants
    * Using them allows for constant folding to happen
* Use class `friend`s to improve encapsulation
    * Using `friend`s makes sense to eliminate thin `set()`/`get()`` functions which might expose internal class details
    * Keep private members private with `friend` functions

## Use of optional Returns

If a function could fail to return a good value, use `std::optional<T>` or `type_safe::optional_ref<T>` as the return type. The user can check to see if the optional value has been supplied. This provides a better alternative to "inout" parameters, `std::pair` returns, or using an "invalid" value (i.e., '-1').

For a given type `T`, `optional<T>` represent an object that can be:
* either a value of type `T`,
* or an “empty” value.

This way a new value is added to the possible values that `T` can hold, which avoids to sacrifice a true value of `T` (like `-1` or `“”`) to represent a `T` that is “empty” or “not set”.

## Avoiding Exceptions

The framework does not use C++ exceptions.

Utilize explicit error handling when required, and implicit if not.

Every function which might fail exists in two versions: `Foo` & `TryFoo`. The latter returns an error code, the former performs an `abort` on failure.

## Prefer STL Algorithms to Raw Loops

In Sean Parent’s [C++ Seasoning talk](http://sean-parent.stlab.cc/presentations/2013-09-11-cpp-seasoning/cpp-seasoning.pdf), he explains how raw loops suffer from several issues:

* They are difficult to reason about
* It is difficult to prove post conditions
* They are error prone and likely to fail under non-obvious conditions
* They may introduce non-obvious performance problems
* They complicate the reasoning about the surrounding code

Instead of raw loops, prefer using an algorithm. The algorithm can be either an existing STL algorithm, or a custom implementation.

## Avoid `void*` in Interfaces

In interfaces, if you have memory where you know the type, use `T*`.

If you don't know the type, use `std::byte*` instead of `void*`. `std::byte*` provides support for pointer arithmetic without casting and is still a generic construct.

## References

* [Tutorial: When to Write Which Special Member Function](https://foonathan.net/blog/2019/02/26/special-member-functions.html)
* [Sean Parent: C++ Seasoning](http://sean-parent.stlab.cc/presentations/2013-09-11-cpp-seasoning/cpp-seasoning.pdf)
