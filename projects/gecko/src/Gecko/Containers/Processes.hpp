#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Process.hpp"

namespace Gecko
{
    class Processes :
        public Serializable
    {
    public:
        // From Serializable.
        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        typedef std::map<std::string, Process> Map;

    public:
        void update(float time, Id id, const Ogre::Vector3& position, std::shared_ptr<Resources> resources);

    public:
        void clear()
        {
            m_items.clear();
        }

        auto empty() const
        {
            return m_items.empty();
        }

        auto size() const
        {
            return m_items.size();
        }

    public:
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

        auto cend()
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
