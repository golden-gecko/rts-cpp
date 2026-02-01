#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Managers/Manager.hpp"

namespace Gecko
{
    class ObjectManager :
        public Ogre::Singleton<ObjectManager>,
        public Manager<Object, std::string, Id>,
        public Updatable
    {
    private:
        using base_type = Manager<Object, std::string, Id>;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        using ObjectsInRange = std::vector<std::pair<Object*, float>>;

    public:
        void init(const ConfigurationPtr& configuration);

        Object* create(const std::string& name);

    public:
        ObjectsInRange get_in_range(const Ogre::Vector3& position, float range = std::numeric_limits<float>::max());
    };
}
