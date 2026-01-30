#include "Gecko/UI/Indicators/Point.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Maps/Map.hpp"

namespace Gecko
{
    Point::Point()
    {
        m_entity = Game::getSingleton().create_entity("cone.mesh");
        m_entity->setCastShadows(false);

        m_scene_node = Game::getSingleton().create_scene_node();
        m_scene_node->attachObject(m_entity);
        // m_scene_node->setScale(Ogre::Vector3::UNIT_SCALE * 0.01f);
    }

    Point::~Point()
    {
        Game::getSingleton().destroy_scene_node(m_scene_node);
        Game::getSingleton().destroy_entity(m_entity);
    }
}
