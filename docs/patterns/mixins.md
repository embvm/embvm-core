  

## Notes

Odin's C++ Mixins Talk: Notes

"Typically you don't want to use virtual functions on a microcontroller because they are not efficient"

-- PJ is that true?

"Aggregation" out of compose-able pieces with glue code: FAIL (he says) - glue code is a lot of code, ends up being the same as if you wrote the whole thing by hand

"inheritance + virtual functions" - e.g., QT - in the micro world, ARM's own dev team uses it for mbed OS. This in Odin's opinion is a FAIL - you have problems with binary size. The optimizer can't see through all of the virtual function calls so can't optimize the stuff you're not using. (Is this true?)
he claims mbed pin toggling 30x slower than assembly - also true?

"Inheritance + CRTP" - also FAIL
CRTP: a trick using templates with which you can make a base class call functions in the derived class
- Read more about Policy Based Class Design
- Odin thought it was the solution for years
- works, efficient, but unteachable and unmaintainable and too much template spaghetti

Odin wants to abstract the glue code so TMP writes the glue code for us. You just compose things.

```
auto thing = mixin::compose(mixin::interface<bells, whistles>, guts, more_guts);
```

InterfaceS: no state. Implementation: can initialize with state:

```
auto thing = mixin::compose(mixin::interface<bells, whistles>, guts{haggis}, more_guts);
```

Terms:
composition - object containing valid set of mixins
valid set - every mixin that has something it requires, another mixin provides that functionality
interface mixins - add to public interface
implementation mixins - private implementation detail
Abilities: not quite a concept, a syntactic requirement plus a semantic thing that is associated. Provides an interface and does some thing to fulfill it.
Requirements: abilities you require from other classes
    e.g., vector control block requires allocator or buffer
    not explicitly specified - inherently specified by the fact that within functions you call other functions

Requirement for an interface mixin: takes one implementation parameter and derives from it publicly

```
template<typename T>
struct bells: T
{
    void ring();
}
```

Don't need to understand why - do that :) (source idea: liberase library)

Anything in the interface mixins gets added to the public interface of the composed object

Syntactic requirements of implementation detail: nothing. Anything can be an implementation detail

How does the public interface find the right mixin to talk to that it requires? How do they work together?

We don't want the interface mixin to know the type of the implementation mixin it wants to call, or know the type of the allocator, etc.

The way they find each other: "abilities"
Abilities are just tags or types

```
struct ringable {};
```

Call something on every function with that ability, we use foreach, pass in our this pointer, pass it the ability that we are looking for, and a polymorphic lambda. The template meta-monster is going to call the polymorphic lambda for every mixin that has the ability, and pass a reference to the mixin in.

```
template<typename T>
struct bells: T{
    void ring()
    {
        for_each(this, ability<ringable>, [](auto &a)
        {
            a.ring();
        }
    }
}
```

How do we associate abilities with an implementation mixin?

guts_impl is the actual class type here. Then we have a variadic pack of abilities that we are associating with the class type. Forwards all the stuff to the guts_impl type.

```
using guts = make_mixin<
    guts_impl,
    ringable,
    magic_frog_power,
    allocator_use_capable>;
```

Could make your own custom filter requirement:

```
template<typename T>
struct bells: T{
    void ring()
    {
        call_on(this, predicate<my_selector>, [](auto &a)
        {
            a.ring();
        }
    }
}
```

What does this look like in code? How do we implement?

Return of compose():

Takes all the types passed to compose as template arguments, then derives from some function call

```
template<typename... Ts>
class composition : public call<detail::make_base<composition<Ts...>>, Ts...>
{
    std:tuple<Ts...> data;
    //...
}
```

Basically what this metafunction is doing is taking some magical type called access, some magical type called protect

```
protect<assess<composition<Ts...>>>
```

the protect function takes one argument and derives protectedly from it:

```
template<typename T>
struct protect: protected T {};
```

Our composition is a chain of base classes:

```
interface3<<interface2<interface1<protect<assess<composition<Ts...>>>>>>
```

Composition <--- Interface 2 <--- Interface 1 <--- Protect <--- Access

If two interfaces have the same member function name, one will overshadow the other. Not perfect!

Interface mixins have to access the implementation mixins which are in the `data` tuple. Access has the types of the composition. It's the base class of that. So we are allowed to take our this pointer of access and cast it to the top level type. (You can cast the pointer of base to the pointer of derived if you know for sure that base is a base of that derived class)

For the base class to access the members of the derived class it has to be a friend:

```
template<typename... Ts>
class composition : public call<detail::make_base<composition<Ts...>>, Ts...>
{
    std:tuple<Ts...> data;
    friend access<composition<Ts...>>;
    //...
}
```


Why doesn't this break encapsulation? The pointer type of foreach is constrained to an access T. Only at the call site if you can access the protected base can you call the function.

```
template<typename T, typename A, typename L>
void for_each(access<T>* p, A, L l){
    auto& data = p->get_data();
    //magic here
}
```

What about internal interfaces that need to access each other? They don't know about each other, but they may need to access internal abilities

```
template<typename T>
struct bells: T{
    void ring()
    {
        for_each(this, ability<ringable>, [a = access_to(this)](auto& m))
        {
            m.ring(a);
        }
    }
}
```

access_to: free function which takes any pointer and casts it to the access, or constrains itself so that pointer conversion has to happen at that call site.

Conceptually, all of the implementation mixins are within the composition. So they can take the pointer to the composition and call for_each or other free functions on the pointer to access functions on the pointer as if it was their this pointer.

Same semantics whether from an interface class or mixin class

Doing something to guts, we pass it the access pointer and then it can keep going with the pointer, call execute on the allocator, etc.

What about initialization? Interactions between mixins on constructions? guts may want to allocate something before it's used, on initialization.

```
template<typename... Ts>
class composition : public call<detail::make_base<composition<Ts...>>, Ts...>
{
    //...
public:
    composition(std::tuple<Ts...> &&d) : data{std::move(d)} {
        for_each(this, ability<requires_init_and_destruct>,
            detail::call_init(this));
    }
    ~composition()
    {
        for_each(this,ability<requires_init_and_destruct>,
            detail::call_destruct(this));
    }
}
```

Two-phase initialization, non-optimal... Would also be nice if we could do it in order. Would be easy to implement but long compile-times.

Not as misbehaved as classic two-phase init. Classic problem: you have the ability to access a non-initialized object. In this case, the two-phase initialization is happening completly within the implmentation details of the compositoin. The user can't just go and access a partially initialized implemnetation mixin. (they can't access them directly anyway)

Can we build a fixed vector? (fixed capacity, variable size)

Interface mixins:
    * begin() end()
    * size()
    * Index operator
    * push_back()

Implementation mixins:
    * Fixed size buffer
    * Contiguous control block (3 pointers: begin, size, capacity)

Issues:
* Data footprint dependent on other mixins
* Debug builds are bloated - calls are inlined easier by the optimizer, but debug binaries are huge and slow.
    * Compile in release + debug symbols, and do not optimize what you want to debug
* Iterator validity contracts are hard (and other contracts)
* Testing is hard (so many compositional possibilities)
* Constructors are hard

Still trying to make it work.... Whoops.

We can add a new alias: make_dynamic_mixin. You don't apss a concrete type, you pass a metaclosure (factory, where you give it the types of all the other htings in the composition) and it will spit out the type the implementation mixin will ahve in that context

```
using guts = make_dynamic_mixin<
    fixed_buffer_factory,
    allocator>;
```

Then for compose, you don't sotre the tuple Ts of data, you store a tuple "I call every Ts with all the other Ts"

```
template<typename... Ts>
class composition : public call<detail::make_base<composition<Ts...>>, Ts...>
{
    std:tuple<call<Ts, Ts...>...> data;
}
```

(He's lying to us: his implementation does not actually use a tuple)

```
auto myUart = make_uart(interface<blocking_tx>, uart1, 9600_baud, rx = 0.9_pin, tx = 0.10_pin);

myuart.blocking_send("hello world");
```

He did drivers that did this using policy based class design, but they were not maintainable even if 10x more efficient

Composition:
blocking_tx interface

implementation:

```
uart1, baud_t<uint<9600>>, rx_pin_t<pin<0,9>>, tx_pin_t<pin<0,10>>
```
