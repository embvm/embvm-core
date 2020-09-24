# Pattern: Singleton

Creational Pattern

Ensure a class has only one instance, and provide a global point of access to it.
Encapsulated "just-in-time initialization" or "initialization on first use".

## Diagrams

* Include / link to visual representations of this pattern

## Context

* How does this pattern help to complete larger patterns?
* What is the intent of this pattern?

## Problem

Application needs one, and only one, instance of an object. Additionally, lazy initialization and global access are necessary.

## Solution

Make the class of the single instance object responsible for creation, initialization, access, and enforcement. Declare the instance as a private static data member. Provide a public static member function that encapsulates all initialization code, and provides access to the instance.

The client calls the accessor function (using the class name and scope resolution operator) whenever a reference to the single instance is required.

Singleton should be considered only if all three of the following criteria are satisfied:

Ownership of the single instance cannot be reasonably assigned
Lazy initialization is desirable
Global access is not otherwise provided for
If ownership of the single instance, when and how initialization occurs, and global access are not issues, Singleton is not sufficiently interesting.

The Singleton pattern can be extended to support access to an application-specific number of instances.

The "static member function accessor" approach will not support subclassing of the Singleton class. If subclassing is desired, refer to the discussion in the book.

Make the class of the single instance responsible for access and "initialization on first use". The single instance is a private static attribute. The accessor function is a public static method.

Check list

Define a private static attribute in the "single instance" class.
Define a public static accessor function in the class.
Do "lazy initialization" (creation on first use) in the accessor function.
Define all constructors to be protected or private.
Clients may only use the accessor function to manipulate the Singleton.


## Consequences

* What are the benefits of using this pattern?
* What are the liabilities with using this pattern?

Deleting a Singleton class/instance is a non-trivial design problem. See "To Kill A Singleton" by John Vlissides for a discussion.

The Singleton design pattern is one of the most inappropriately used patterns. Singletons are intended to be used when a class must have exactly one instance, no more, no less. Designers frequently use Singletons in a misguided attempt to replace global variables. A Singleton is, for intents and purposes, a global variable. The Singleton does not do away with the global; it merely renames it.
When is Singleton unnecessary? Short answer: most of the time. Long answer: when it's simpler to pass an object resource as a reference to the objects that need it, rather than letting objects access the resource globally. The real problem with Singletons is that they give you such a good excuse not to think carefully about the appropriate visibility of an object. Finding the right balance of exposure and protection for an object is critical for maintaining flexibility.
Our group had a bad habit of using global data, so I did a study group on Singleton. The next thing I know Singletons appeared everywhere and none of the problems related to global data went away. The answer to the global data question is not, "Make it a Singleton." The answer is, "Why in the hell are you using global data?" Changing the name doesn't change the problem. In fact, it may make it worse because it gives you the opportunity to say, "Well I'm not doing that, I'm doing this" – even though this and that are the same thing.

## Known Uses

* Describe known applications for this pattern
* Link to uses of the pattern within the source code

## Variants

Link to any patterns that are varients of this

## Related Patterns

Abstract Factory, Builder, and Prototype can use Singleton in their implementation.
Façade objects are often Singletons because only one Façade object is required.
State objects are often Singletons.
The advantage of Singleton over global variables is that you are absolutely sure of the number of instances when you use Singleton, and, you can change your mind and manage any number of instances.


## Notes

Singleton Base Class (generic)

Read and review: https://www.michaelsafyan.com/tech/design/patterns/singleton

https://dzone.com/articles/singleton-anti-pattern

Anybody in the system can make a singleton by suvclassig the base class

According to C++ Singleton design pattern I wrote a singleton template

```
template <typename T>
class Singleton
{
    public:
        static T& getInstance()
        {
            static T instance;
            return instance;
        }
    protected:
        Singleton() {}
        ~Singleton() {}
    public:
        Singleton(Singleton const &) = delete;
        Singleton& operator=(Singleton const &) = delete;
};
```

About the seconde one I omit the destructor and then complier would generate a default inline public one. That seems more concise and more efficient.

```
class Logger:public Singleton<Logger>    // derived Singleton<Logger>
{
    friend class Singleton<Logger>;
    private:
        Logger(){};
    public:
        void print(void){std::cout << "hello" << std::endl;}
};
```

https://codereview.stackexchange.com/questions/83420/a-singleton-base-and-derived-class

https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/1008289#1008289
