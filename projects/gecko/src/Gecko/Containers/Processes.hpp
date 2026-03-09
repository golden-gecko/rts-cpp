#pragma once

#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Process.hpp"

namespace Gecko
{
    class Processes :
        public Serializable
    {
    public:
        using Map = std::map<std::string, Process>;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        void update(float time, const Id& id, const Ogre::Vector3& position, const ResourcesPtr& resources);

    public:
        void clear()
        {
            m_items.clear();
        }

        bool empty() const
        {
            return m_items.empty();
        }

        Map::size_type size() const
        {
            return m_items.size();
        }

    public:
        Map::iterator begin()
        {
            return m_items.begin();
        }

        Map::iterator end()
        {
            return m_items.end();
        }

        Map::const_iterator cbegin() const
        {
            return m_items.cbegin();
        }

        Map::const_iterator cend() const
        {
            return m_items.cend();
        }

    public:
        bool operator==(const Processes& other) const
        {
            return m_items == other.m_items;
        }

    private:
        Map m_items;
    };
}
