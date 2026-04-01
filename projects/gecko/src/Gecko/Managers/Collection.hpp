#pragma once

#include "Gecko/Managers/CollectionBase.hpp"

namespace Gecko
{
    template<typename BaseType, typename Type>
    class Collection :
        public CollectionBase<BaseType>
    {
    private:
        using Allocator = std::allocator<Type>;
        using Traits    = std::allocator_traits<Allocator>;

    public:
        Collection(std::function<BaseType* (Type*)> factory) :
            m_factory(factory)
        {
        }

    public:
        bool allocate(std::uint64_t max_size) override
        {
            if (is_allocated())
            {
                return false;
            }

            if (max_size <= 0)
            {
                return false;
            }

            // Set size.
            m_max_size = max_size;

            // Allocate memory.
            m_pointer = Traits::allocate(m_allocator, m_max_size);

            // Allocate elements.
            for (std::uint64_t i = 0; i < m_max_size; i++)
            {
                m_factory(m_pointer + i);
                m_elements.emplace_back(m_pointer + i);
            }

            return true;
        }

        void deallocate() override
        {
            if (is_allocated() == false)
            {
                return;
            }

            // Destroy elements.
            destroy_all();

            // Deallocate elements.
            for (std::uint64_t i = 0; i < m_max_size; i++)
            {
                Traits::destroy(m_allocator, m_pointer + i);
            }

            // Deallocate memory.
            Traits::deallocate(m_allocator, m_pointer, m_max_size);
            m_pointer = nullptr;

            // Set size.
            m_max_size = 0;
        }

        BaseType* create() override
        {
            if (is_full())
            {
                return nullptr;
            }

            return m_elements[m_size++];
        }

        void destroy(BaseType* element) override
        {
            auto element_position = std::ranges::find(m_elements, element);

            if (element_position != m_elements.end())
            {
                std::uint64_t index_1 = element_position - m_elements.begin();
                std::uint64_t index_2 = --m_size;

                std::swap(m_elements[index_1], m_elements[index_2]);
            }
        }

        void destroy_all() override
        {
            m_size = 0;
        }

        BaseType* at(std::uint64_t index) override
        {
            if (index >= m_max_size)
            {
                return nullptr;
            }

            return m_elements[index];
        }

        void iterate(std::function<void(BaseType& element)> functor) override
        {
            for (std::uint64_t i = 0; i < m_size; i++)
            {
                functor(*(m_elements[i]));
            }
        }

        void iterate_all(std::function<void(BaseType& element)> functor) override
        {
            for (std::uint64_t i = 0; i < m_max_size; i++)
            {
                functor(*(m_elements[i]));
            }
        }

    public:
        std::uint64_t get_size() const override
        {
            return m_size;
        }

        std::uint64_t get_max_size() const override
        {
            return m_max_size;
        }

    public:
        bool is_allocated() const override
        {
            return m_pointer != nullptr;
        }

        bool is_empty() const override
        {
            return m_size == 0;
        }

        bool is_full() const override
        {
            return m_size == m_max_size;
        }

    private:
        std::function<BaseType* (Type*)> m_factory;
        Allocator                        m_allocator;

        Type* m_pointer = nullptr;

        std::vector<Type*> m_elements;

        std::uint64_t m_size     = 0;
        std::uint64_t m_max_size = 0;
    };
}
