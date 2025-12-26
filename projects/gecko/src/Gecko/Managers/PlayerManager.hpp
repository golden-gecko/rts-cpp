#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Managers/Manager.hpp"

namespace Gecko
{
    class PlayerManager :
        public Ogre::Singleton<PlayerManager>,
        public Manager<Player, std::string, Id>,
        public Updatable
    {
    private:
        using base_type = Manager<Player, std::string, Id>;

    public:
        void update(float time) override;
    };
}
