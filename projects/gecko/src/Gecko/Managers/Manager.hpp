#pragma once

#include "Gecko/Managers/Collection.hpp"

namespace Gecko
{
    template<typename BaseType, typename TypeName, typename TypeId>
    class Manager
    {
    public:
        using Collections = std::map<TypeName, std::unique_ptr<CollectionBase<BaseType>>>;
        using Items       = std::map<TypeId, BaseType*>;

    public:
        virtual ~Manager()
        {
            unregister_all();
        }

    public:
        template<typename Type>
        bool register_type(const TypeName& name, std::function<BaseType* (Type*)> factory)
        {
            if (is_type_registered(name))
            {
                return false;
            }

            m_collections.emplace(name, std::make_unique<Collection<BaseType, Type>>(factory));

            return true;
        }

        bool allocate(const TypeName& name, std::uint64_t max_size)
        {
            if (is_type_registered(name) == false)
            {
                return false;
            }

            return m_collections.at(name)->allocate(max_size);
        }

        BaseType* at(const TypeName& name, std::uint64_t index)
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

            BaseType* element = m_collections.at(name)->create();

            if (element == nullptr)
            {
                return nullptr;
            }

            m_last_id++;

            element->set_id(m_last_id);
            // element->init(); TODO: ???

            m_items.emplace(m_last_id, element);

            return element;
        }

        void deallocate_all()
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

            m_items.erase(element->get_id());

            // element->deinit(); TODO: ???
            element->set_id(TypeId());

            for (const auto& [_, collection] : m_collections)
            {
                collection->destroy(element);
            }
        }

        void destroy(const TypeId& id)
        {
            destroy(get(id));
        }

        void destroy_all()
        {
            m_last_id = 0;

            m_items.clear();

            for (const auto& [_, collection] : m_collections)
            {
                collection->destroy_all();
            }
        }

        void iterate(std::function<void(BaseType& element)> functor)
        {
            for (const auto& [_, collection] : m_collections)
            {
                collection->iterate(functor);
            }
        }

        void iterate_all(std::function<void(BaseType& element)> functor)
        {
            for (const auto& [_, collection] : m_collections)
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

        std::uint64_t get_size(const TypeName& name) const
        {
            if (is_type_registered(name) == false)
            {
                return 0;
            }

            return m_collections.at(name)->get_size();
        }

        std::uint64_t get_max_size(const TypeName& name) const
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
            deallocate_all();

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
