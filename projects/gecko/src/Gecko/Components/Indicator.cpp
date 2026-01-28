#include "Gecko/Components/Indicator.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Maps/Map.hpp"

namespace Gecko
{
    void Indicator::set_material_name(const std::string& material_name)
    {
        m_entity->setMaterialName(material_name);
    }

    void Indicator::set_position(const Ogre::Vector3& position)
    {
        m_scene_node->setPosition(position);
    }

    Cone::Cone()
    {
        m_entity = Game::getSingleton().create_entity("cone.mesh");
        m_entity->setCastShadows(false);

        m_scene_node = Game::getSingleton().create_scene_node();
        m_scene_node->attachObject(m_entity);
    }

    Cone::~Cone()
    {
        Game::getSingleton().destroy_scene_node(m_scene_node);
        Game::getSingleton().destroy_entity(m_entity);
    }
}
