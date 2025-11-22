#pragma once

#include "Id.hpp"
#include "Managers/Manager.hpp"

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
