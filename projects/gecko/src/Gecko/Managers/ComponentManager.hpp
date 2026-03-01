#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Managers/Manager.hpp"

namespace Gecko
{
    class ComponentManager :
        public Ogre::Singleton<ComponentManager>,
        public Manager<Component, std::string, Id>
    {
    private:
        using base_type = Manager<Component, std::string, Id>;

    public:
        // From Initializable.
        void init();
        void deinit();
    
    public:
        ComponentManager(const ConfigurationPtr& configuration, const ScenePtr& scene);

    private:
        ConfigurationPtr m_configuration;
        ScenePtr m_scene;
    };
}
