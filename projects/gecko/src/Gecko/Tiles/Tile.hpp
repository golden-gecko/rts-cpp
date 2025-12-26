#pragma once

#include "Gecko/Interfaces/Initializable.hpp"

namespace Gecko
{
    class Tile : public Initializable
    {
    public:
        void set_owner(Layer* owner)
        {
            m_owner = owner;
        }

    protected:
        Layer* m_owner = nullptr;

        Ogre::Entity* m_entity = nullptr;
        Ogre::SceneNode* m_scene_node = nullptr;
    };
}
