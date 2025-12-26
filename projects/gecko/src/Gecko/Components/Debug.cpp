#include "Gecko/Components/Debug.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Utils/Mesh.hpp"

namespace Gecko
{
    Debug* Debug::create(Debug* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Debug();

        component->deserialize(configuration);

        return component;
    }

    Debug::Debug()
    {
        m_manual_object = Game::getSingleton().create_manual_object();
    }

    Debug::Debug(const Debug& other) :
        base_type(other)
    {
        if (other.m_manual_object)
        {
            m_manual_object = Utils::Mesh::copy_manual_object(*(other.m_manual_object));
        }

        if (other.m_scene_node)
        {
            m_scene_node = Utils::Mesh::copy_scene_node(*(other.m_scene_node));
            m_scene_node->attachObject(m_manual_object);
        }
    }

    void Debug::init()
    {
        assert(m_owner);

        m_scene_node = m_owner->get_scene_node().createChildSceneNode();
        m_scene_node->attachObject(m_manual_object);

        // Draw direction.
        draw_line(Ogre::Vector3::ZERO, Ogre::Vector3::NEGATIVE_UNIT_Z * 5.0f, Ogre::ColourValue::Red);

        // Draw normal (up).
        draw_line(Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_Y * 5.0f, Ogre::ColourValue::Blue);

        // Draw normal (right).
        draw_line(Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_X * 5.0f, Ogre::ColourValue::Green);
    }

    void Debug::update(float time)
    {
    }

    void Debug::draw_line(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& color)
    {
        assert(m_manual_object);

        m_manual_object->begin("vertex_color", Ogre::RenderOperation::OT_LINE_LIST);

        m_manual_object->position(start);
        m_manual_object->colour(color);
        m_manual_object->normal(Ogre::Vector3::ZERO);

        m_manual_object->position(end);
        m_manual_object->colour(color);
        m_manual_object->normal(Ogre::Vector3::ZERO);

        m_manual_object->end();
    }
}
