#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Managers/Manager.hpp"

namespace Gecko
{
    class ComponentManager :
        public Ogre::Singleton<ComponentManager>,
        public Manager<Component, std::string, Id>
    {
    private:
        using base_type = Manager<Component, std::string, Id>;
    };
}
