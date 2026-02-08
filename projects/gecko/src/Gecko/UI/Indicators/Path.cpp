#include "Gecko/UI/Indicators/Path.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    Path::Path()
    {
        m_entity = Game::getSingleton().create_entity("cone.mesh");
        m_entity->setCastShadows(false);

        m_manual_object = Game::getSingleton().create_manual_object();
        m_manual_object->setCastShadows(false);

        m_scene_node = Game::getSingleton().create_scene_node();
        // m_scene_node->attachObject(m_entity);
        m_scene_node->attachObject(m_manual_object);
        // m_scene_node->setScale(Ogre::Vector3::UNIT_SCALE * 0.01f);
    }

    Path::~Path()
    {
        Game::getSingleton().destroy_scene_node(m_scene_node);
        Game::getSingleton().destroy_entity(m_entity);
        Game::getSingleton().destroy_manual_object(m_manual_object);
    }

    void Path::set_points(const std::string& material_name, const Navigation::Path::Points& points)
    {
        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (LayerPtr layer = map->get_layer(Settings::Layer::Terrain))
            {
                m_manual_object->begin(material_name, Ogre::RenderOperation::OperationType::OT_LINE_STRIP);

                for (const auto& point : points)
                {
                    m_manual_object->position(
                        point.x * layer->get_scale().x + layer->get_scale().x * 0.5f, 4.0f,
                        point.z * layer->get_scale().z + layer->get_scale().z * 0.5f
                    );
                }

                m_manual_object->end();
            }
        }
    }
}
