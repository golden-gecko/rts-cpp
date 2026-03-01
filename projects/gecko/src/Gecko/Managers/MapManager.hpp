#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Managers/Manager.hpp"

namespace Gecko
{
    class MapManager :
        public Ogre::Singleton<MapManager>,
        public Manager<Map, std::string, Id>,
        public Updatable
    {
    private:
        using base_type = Manager<Map, std::string, Id>;

    public:
        // From Initializable.
        void init();
        void deinit();

    public:
        // From Updatable.
        void update(float time) override;

    public:
        MapManager(const ConfigurationPtr& configuration, const ScenePtr& scene);

    private:
        ConfigurationPtr m_configuration;
        ScenePtr m_scene;
    };
}
