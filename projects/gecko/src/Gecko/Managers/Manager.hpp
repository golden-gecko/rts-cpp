#pragma once

namespace Gecko
{
    template <typename BaseType>
    class CollectionBase
    {
    public:
        using Size = std::uint64_t;

    public:
        virtual bool allocate(Size _max_size) = 0;
        virtual BaseType* at(Size index) = 0;
        virtual BaseType* create() = 0;
        virtual void deallocate() = 0;
        virtual void destroy(BaseType* element) = 0;
        virtual void destroy_all() = 0;
        virtual void iterate(std::function<void(BaseType& element)> functor) = 0;
        virtual void iterate_all(std::function<void(BaseType& element)> functor) = 0;

        virtual Size get_size() const = 0;
        virtual Size get_max_size() const = 0;

        virtual bool is_allocated() const = 0;
        virtual bool is_empty() const = 0;
        virtual bool is_full() const = 0;
    };

    template<typename BaseType, typename Type>
    class Collection :
        public CollectionBase<BaseType>
    {
    public:
        using Size = CollectionBase<BaseType>::Size;

    public:
        explicit Collection(std::function<BaseType* (Type*)> factory) :
            m_factory(factory)
        {
        }

        ~Collection()
        {
        }

        bool allocate(Size max_size) override
        {
            if (is_allocated())
            {
                return false;
            }

            if (max_size <= 0)
            {
                return false;
            }

            m_max_size = max_size;

            m_pointer = m_memory.allocate(m_max_size);
            m_elements.reserve(m_max_size);

            // Create first element.
            m_factory(m_pointer);
            m_elements.emplace_back(m_pointer);

            // Create elements by copying the first one.
            for (Size i = 1; i < m_max_size; i++)
            {
                new (m_pointer + i) Type(*m_pointer);
                m_elements.emplace_back(m_pointer + i);
            }

            return true;
        }

        BaseType* at(Size index) override
        {
            if (index >= m_max_size)
            {
                return nullptr;
            }

            return m_elements[index];
        }

        BaseType* create() override
        {
            if (is_full())
            {
                return nullptr;
            }

            return m_elements[m_size++];
        }

        void deallocate() override
        {
            destroy_all();

            if (is_allocated() == false)
            {
                return;
            }

            for (Size i = 0; i < m_max_size; i++)
            {
                // TODO: Fix.
                // m_memory.destroy(m_pointer + i);
            }

            m_memory.deallocate(m_pointer, m_max_size);
            m_pointer = nullptr;

            m_max_size = 0;
        }

        void destroy(BaseType* element) override
        {
            auto element_position = std::ranges::find(m_elements, element);

            if (element_position != m_elements.end())
            {
                Size index_1 = element_position - m_elements.begin();
                Size index_2 = --m_size;

                std::swap(m_elements[index_1], m_elements[index_2]);
            }
        }

        void destroy_all() override
        {
            m_size = 0;
        }

        void iterate(std::function<void(BaseType& element)> functor) override
        {
            for (Size i = 0; i < m_size; i++)
            {
                functor(*(m_elements[i]));
            }
        }

        void iterate_all(std::function<void(BaseType& element)> functor) override
        {
            for (Size i = 0; i < m_max_size; i++)
            {
                functor(*(m_elements[i]));
            }
        }

        Size get_size() const override
        {
            return m_size;
        }

        Size get_max_size() const override
        {
            return m_max_size;
        }

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
        std::allocator<Type> m_memory;

        Type* m_pointer = nullptr;

        std::vector<Type*> m_elements;

        Size m_size = 0;
        Size m_max_size = 0;
    };

    template<typename BaseType, typename TypeName, typename TypeId>
    class Manager
    {
    public:
        using Size = CollectionBase<BaseType>::Size;
        using Collections = std::map<TypeName, std::shared_ptr<CollectionBase<BaseType>>>;
        using Items = std::map<TypeId, BaseType*>;

    public:
        template<typename Type>
        bool register_type(const TypeName& name, std::function<BaseType* (Type*)> factory)
        {
            if (is_type_registered(name))
            {
                return false;
            }

            // TODO: Find out why std::unique_ptr causes memory leak.
            m_collections.emplace(name, std::make_shared<Collection<BaseType, Type>>(factory));

            return true;
        }

        bool allocate(const TypeName& name, Size max_size)
        {
            if (is_type_registered(name) == false)
            {
                return false;
            }

            return m_collections.at(name)->allocate(max_size);
        }

        BaseType* at(const TypeName& name, CollectionBase<BaseType>::Size index)
        {
            if (is_type_registered(name) == false)
            {
                return nullptr;
            }

            return m_collections.at(name)->at(index);
        }

        BaseType* create(const TypeName& name)
        {
            if (is_type_registered(name) == false)
            {
                return nullptr;
            }

            auto element = m_collections.at(name)->create();

            if (element == nullptr)
            {
                return nullptr;
            }

            // TODO: Replace with init method.
            element->set_id(++m_last_id);

            m_items.emplace(m_last_id, element);

            return element;
        }

        void deallocate()
        {
            destroy_all();

            for (const auto& [name, collection] : m_collections)
            {
                collection->deallocate();
            }
        }

        void destroy(BaseType* element)
        {
            if (element == nullptr)
            {
                return;
            }

            element->deinit();

            for (const auto& [name, collection] : m_collections)
            {
                collection->destroy(element);
            }

            m_items.erase(element->get_id());
        }

        void destroy(const TypeId& id)
        {
            destroy(get(id));
        }

        void destroy_all()
        {
            for (const auto& [name, collection] : m_collections)
            {
                collection->destroy_all();
            }

            m_items.clear();

            m_last_id = 0;
        }

        void iterate(std::function<void(BaseType& element)> functor)
        {
            for (const auto& [name, collection] : m_collections)
            {
                collection->iterate(functor);
            }
        }

        void iterate_all(std::function<void(BaseType& element)> functor)
        {
            for (const auto& [name, collection] : m_collections)
            {
                collection->iterate_all(functor);
            }
        }

        BaseType* get(const TypeId& id) const
        {
            auto element = m_items.find(id);

            if (element == m_items.end())
            {
                return nullptr;
            }

            return element->second;
        }

        Size get_size(const TypeName& name) const
        {
            if (is_type_registered(name) == false)
            {
                return 0;
            }

            return m_collections.at(name)->get_size();
        }

        Size get_max_size(const TypeName& name) const
        {
            if (is_type_registered(name) == false)
            {
                return 0;
            }

            return m_collections.at(name)->get_max_size();
        }

        bool is_type_registered(const TypeName& name) const
        {
            return m_collections.find(name) != m_collections.end();
        }

        void unregister_all()
        {
            deallocate();

            m_collections.clear();
        }

        Items::iterator begin()
        {
            return m_items.begin();
        }

        Items::iterator end()
        {
            return m_items.end();
        }

        Items::const_iterator cbegin() const
        {
            return m_items.cbegin();
        }

        Items::const_iterator cend() const
        {
            return m_items.cend();
        }

        Items::size_type size() const
        {
            return m_items.size();
        }

    private:
        Collections m_collections;
        Items       m_items;
        TypeId      m_last_id;
    };
}
