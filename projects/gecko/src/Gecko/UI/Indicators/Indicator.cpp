#include "Gecko/UI/Indicators/Indicator.hpp"

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
}
