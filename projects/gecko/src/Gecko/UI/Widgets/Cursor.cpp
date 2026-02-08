#include "Gecko/UI/Widgets/Cursor.hpp"

#include "Gecko/Components/Component.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Objects/Factory.hpp"
#include "Gecko/Objects/Missile.hpp"
#include "Gecko/Objects/Vehicle.hpp"
#include "Gecko/QueryFlags.hpp"

namespace Gecko
{
    void CursorWidget::ProcessEvent(Rml::Event& event)
    {
    }

    CursorWidget::CursorWidget()
    {
        m_square = Game::getSingleton().create_manual_object();
        m_square->setQueryFlags(QueryFlags::QF_Cursor);

        m_scene_node = Game::getSingleton().create_scene_node();
        m_scene_node->attachObject(m_square);
        m_scene_node->setPosition(Settings::UI::CursorOffset);
    }

    CursorWidget::~CursorWidget()
    {
        Game::getSingleton().destroy_scene_node(m_scene_node);
        Game::getSingleton().destroy_manual_object(m_square);
    }

    void CursorWidget::set_mesh(const ConfigurationPtr& configuration)
    {
        set_visible(false);

        // Save configuration name.
        m_mesh_name = configuration->get_name();

        // Find or create component and object.
        auto object = m_objects.find(m_mesh_name);

        if (object == m_objects.end())
        {
            // TODO: Create object factory.
            auto type = configuration->get_string("type");

            if (type == "Factory")
            {
                auto object = std::make_unique<Factory>();

                object->deserialize(configuration);
                object->set_visible(true);
                object->init();

                m_objects.emplace(std::make_pair(configuration->get_name(), std::move(object)));
            }
            else if (type == "Missile")
            {
                auto object = std::make_unique<Missile>();

                object->deserialize(configuration);
                object->set_visible(true);
                object->init();

                m_objects.emplace(std::make_pair(configuration->get_name(), std::move(object)));
            }
            else if (type == "Object")
            {
                auto object = std::make_unique<Object>();

                object->deserialize(configuration);
                object->set_visible(true);
                object->init();

                m_objects.emplace(std::make_pair(configuration->get_name(), std::move(object)));
            }
            else if (type == "Vehicle")
            {
                auto object = std::make_unique<Vehicle>();

                object->deserialize(configuration);
                object->set_visible(true);
                object->init();

                m_objects.emplace(std::make_pair(configuration->get_name(), std::move(object)));
            }
            else
            {
                L_WARNING << "Object type '" + type + "' not found.";
            }
        }
        else
        {
            object->second->set_visible(true);
        }
    }

    void CursorWidget::set_type(Type type)
    {
        set_visible(false);

        // Save type.
        m_type = type;

        // Show cursor.
        if (type == Type::Component)
        {
            auto component = m_components.find(m_mesh_name);

            if (component != m_components.end())
            {
                component->second->set_visible(true);
            }
        }
        else if (type == Type::Object)
        {
            auto object = m_objects.find(m_mesh_name);

            if (object != m_objects.end())
            {
                object->second->set_visible(true);
            }
        }
        else if (type == Type::Square)
        {
            m_scene_node->setVisible(true);
        }
        else
        {
            L_WARNING << "Invalid cursor type.";
        }
    }

    void CursorWidget::set_visible(bool visible)
    {
        // Hide all.
        m_scene_node->setVisible(false);

        for (const auto& [configuration_name, component] : m_components)
        {
            component->set_visible(false);
        }

        for (const auto& [configuration_name, object] : m_objects)
        {
            object->set_visible(false);
        }

        // Show cursor.
        if (visible)
        {
            if (m_type == Type::Component)
            {
                auto component = m_components.find(m_mesh_name);

                if (component != m_components.end())
                {
                    component->second->set_visible(true);
                }
            }
            else if (m_type == Type::Object)
            {
                auto object = m_objects.find(m_mesh_name);

                if (object != m_objects.end())
                {
                    object->second->set_visible(true);
                }
            }
            else if (m_type == Type::Square)
            {
                m_scene_node->setVisible(true);
            }
            else
            {
                L_WARNING << "Invalid cursor type.";
            }
        }
    }

    void CursorWidget::update(const std::pair<LayerPtr, Ogre::Vector3>& cast)
    {
        if (cast.first == nullptr)
        {
            return;
        }

        m_position = cast.first->get_index(cast.second);

        if (m_type == Type::Component)
        {
            auto component = m_components.find(m_mesh_name);

            if (component != m_components.end())
            {
                component->second->set_position(cast.second);
            }
        }
        else if (m_type == Type::Object)
        {
            auto object = m_objects.find(m_mesh_name);

            if (object != m_objects.end())
            {
                object->second->set_position(cast.second);
            }
        }
        else if (m_type == Type::Square)
        {
            // Get node vertices.
            auto index_x = static_cast<Index>(std::floorf(cast.second.x / cast.first->get_scale().x));
            auto index_z = static_cast<Index>(std::floorf(cast.second.z / cast.first->get_scale().z));

            auto size = cast.first->get_size() + 1;

            // Get grid vertices.
            const auto& grid_scale = cast.first->get_grid_scale();

            auto g00 = Ogre::Vector3( m_position.x      * grid_scale.x, 0.0f,  m_position.z      * grid_scale.z);
            auto g01 = Ogre::Vector3( m_position.x      * grid_scale.x, 0.0f, (m_position.z + 1) * grid_scale.z);
            auto g11 = Ogre::Vector3((m_position.x + 1) * grid_scale.x, 0.0f, (m_position.z + 1) * grid_scale.z);
            auto g10 = Ogre::Vector3((m_position.x + 1) * grid_scale.x, 0.0f,  m_position.z      * grid_scale.z);

            // Get Y position of grid vertices.
            g00 = cast.first->get_position(g00.x, g00.z, false);
            g01 = cast.first->get_position(g01.x, g01.z, false);
            g11 = cast.first->get_position(g11.x, g11.z, false);
            g10 = cast.first->get_position(g10.x, g10.z, false);

            // Create cursor.
            m_square->clear();
            m_square->begin("cursor_grid_red"); // TODO: Hardcoded.

            m_square->position(g00);
            m_square->normal(Ogre::Vector3::UNIT_Y);
            m_square->textureCoord(0.0f, 0.0f);

            m_square->position(g01);
            m_square->normal(Ogre::Vector3::UNIT_Y);
            m_square->textureCoord(1.0f, 0.0f);

            m_square->position(g10);
            m_square->normal(Ogre::Vector3::UNIT_Y);
            m_square->textureCoord(0.0f, 1.0f);

            m_square->position(g10);
            m_square->normal(Ogre::Vector3::UNIT_Y);
            m_square->textureCoord(0.0f, 1.0f);

            m_square->position(g01);
            m_square->normal(Ogre::Vector3::UNIT_Y);
            m_square->textureCoord(1.0f, 0.0f);

            m_square->position(g11);
            m_square->normal(Ogre::Vector3::UNIT_Y);
            m_square->textureCoord(1.0f, 1.0f);

            m_square->end();
        }
        else
        {
            L_WARNING << "Invalid cursor type.";
        }
    }
}
