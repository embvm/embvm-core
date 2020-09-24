# Refactoring Guidelines

**Table of Contents**

1. [Removing Duplication](#removing-duplication)
2. [References](#references)

## Removing Duplication

This section comes from the wise mind of [JBrains](https://blog.jbrains.ca).

When I can’t decide whether to remove a certain bit of duplication I’ve found, I fall back on two rules:

1.  Remove duplication only after you see three copies.
2.  If you don’t know how to remove this particular kind of duplication, then write more tests. Either you need more examples to see the pattern, or more examples will show a different, better pattern.

I also remember two guidelines:

1.  Don’t be afraid to remove duplication by introducing a method or class or interface with a stupid name. Remember that you can always improve the name later.
2.  If you sense duplication, but don’t really see it, or can’t explain it to others, then make the surrounding names more precise; then maybe you will see the duplication.

More inspiration for removing duplication, even when you don't understand the code:

> Often, especially when working with legacy code, I notice 6 copies of the same impenetrable 12 lines of code. Even before I know what this code does, I routinely extract this code into a new function, calling it foo(), even though we agree that “foo” does nothing to express its intent or clarify its significance. Over time, as I understand better how this code fits into the rest of the system, I discover a better name, and then rename the function. This doesn’t happen only in legacy code: even in a pristine greenfield environment, I sometimes extract duplication before entirely understanding what it represents, and so I call the new thing foo() or Foo until I can figure out what it does and what it means. Since I value minimizing duplication over maximizing clarity, I routinely introduce new containers for code before I know what they represent, confident that I will be able to give them more suitable name as I learn more about the design

## References

* [jbrains: The Four Elements of Simple Design](https://blog.jbrains.ca/permalink/the-four-elements-of-simple-design)
* [jbrains: When Should I Remoev Duplication?](https://blog.thecodewhisperer.com/permalink/when-should-i-remove-duplication)
