# Framework Error Model

**Table of Contents**

* [Summary](#summary)
* [Philosophy](#philosophy)
    * [Correctness and Robustness](#correctness-and-robustness)
    * [A Taxonomy of Errors](#a-taxonomy-of-errors)
* [Error Returns from Functions](#error-returns-from-functions)
* [Exceptions](#exceptions)
* [Assertions](#assertions)
* [Design by Contract](#design-by-contract)
* [Panics](#panics)

## Summary

* No exceptions in the framework core
* No exceptions in framework applications, whenever possible
* Use `assert` to document preconditions, postconditions, and logical errors
* Use `static_assert` when possible to enforce compile-time conditions (e.g., "Type must be TriviallyCopiable")
* Panic handlers are called for non-recoverable error conditions
* Do not start cancellation cascade if releasing resources fails
* Do not stop cancellation cascade, unless you are sure that subsequent operations do not depend on the canceled ones
* Make sure that any mutating operation when fails leaves the objects in the state where they can be safely destroyed and reset.
* Apply the above advice regardless of what technique you use for handling errors.

## Philosophy

### Correctness and Robustness

Correctness and robustness are different concerns for our programs. Correctness is about the number of bugs in the program. Robustness is about how the program behaves when an error or unexpected condition occurs, regardless of where the error originated.

### A Taxonomy of Errors

According to Jerry Fitzpatrick, there are only two kinds of errors:

* **Error type A:** an error that can be eliminated by changing your program's design or implementation
    * These errors relate to correctness
* **Error type B:** an error that can't be eliminated by changing your program, but whose impact can be softened
    * These errors relate to robustness

Type B errors can arise from various sources and circumstances. Regardless of how they occur, Type B errors are beyond your control. You have no way to predict them or fix them. They are true exceptions.

Programs can be kept simpler by using assertions for Type A errors and exception handlers for Type B errors. Specifically:

* Never throw an exception for a type A error
    * Instead, expose the error using an assertion or a test (and fix it ASAP)
* If a type B error is based on a status code, not an exception, and it can't be fixed locally:
    * Throw an exception that is equivalent to the status code, allowing the last resort procedure to be used
* Always catch exceptions

An important note emphasized by the distinction between Type A and Type B: bugs do not count as recoverable errors. They should be identified via assertions or tests and corrected in the source. If bugs are detected during program operation, the correct behavior is to halt immediately.

## Error Returns from Functions

We prefer not to return error codes from functions under normal conditions. If the user can take action to address the error code, then returning an error is fine. 

Note that error codes should only be returned for *recoverable* errors. Errors which cannot be recovered must be handled with an `assert()` or `panic()`.

### Handling Functions that Can Fail

If a function can fail, we recommend two alternatives to an error code return:

1. Return a `std::optional<T>` or `type_safe::optional_ref<T>` from the function
2. Implement two variants of the function (alternative to throwing an exception):
    1. `Foo()`, which asserts if there is a failure
    2. `TryFoo()`, which can provide failure information

## Exceptions

Exceptions are rarely used for embedded platforms. No exceptions are used or enabled for the framework core components.

Exceptions may be enabled by framework applications. They should be enabled locally, and framework core components should not have them enabled even in this case.

If exceptions must be used, follow these guidelines:

* You always want to handle exceptions in your program - otherwise there's a "crash"
* The cardinal rule of exception handling is this: Never catch an exception unless you can correct the error it represents.
    * Because the errors behind the exceptions can rarely be corrected, this rule implies that there should be few catch clauses in your program.
* Using a catch clause that does nothing is acceptable if it doesn't hide a bug.
* Because the "failure response" is a last resort, it can generally be implemented using a single procedure and a try/catch block.
    - Add a single last-resort handler near the entry point of the program. The last resort procedure should be called in this catch clause.
    - Example: `try`/`catch` around `app_start()`, the application logic's entry point
    - This last-resort procedure should only be responsible for reporting the error, not for restoring the application to a known good state. The procedure should have parameters that provide the error details and call stack.
* Exception handlers which terminate (such as the last-resort handler) should be helpful:
    * Display an error message to the user
    - Always be clear/concise
    - Always provide specific information whenever possible
    - If the program needs to exit, the message should mention it in a non-frightening manner
    - Log or transmit error details for later analysis by developers

## Assertions

We rely on assertions to expose bugs and implement [Design by Contract](#design-by-contract). Our primary uses are:

* Use `assert` to document preconditions, postconditions, and logical errors
* Use `static_assert` when possible to enforce compile-time conditions (e.g., "Type must be TriviallyCopiable")

We also use assertions to avoid "validity checks". These checks are like sanity checks, except they detect errors that are possible during normal execution. These are normally handled with `if`/`else` conditional blocks or exceptions. Unfortunately, these clutter up code.

The one-line assertion performs the same check as before, but it's easier to understand and minimizes code bloat (no `if`, `throw`, or `catch` statements). The behavior of assertions is also consistent compared with `if` statements, whose actions are developer-dependent.

## Design by Contract

Use Design by Contract (DbC) approaches whenever possible.

### Summary of DbC:

* A procedure has specific inputs and outputs, and will only function correctly if its inputs are valid.
* A precondition is a condition that must be true when a procedure is called. Preconditions are a benefit for the procedure (service) and an obligation of the caller (client)
    * If a precondition isn't satisfied, the procedure may exhibit any behavior whatsoever.
        * Can return an incorrect result, halt the program, or do nothing.
        * May seem odd, but it's an important aspect of DbC
* A postcondition is the condition that must be true when a procedure returns. Benefit for the caller (client) and an obligation of the procedure (service).
    * Because each procedure checks its own postconditions, there is no need for callers to check them.
    * Preconditions and postconditions are verified using assertions.
* A postcondition failure means that the procedure itself has a bug. You should fix the procedure itself.
* Read *Object Oriented Software Construction* for the definitive text of DbC

We use assertions and static assertions in our functions to check pre-conditions and post-conditions whenever possible. Documentation for the function should also include pre/post-condition details. Assertions verify (and document) preconditions and postconditions without being overly-intrusive or reducing the performance of release builds.

In addition to checking preconditions and postconditions, assertions are self-documenting, and more compact than conditional logic. They're also easy to understand and behave consistently whenever they're used. Using if/else or try/catch logic instead of assertions makes your program larger and more difficult to understand. The implementations will vary from developer to developer, producing inconsistent program behavior.

### DbC Guidelines

Here are a few guidelines for using DbC correctly:

- Use assertions, not control structures (`if`/`else`, `try`/`catch`) to check preconditions and postconditions
- Prefer strict assertions to tolerant ones
- Use positive logic in assertions (it's clearer than negative logic)
- One condition per assertion.
    - An assertion with multiple conditions is harder to understand than multiple assertions with one condition each

## Panics

TBD, panic support not implemented.

Error conditions which are non-recoverable should call invoke a panic. Such conditions include:

* Memory fault
* Instruction fault
* MMU access violation
* Non-functioning critical hardware

The panic handler behavior is different for debug mode and release mode.

In debug mode, a panic trace will be printed and the processor will be halted so a developer can attach a debug program.

```
// TBD: show a trace
```

In release mode, the panic will be noted in a boot flag and the processor will be automatically rebooted.
