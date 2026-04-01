#pragma once

namespace Gecko
{
    template <typename BaseType>
    class CollectionBase
    {
    public:
        virtual ~CollectionBase() = default;

    public:
        virtual bool allocate(std::uint64_t max_size) = 0;
        virtual void deallocate() = 0;

        virtual BaseType* create() = 0;
        virtual void destroy(BaseType* element) = 0;
        virtual void destroy_all() = 0;

        virtual BaseType* at(std::uint64_t index) = 0;

        virtual void iterate(std::function<void(BaseType& element)> functor) = 0;
        virtual void iterate_all(std::function<void(BaseType& element)> functor) = 0;

    public:
        virtual std::uint64_t get_size() const = 0;
        virtual std::uint64_t get_max_size() const = 0;

    public:
        virtual bool is_allocated() const = 0;
        virtual bool is_empty() const = 0;
        virtual bool is_full() const = 0;
    };
}
