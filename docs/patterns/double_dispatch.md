# Pattern: Double Dispatch Visitor

TBD

## Diagrams

TBD

## Context

TBD

## Problem

TBD

## Solution

TBD

## Consequences

Benefits of Double Dispatch over the other approaches

1. Still fast
    * 2 virtual function calls, no `dynamic_cast`s or long `if…else if` blocks
    * overall time is still the same as both manual RTTI and single dispatch (Timings)
2. Encapsulation
    * The Person class still controls all impl details for how it will react to any specific animal type
3. Separation of responsibility
    * The logic for reacting to any individual animal is cleanly separated from the logic for reacting to any other animal
    * This improves readability, and if the logic for reacting to any individual animal became sufficiently complex, we could put it into its own translation unit (.cpp file)
1. More stable class interfaces for the Animal hierarchy – Adding a new animal to the hierarchy doesn’t affect any base or derived class – Animal::Visit() accepts an interface now, which makes it a pathway for ALL type-specific behavior.

## Known Uses

TBD

## Variants

TBD

## Related Patterns

TBD

## References

TBD

## Notes

```
A Double-Dispatch Visitor
"All problems in computer science can be solved by another level of indirection." – David Wheeler
Hang, on things are about to get a little weird.
At present, we have created what I’ve called a "single dispatch" visitor. Classes that derive from Animal override a virtual void Visit method that receives a ReactionVisitor in order for us to separate the animal-specific logic.
The problem is that the animals themselves are now responsible for Person-specific logic.
By also giving our ReactionVisitor a virtual Visit method, we can address this shortcoming.
1
2
3
4
5
6
7
8
9
10
11
12
13
struct AnimalVisitor
{
virtual void Visit(Animal*) = 0;
};
struct ReactionVisitor : AnimalVisitor
{
void Visit(Animal*) override
{
// ????
}
Person* person_ = nullptr; // person doing the reacting
};
This doesn’t appear very useful…yet. We still want:
1. Encapsulation, while maintaining
2. Separation of responsibilities
Starting with #1, let’s make ReactionVisitor a friend of our Person class
1
2
3
4
5
struct Person
{
/*...*/
friend ReactionVisitor;
};
This is one of the use cases where a friend class makes perfect sense. Logically, ReactionVisitor is part of the same class as Person, so it should have access to Person‘s private members.
Making it a friend (vs a nested class) allows us to cleanly separate the interfaces, de-clutter the Person class, and as a bonus leaves the ReactionVisitor directly testable.
This achieves encapsulation, but so far our AnimalVisitor::Visit method only knows about the base Animal class, which doesn’t really help with separation of responsibility; we’re back to step one. So let’s do the obvious thing and make AnimalVisitor know about each Animal type.
1
2
3
4
5
6
7
8
9
10
11
12
13
struct AnimalVisitor
{
virtual void Visit(Cat*) = 0;
virtual void Visit(Dog*) = 0;
/*...*/
};
struct ReactionVisitor : public AnimalVisitor
{
void Visit(Cat*) override;
void Visit(Dog*) override;
Person* person_ = nullptr;
};
Ignoring any potential issues with this approach, how do we make it work? That is, how do we ultimately call ReactionVisitor::Visit(Cat*)?
First, we reclaim the ReactTo logic from the derived classes and give it to ReactionVisitor instead:
1
2
3
4
5
6
7
8
9
10
11
struct ReactionVisitor : public AnimalVisitor
{
void Visit(Dog* _dog) override{
if (_dog.GetBreed() == DogBreed.Daschund)
person_.TryToPet(this);
else
person_.RunAwayFrom(this);
}
//... (other overridden Visit methods)
Person* person_ = nullptr; // person doing the reacting
};
In this way we preserve separation of responsibility without leaking implementation details for Person.
Next, the entire Animal::Visit hierarchy is refactored to so that derived classes call through to the visitor’s Visit method with the this pointer (will explain why shortly):
1
2
3
4
5
6
7
void Cat::Visit(AnimalVisitor* _visitor){ // overridden virtual method
_visitor->Visit(this);
}
void Dog::Visit(AnimalVisitor* _visitor){ // overriden virtual method
_visitor->Visit(this);
}
Finally, we can leave Person::ReactTo untouched
1
2
3
4
void Person::ReactTo(Animal* _animal){
ReactionVisitor visitor{this};
_animal->Visit(&visitor);
}
How does double dispatch work?
…I warned you things would get weird. It can be hard to wrap your head around without visualization.
We have 2 virtual dispatch calls:
1. From Person::ReactTo, we call Animal::Visit, which will dispatch to the appropriate overridden Visit (Cat::Visit, Dog::Visit, etc.)
1. From the overridden Cat::Visit(AnimalVisitor*), we call AnimalVisitor::Visit, which will dispatch to the appropriate overridden AnimalVisitor::Visit method (ReactionVisitor::Visit(Cat*))



The double dispatch pattern is a flexible design pattern that confers a number of benefits:
* separation of responsibility for type-specific logic
* performance over dynamic_cast and check or enum/string comparison
* new functionality can be added without touching any class headers
* (It’s also been battle-tested for about as long as polymorphism has been around. Dr. Dobbs has an article on this exact pattern since 1998, and I wouldn’t be surprised to find artifacts of it from long before.)
When your class hierarchy is finalized (if), you can begin decommissioning the visitor pattern in favor of an even faster std::variant approach.


Fixing issues with our double dispatch implementation
The implementation I’ve shown still has some problems, let’s examine them and see how we might mitigate them.
But first let’s review our current double dispatch implementation
Review of our current double dispatch impl

1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
// AnimalVisitor.h
struct Cat;
struct Dog;
// ... (other forward declarations)
struct AnimalVisitor
{
virtual void Visit(Cat*) = 0;
virtual void Visit(Dog*) = 0;
/*...*/
};
// Animal.h
struct AnimalVisitor;
struct Animal
{
virtual void Visit(AnimalVisitor* _visitor) = 0;
// ...
};
// Dog.h
#include "Animal.h"
struct Dog
{
virtual void Visit(AnimalVisitor* _visitor){
_visitor->Visit(this);
}
};
// Cat.h etc follow Dog.h impl
// ReactionVisitor.h
#include "AnimalVisitor.h"
#include "Cat.h"
#include "Dog.h"
struct ReactionVisitor : public AnimalVisitor
{
void Visit(Cat*) override{/*...*/}
void Visit(Dog*) override{/*...*/}
Person* person_ = nullptr;
};
// Person.h
#include "Animal.h"
#include "ReactionVisitor.h"
struct Person
{
/*...*/
void ReactTo(Animal* _animal){
ReactionVisitor visitor{this};
_animal->Visit(&visitor);
}
};
Issue #1: Each new derived Animal must override Visit() in the same way
It can be easy to forget to override the Visit() method, and even when it’s done it is a tedious copy-paste task. Leaving it this way is safe if your team is disciplined, has keen-eyed code reviewers, and solid unit test coverage.
Or, you could take a Mixin/CRTP  approach to idiot-proof things with extremely minimal runtime overhead.
The idea is that all classes derive from a common base that automatically provides the Visit(AnimalVisitor*) functionality. Let’s see it in action:
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
struct Animal
{
virtual void Visit(AnimalVisitor* _visitor) = 0;
// ...
};
template<class T>
struct VisitableAnimal : Animal
{
void Visit(AnimalVisitor* _visitor) override
{
_visitor->Visit(static_cast<T*>(this));
}
};
struct Cat : VisitableAnimal<Cat>
{
};
struct Dog : VisitableAnimal<Dog>
{
};
Issue #2: It’s easy to add new Visitors, painful to add new derived Animal classes
Adding a new Animal requires that we update the AnimalVisitor base class:
1
2
3
4
5
6
7
struct AnimalVisitor
{
virtual void Visit(Cat*) = 0;
virtual void Visit(Dog*) = 0;
/*...*/
virtual void Visit(NewAnimal*) = 0;
};
This affects every derived AnimalVisitor class. Personally, I believe this is a blessing in disguise; you now have a very easy way to find all the functionality that needs to be updated to handle the addition of a new Animal.
However, not all visitors need to know about every derived Animal all the time. For example, if I simply want a visitor that acts as a Filter for Cat objects, I don’t want to implement an empty Visit(Dog*).
What can be done?
There’s a couple of mitigative steps we can undertake to insulate visitors from new derived classes.
Derive from a "do-nothing" base Visitor
This approach also uses CRTP like we performed above with VisitableAnimal. The idea is to create a derived class that will automatically "do nothing" for each derived Animal, allowing your derived visitor to opt-in to doing something. In C++17, the addition of variadic using declarations makes this fairly easy:
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
// DefaultDoNothingAnimalVisitor.h
#include "AnimalVisitor.h"
template<class T>
struct SingleDoNothingAnimalVisitor : virtual AnimalVisitor
{
using AnimalVisitor::Visit;
void Visit(T*) override{}
};
template<class... T>
struct MultipleDoNothingAnimalVisitor : public SingleDoNothingAnimalVisitor<T>...
{
using SingleDoNothingAnimalVisitor<T>::Visit...;
};
// strong typedef
struct DoNothingAnimalVisitor : public MultipleDoNothingAnimalVisitor<Cat, Dog, ...>
{};
Now we can write our CatFilter visitor without worrying about the rest of the hierarchy:
1
2
3
4
5
6
7
8
9
struct CatFilter : DoNothingAnimalVisitor
{
using DoNothingAnimalVisitor::Visit;
void Visit(Cat* _cat) override
{
cats_.push_back(_cat);
}
std::vector<Cat*> cats_;
};
(Here’s a working demo)
Adding a new derived class will now touch the MultipleDoNothingAnimalVisitor base class, but no other code needs to change.
Dispatch to a Visit(Animal*) default
This approach has us adding an additional Visit method to the base AnimalVisitor to allow dispatch to a generic Animal* catch-all:
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
struct AnimalVisitor
{
virtual void Visit(Animal*) = 0;
// ... (other Visit methods)
};
struct DefaultAnimalVisitor : AnimalVisitor
{
void Visit(Animal*) override{}
void Visit(Cat* _cat) override{
Visit(static_cast<Animal*>(_cat);
}
void Visit(Dog* _dog) override{
Visit(static_cast<Animal*>(_dog));
}
// ...
};
This allows you to write the inverse of a CatFilter — a visitor that will visit everything except for Cats:
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
struct DefaultAnimalVisitor : AnimalVisitor
{
void Visit(Animal*){}
void Visit(Cat* _cat){Visit(static_cast<Animal*>(_cat));}
void Visit(Dog* _dog){Visit(static_cast<Animal*>(_dog));}
// ...
};
struct AllButCatFilter : DefaultAnimalVisitor
{
using DefaultAnimalVisitor::Visit;
void Visit(Animal* _animal) override
{
animals_.push_back(_animal);
}
void Visit(Cat*) override{/*intentionally blank*/}
std::vector<Animal*> animals_;
};
(Here’s a working demo)
A mixture of the above
The double dispatch pattern is flexible enough where you could mix a "do-nothing" approach and a "dispatch to a default approach" into a "dispatch into a fallback that does nothing by default" kind of approach.
Or neither. The point is that we can insulate our derived visitors in a number of ways.
Issue #3: There’s a lot of boilerplate if you only care about visiting a single type
What’s easier, writing a new class to inherit from AnimalVisitor, then capturing whatever state you need as member variables, and finally overriding the correct method(s) to get what you want, or writing a single dynamic_cast?
It’s hard to say that dynamic_cast is "wrong". It may well be more readable and fast enough for your needs. The only danger here is that when the code inevitably needs to support another type, will the next programmer simply copy + paste the dynamic_cast or will they recognize when a refactor is needed?
In my experience, too often is the copy + paste route taken, and this isn’t caught until code review time (or later), at which point the developer will be annoyed by your badgering to suddenly implement all the machinery needed for a double dispatch implementation. So your code review comment becomes a tech debt item with low priority that is eventually forgotten (until the next copy + paste).


Reusing the Visitor machinery
Point #4 above is what makes this pattern extra powerful. No longer do you need to add intrusive new virtual functions to the base class to support type-specific behavior. Clients who don’t use any type-specific machinery also don’t have to rebuild their code every time a new derived class is added.
What are some of the situations where you could reuse double dispatch?
In a real system, it wouldn’t just be Person::ReactTo that might require knowing the actual type behind the base Animal* pointer.
* You can implement filtering with double-dispatch. E.g., "give me only the Cats from a vector<Animal*>"
* You can expose your vector<Animal*> to Python as a heterogeneous list of [Cat, Dog, etc.] via a visitor that constructs appropriate PyObjects for each type
* You can co-opt it to make serialization of your generic containers easier.
    * In the next blog post I will show a demonstration of using this machinery to implement I/O for a vector<Animal*>
```




