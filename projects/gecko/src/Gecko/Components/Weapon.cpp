#include "Gecko/Components/Weapon.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    Weapon* Weapon::create(Weapon* memory, const ConfigurationPtr& configuration)
    {
        auto component = new (memory) Weapon();

        component->deserialize(configuration);

        return component;
    }

    Weapon::Weapon(const Weapon& other) :
        base_type(other)
    {
        m_missile_configuration = other.m_missile_configuration;
        m_missiles_fired = other.m_missiles_fired;
        m_rotation_speed = other.m_rotation_speed;
        m_reload_timer = other.m_reload_timer;
    }

    void Weapon::init()
    {
        base_type::init();

        m_reload_timer.reset();
    }

    ConfigurationPtr Weapon::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("missile.configuration", m_missile_configuration);
        configuration->set("missiles_fired", m_missiles_fired);
        configuration->set("rotation_speed", m_rotation_speed);
        configuration->set("reload_timer", m_reload_timer.serialize());

        return configuration;
    }

    void Weapon::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_missile_configuration = configuration->get_string("missile.configuration");
        m_missiles_fired = configuration->get_int<uint16_t>("missiles_fired", 0);
        m_rotation_speed = configuration->get_float("rotation_speed", 0.0f);
        m_reload_timer.deserialize(configuration->get_child("reload_timer"));
    }

    void Weapon::update(float time)
    {
        // TODO: Get target from object.
        /*
        reload_timer.update(time);

        const auto& current_position = Utils::get_node_position(get_scene_node());

        // TODO: Remove y = 0 after fixing Utils::rotate_node_towards_position.
        auto current_direction = Utils::get_node_direction(get_scene_node());
        current_direction.y = 0.0f;
        current_direction.normalise();

        auto target_position = Ogre::Vector3::ZERO;

        auto original_target_direction = target_position - current_position;

        auto target_direction = original_target_direction;
        target_direction.y = 0.0f;
        target_direction.normalise();

        auto target_angle = target_direction.angleBetween(current_direction).valueDegrees();

        if (target_angle <= Settings::Game::MinWeaponAngle)
        {
            if (reload_timer)
            {
                Utils::fire_missile(*this, missile_configuration, current_position, original_target_direction);

                ++missiles_fired;

                reload_timer.reset();
            }
        }
        else
        {
            Utils::rotate_node_towards_position(get_scene_node(), target_position, time);
        }
        */
    }
}
