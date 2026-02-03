#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    // TODO: Refactor to serialize from map instead of array, to allow overriding.
    class Skills :
        public Serializable
    {
    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        typedef std::vector<Skill*> Items;

    public:
        explicit Skills() = default;
        explicit Skills(const Skills& other);

    public:
        void activate(const std::string& name, const Id& id);
        void activate(const std::string& name, const Ogre::Vector3& position);

        void clear()
        {
            m_items.clear();
        }

        auto empty() const
        {
            return m_items.empty();
        }

        template<typename Type>
        bool has() const
        {
            for (const auto& i : m_items)
            {
                if (dynamic_cast<Type*>(i) != nullptr)
                {
                    return true;
                }
            }

            return false;
        }

        bool is_available(const std::string& name) const;

        auto size() const
        {
            return m_items.size();
        }

    public:
        auto begin()
        {
            return m_items.begin();
        }

        auto end()
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

    public:
        bool operator==(const Skills& other) const
        {
            return m_items == other.m_items;
        }

    private:
        Items m_items;
    };
}
