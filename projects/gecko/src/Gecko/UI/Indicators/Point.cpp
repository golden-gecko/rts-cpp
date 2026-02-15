#include "Gecko/UI/Indicators/Point.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Scenes/Scene.hpp"

namespace Gecko
{
    Point::Point()
    {
        m_entity = Game::getSingleton().get_active_map()->get_scene()->create_entity("cone.mesh");
        m_entity->setCastShadows(false);

        m_scene_node = Game::getSingleton().get_active_map()->get_scene()->create_scene_node();
        m_scene_node->attachObject(m_entity);
    }

    Point::~Point()
    {
        Game::getSingleton().get_active_map()->get_scene()->destroy_scene_node(m_scene_node);
        Game::getSingleton().get_active_map()->get_scene()->destroy_entity(m_entity);
    }
}
