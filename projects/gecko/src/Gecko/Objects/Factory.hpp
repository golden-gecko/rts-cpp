#pragma once

#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    class Factory :
        public Object
    {
    private:
        using base_type = Object;

    public:
        static Factory* create(Factory* memory, const ConfigurationPtr& configuration);

    public:
        explicit Factory() = default;
        explicit Factory(const Factory& other) = default;

    public:
        const std::map<std::string, float>& get_usage() const
        {
            return m_usage;
        }

    private:
        std::set<std::string> m_configurations;
        Ogre::Vector3 m_rally_point = Ogre::Vector3::ZERO;
        Id m_created_objects;
        float m_spent_resources = 0.0f;
        std::map<std::string, float> m_usage;
    };
}
