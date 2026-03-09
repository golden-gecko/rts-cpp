#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
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
        // From Initializable.
        void init();
        void deinit();

    public:
        // From Updatable.
        void update(float time) override;

    public:
        using ObjectsInRange = std::vector<std::pair<Object*, float>>;

    public:
        ObjectManager(const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        Object* create(const std::string& name);
        ObjectsInRange get_in_range(const Ogre::Vector3& position, float range = std::numeric_limits<float>::max(), const std::vector<Id>& exclude = {});

    private:
        ConfigurationPtr m_configuration;
        ScenePtr m_scene;
    };
}
