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
        static Factory* create(Factory* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Factory() = default;
        explicit Factory(const Factory& other) = default;

    public:
        const std::map<std::string, float>& get_usage() const
        {
            return usage;
        }

    private:
        std::set<std::string> configurations;
        Ogre::Vector3 rally_point = Ogre::Vector3::ZERO;
        Id created_objects;
        float spent_resources = 0.0f;
        std::map<std::string, float> usage;
    };
}
