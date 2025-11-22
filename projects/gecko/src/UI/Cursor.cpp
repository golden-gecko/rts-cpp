#include "Cursor.hpp"

#include "Components/Component.hpp"
#include "Configuration.hpp"
#include "Games/Game.hpp"
#include "Layers/Layer.hpp"
#include "Log.hpp"
#include "Objects/Factory.hpp"
#include "Objects/Missile.hpp"
#include "Objects/Vehicle.hpp"
#include "QueryFlags.hpp"

namespace Gecko
{
    Cursor::Cursor()
    {
        square = Game::getSingleton().create_manual_object();
        square->setQueryFlags(QueryFlags::QF_CURSOR);

        scene_node = Game::getSingleton().create_scene_node();
        scene_node->attachObject(square);
        scene_node->setPosition(Settings::UI::CursorOffset);
    }

    Cursor::~Cursor()
    {
        Game::getSingleton().destroy_scene_node(scene_node);
        Game::getSingleton().destroy_manual_object(square);
    }

    void Cursor::set_mesh(const std::shared_ptr<Configuration>& configuration)
    {
        set_visible(false);

        // Save configuration name.
        mesh_name = configuration->get_name();

        // Find or create component and object.
        auto object = objects.find(mesh_name);

        if (object == objects.end())
        {
            // TODO: Create object factory.
            auto type = configuration->get_string("type");

            if (type == "Factory")
            {
                auto object = std::make_unique<Factory>();

                object->deserialize(configuration);
                object->set_visible(true);
                object->init();

                objects.emplace(std::make_pair(configuration->get_name(), std::move(object)));
            }
            else if (type == "Missile")
            {
                auto object = std::make_unique<Missile>();

                object->deserialize(configuration);
                object->set_visible(true);
                object->init();

                objects.emplace(std::make_pair(configuration->get_name(), std::move(object)));
            }
            else if (type == "Object")
            {
                auto object = std::make_unique<Object>();

                object->deserialize(configuration);
                object->set_visible(true);
                object->init();

                objects.emplace(std::make_pair(configuration->get_name(), std::move(object)));
            }
            else if (type == "Vehicle")
            {
                auto object = std::make_unique<Vehicle>();

                object->deserialize(configuration);
                object->set_visible(true);
                object->init();

                objects.emplace(std::make_pair(configuration->get_name(), std::move(object)));
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

    void Cursor::set_type(Type type)
    {
        set_visible(false);

        // Save type.
        this->type = type;

        // Show cursor.
        if (type == Type::Component)
        {
            auto component = components.find(mesh_name);

            if (component != components.end())
            {
                component->second->set_visible(true);
            }
        }
        else if (type == Type::Object)
        {
            auto object = objects.find(mesh_name);

            if (object != objects.end())
            {
                object->second->set_visible(true);
            }
        }
        else if (type == Type::Square)
        {
            scene_node->setVisible(true);
        }
        else
        {
            // TODO: Log warning.
        }
    }

    void Cursor::set_visible(bool visible)
    {
        // Hide all.
        scene_node->setVisible(false);

        for (const auto& [configuration_name, component] : components)
        {
            component->set_visible(false);
        }

        for (const auto& [configuration_name, object] : objects)
        {
            object->set_visible(false);
        }

        // Show cursor.
        if (visible)
        {
            if (type == Type::Component)
            {
                auto component = components.find(mesh_name);

                if (component != components.end())
                {
                    component->second->set_visible(true);
                }
            }
            else if (type == Type::Object)
            {
                auto object = objects.find(mesh_name);

                if (object != objects.end())
                {
                    object->second->set_visible(true);
                }
            }
            else if (type == Type::Square)
            {
                scene_node->setVisible(true);
            }
            else
            {
                // TODO: Log warning.
            }
        }
    }

    void Cursor::update(const std::pair<Layer*, Ogre::Vector3>& cast)
    {
        if (cast.first == nullptr)
        {
            return;
        }

        position = cast.first->get_index(cast.second);

        if (type == Type::Component)
        {
            auto component = components.find(mesh_name);

            if (component != components.end())
            {
                component->second->set_position(cast.second);
            }
        }
        else if (type == Type::Object)
        {
            auto object = objects.find(mesh_name);

            if (object != objects.end())
            {
                object->second->set_position(cast.second);
            }
        }
        else if (type == Type::Square)
        {
            // Get node vertices.
            auto index_x = static_cast<Index>(std::floorf(cast.second.x / cast.first->get_scale().x));
            auto index_z = static_cast<Index>(std::floorf(cast.second.z / cast.first->get_scale().z));

            auto size = cast.first->get_size() + 1;

            // Get grid vertices.
            const auto& grid_scale = cast.first->get_grid_scale();

            auto g00 = Ogre::Vector3( position.x      * grid_scale.x, 0.0f,  position.z      * grid_scale.z);
            auto g01 = Ogre::Vector3( position.x      * grid_scale.x, 0.0f, (position.z + 1) * grid_scale.z);
            auto g11 = Ogre::Vector3((position.x + 1) * grid_scale.x, 0.0f, (position.z + 1) * grid_scale.z);
            auto g10 = Ogre::Vector3((position.x + 1) * grid_scale.x, 0.0f,  position.z      * grid_scale.z);

            // Get Y position of grid vertices.
            g00 = cast.first->get_position(g00.x, g00.z, false);
            g01 = cast.first->get_position(g01.x, g01.z, false);
            g11 = cast.first->get_position(g11.x, g11.z, false);
            g10 = cast.first->get_position(g10.x, g10.z, false);

            // Create cursor.
            square->clear();
            square->begin("cursor_grid_red"); // TODO: Hardcoded.

            square->position(g00);
            square->normal(Ogre::Vector3::UNIT_Y);
            square->textureCoord(0.0f, 0.0f);

            square->position(g01);
            square->normal(Ogre::Vector3::UNIT_Y);
            square->textureCoord(1.0f, 0.0f);

            square->position(g10);
            square->normal(Ogre::Vector3::UNIT_Y);
            square->textureCoord(0.0f, 1.0f);

            square->position(g10);
            square->normal(Ogre::Vector3::UNIT_Y);
            square->textureCoord(0.0f, 1.0f);

            square->position(g01);
            square->normal(Ogre::Vector3::UNIT_Y);
            square->textureCoord(1.0f, 0.0f);

            square->position(g11);
            square->normal(Ogre::Vector3::UNIT_Y);
            square->textureCoord(1.0f, 1.0f);

            square->end();
        }
        else
        {
            // TODO: Log warning.
        }
    }
}
