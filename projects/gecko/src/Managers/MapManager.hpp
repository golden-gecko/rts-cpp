#pragma once

#include "Id.hpp"
#include "Interfaces/Updatable.hpp"
#include "Managers/Manager.hpp"

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
