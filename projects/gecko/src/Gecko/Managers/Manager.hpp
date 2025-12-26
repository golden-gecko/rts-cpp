#pragma once

namespace Gecko
{
    template <typename BaseType>
    class CollectionBase
    {
    public:
        virtual bool allocate(int _max_size) = 0;
        virtual BaseType* at(int index) = 0;
        virtual BaseType* create() = 0;
        virtual void deallocate() = 0;
        virtual void destroy(BaseType* element) = 0;
        virtual void destroy_all() = 0;
        virtual void iterate(std::function<void(BaseType& element)> functor) = 0;
        virtual void iterate_all(std::function<void(BaseType& element)> functor) = 0;

        virtual int get_size() const = 0;
        virtual int get_max_size() const = 0;

        virtual bool is_allocated() const = 0;
        virtual bool is_empty() const = 0;
        virtual bool is_full() const = 0;
    };

    template<typename BaseType, typename Type>
    class Collection : public CollectionBase<BaseType>
    {
    public:
        explicit Collection(std::function<BaseType* (Type*)> factory) :
            m_factory(factory)
        {
        }

        ~Collection()
        {
            // TODO: Is this correct?
            deallocate();
        }

        bool allocate(int max_size) override
        {
            if (is_allocated())
            {
                return false;
            }

            if (max_size == 0)
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
            for (int i = 1; i < m_max_size; ++i)
            {
                new (m_pointer + i) Type(*m_pointer);
                m_elements.emplace_back(m_pointer + i);
            }

            return true;
        }

        BaseType* at(int index) override
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

            for (int i = 0; i < m_max_size; ++i)
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
                auto index_1 = element_position - m_elements.begin();
                auto index_2 = --m_size;

                std::swap(m_elements[index_1], m_elements[index_2]);
            }
        }

        void destroy_all() override
        {
            m_size = 0;
        }

        void iterate(std::function<void(BaseType& element)> functor) override
        {
            for (int i = 0; i < m_size; ++i)
            {
                functor(*(m_elements[i]));
            }
        }

        void iterate_all(std::function<void(BaseType& element)> functor) override
        {
            for (int i = 0; i < m_max_size; ++i)
            {
                functor(*(m_elements[i]));
            }
        }

        int get_size() const override
        {
            return m_size;
        }

        int get_max_size() const override
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

        int m_size = 0;
        int m_max_size = 0;
    };

    template<typename BaseType, typename TypeName, typename TypeId>
    class Manager
    {
    public:
        typedef std::map<TypeName, std::shared_ptr<CollectionBase<BaseType>>> Collections;
        typedef std::map<TypeId, BaseType*> Items;

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

        bool allocate(const TypeName& name, int max_size)
        {
            if (is_type_registered(name) == false)
            {
                return false;
            }

            return m_collections.at(name)->allocate(max_size);
        }

        BaseType* at(const TypeName& name, int index)
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

        int get_size(const TypeName& name) const
        {
            if (is_type_registered(name) == false)
            {
                return 0;
            }

            return m_collections.at(name)->get_size();
        }

        int get_max_size(const TypeName& name) const
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

        auto begin() const
        {
            return m_items.begin();
        }

        auto end() const
        {
            return m_items.end();
        }

        auto cbegin() const
        {
            return m_items.cbegin();
        }

        auto cend() const
        {
            return m_items.cend();
        }

        auto size() const
        {
            return m_items.size();
        }

    protected:
        Collections m_collections;
        Items m_items;
        TypeId m_last_id = 0;
    };
}
