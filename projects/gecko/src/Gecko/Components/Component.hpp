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
        // From Updatable.
        void update(float time) override;

    public:
        explicit Component() = default;
        explicit Component(const Component& other);

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

        auto get_health() const
        {
            return m_health_timer.get_current();
        }

        auto get_max_health() const
        {
            return m_health_timer.get_max();
        }

        const auto& get_name() const
        {
            return m_name;
        }

        virtual Ogre::Vector3 get_size() const
        {
            return Ogre::Vector3::ZERO;
        }

        auto get_owner() const
        {
            return m_owner;
        }

    public:
        void set_name(const std::string& name)
        {
            m_name = name;
        }

        virtual void set_owner(Object* owner)
        {
            m_owner = owner;
        }

        virtual void set_position(const Ogre::Vector3& position);

        virtual void set_visible(bool visible);

    protected:
        Object* m_owner = nullptr;
        std::string m_name;
        Timer m_health_timer;
    };
}
