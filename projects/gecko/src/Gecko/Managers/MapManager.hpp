#pragma once

#include "Gecko/Id.hpp"
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
        void update(float time) override;
    };
}
