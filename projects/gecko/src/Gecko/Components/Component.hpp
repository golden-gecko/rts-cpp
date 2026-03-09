#pragma once

#include "Gecko/Item.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    class Component :
        public Item
    {
    private:
        using base_type = Item;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        explicit Component() = default;
        Component(const Component& other);

    public:
        Component& operator=(const Component&) = delete;

    public:
        virtual bool is_visible() const
        {
            return false;
        }

    public:
        virtual Ogre::Vector3 get_direction() const
        {
            return Ogre::Vector3::ZERO;
        }

        float get_health() const
        {
            return m_health_timer.get_current();
        }

        float get_max_health() const
        {
            return m_health_timer.get_max();
        }

        const std::string& get_name() const
        {
            return m_name;
        }

        virtual Ogre::Vector3 get_size() const
        {
            return Ogre::Vector3::ZERO;
        }

        ObjectPtr get_owner() const
        {
            return m_owner;
        }

    public:
        virtual void set_owner(ObjectPtr owner)
        {
            m_owner = owner;
        }

        void set_scene(const ScenePtr& scene)
        {
            m_scene = scene;
        }

        void set_name(const std::string& name)
        {
            m_name = name;
        }

        virtual void set_position(const Ogre::Vector3& position)
        {
        }

        virtual void set_visible(bool visible)
        {
        }

    protected:
        ObjectPtr   m_owner = nullptr;
        ScenePtr    m_scene;
        std::string m_name;
        Timer       m_health_timer;
    };
}
