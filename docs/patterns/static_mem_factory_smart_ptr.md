# Pattern: Static Memory Allocation Factory + Smart Pointer

TBD

## Diagrams

TBD

## Context

TBD

## Problem

There are many times where we want to replace dynamic memory allocation with static memory.

## Solution

Not using dynamic memory allocation means it would need to be preallocated. 

## Consequences

TBD

## Known Uses

TBD

## Variants

TBD

## Related Patterns

TBD

## References

TBD

## Notes


## Sample Code

```
#include <type_traits>
#include <array>
#include <algorithm>

void print(const char*);

template<typename Base, 
         std::size_t NumberOfElements,
         std::size_t ChunkSize = sizeof(Base) /* increase if child might be bigger*/,
         std::size_t ChunkAlign = alignof(Base)>
class factory
{
    static_assert(std::has_virtual_destructor<Base>::value);

    struct _chunk 
    {
        std::aligned_storage_t<ChunkSize, ChunkAlign> memory;
        bool used = false;
    };
    
    std::array<_chunk, NumberOfElements> _data{};

public:
    factory() = default;
    factory(const factory& ) = delete;
    factory& operator=(const factory& ) = delete;

    template<typename T, typename ...Args>
    T* allocate(Args&& ... args)
    {
        auto itr = std::find_if(_data.begin(), _data.end(), [](auto & p){return !p.used;});
        if (itr == _data.end())
            return nullptr;

        auto res = new(&itr->memory) T(std::forward<Args>(args)...);
        itr->used = res != nullptr; //because the ctor might throw
        return res;
    }

    void deallocate(Base* ptr)
    {
        auto itr = std::find_if(_data.begin(), _data.end(), 
                                [&](auto & p){return p.used && (reinterpret_cast<void*>(&p.memory) == ptr);});
        
        if (itr != _data.end())
        {
            ptr->~Base();
            itr->used = false;
        }
    }
};
```


#### Function showing use

```
void test()
{
    struct base
    {
        virtual void foo() = 0;
        virtual ~base() {}
    };

    struct child : base
    {
        virtual void foo() override {print("foo");} //dummy, so it shows up in the disassembly
    };

    factory<base, 4> f;

    base* p = f.allocate<child>();
    p->foo();

    f.deallocate(p);
}
```
