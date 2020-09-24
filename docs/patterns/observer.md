# Pattern: Observer

Defines a one-to-many dependency between objects so that when one object changes state, all of its dependents are notified and updated automatically. Also known as "publish-subscribe".

* **Classification:** Behavioral Object Pattern

## Diagrams

* Include / link to visual representations of this pattern

## Context

* How does this pattern help to complete larger patterns?
* What is the intent of this pattern?

## Problem

* Two sentence description
* Include example/motivation
* Include the forces at play (applicability)
* Follow with empirical background for the pattern
* Evidence for validity
* Range of valididty

## Solution

* Stated as an instruction
* Describe the structure, dynamics, and implementation heuristics

## Consequences

* What are the benefits of using this pattern?
* What are the liabilities with using this pattern?

## Known Uses

* Describe known applications for this pattern
* Link to uses of the pattern within the source code

## Variants

Link to any patterns that are varients of this

## Related Patterns

Link to any related patterns

## Notes

Observer

A set of template classes to enable the easy creation of objects using the Observer pattern.




The observer pattern is a software design pattern in which an object maintains a list of observers, and notifies of any 
changes. It is often used to implement event handling systems. See the tutorial.




There are two templated classes:-




observer

Derive observers from this class.
Up to eight different notification messages may be defined.




template <typename T1,        typename T2 = void, typename T3 = void, typename T4 = void,
          typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
class observer




observer defines a pure virtual notification member function for each defined type, taking the type as a parameter.
Derived classes are forced to implement these functions.




Example:-




// An observer type that handles int (by value), std::string (by reference)
// and std::complex (by const reference).
typedef  etl::observer<int, std::string&, const std::complex&> observer_type;




class my_observer : public observer_type
{
public:




  void notification(int i);
  void notification(std::string& s);
  void notification(const std::complex);
};




observable

This class is observable from a class derived from observer.
Derive observable classes from this.




template <typename TObserver, const size_t MAX_OBSERVERS>
class observable




void add_observer(TObserver& observer);
Adds an observer to the list. If the maximum number of observers have already been added then an 
etl::observer_list_full is emitted.




void remove_observer(TObserver& observer);
Remove an observer from the list. If the observer is not in the list then there is no change.




void clear_observers();
Removes all observers from the list.




size_type number_of_observers() const;
Returns the number of observers currently in the list.




void notify_observers(T data);
Notifies all of the observers with the supplied data. The data will be one of those configured in the observer type list.

