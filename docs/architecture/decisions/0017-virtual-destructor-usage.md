# 2. Virtual Destructor Usage

Date: 2020-10-19

## Status

Accepted

## Context

When trying to link an embvm application that did not use any dynamic memory allocations, we failed to link because of a missing `free` symbol. This is because a virtual destructor in a base class was causing the compiler to generate a deleting destructor.

## Decision

By default, make destructors in abstract classes protected and non-virtual. 

If you encounter a situation where you need to provide a virtual destructor (because you are destroying an object with a base class pointer), then you need to implement a default `operator delete` in your final class if dynamic memory management isn't used. 

If you don't expect a deleting destructor to be called (you would need to manually call `delete` on the base class), then trigger an assertion in case it is called.

```
    void operator delete(void* p)
    {
        assert(0); // should not be called
    }
```

## Consequences

You cannot delete derived objects through base class interface pointers under the default case.

## Further Reading

* [Herb Sutter: Virtuality](http://www.gotw.ca/publications/mill18.htm)
* [Eli Bendersky: Deleting Destructors and Virtual `operator delete`](https://eli.thegreenplace.net/2015/c-deleting-destructors-and-virtual-operator-delete/)
