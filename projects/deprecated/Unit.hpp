#pragma once

#include "Object.hpp"
#include "Path.hpp"
#include "Timer.hpp"

namespace Gecko
{
    class Unit : public Object
    {
    private:
        using base_type = Object;

    public:
        static Unit* create(Unit* memory, std::shared_ptr<Configuration> configuration);

    public:
        Unit();
        Unit(const Unit& other);

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(std::shared_ptr<Configuration> _configuration) override;

        void update(float time) override;

    public:
        float get_fuel() const
        {
            return fuel;
        }

        float get_max_fuel() const
        {
            return max_fuel;
        }

        float get_range() const
        {
            return range;
        }

    public:
        void set_direction(const Ogre::Vector3& direction) override
        {
            scene_node->setDirection(direction.x, 0.0f, direction.z, Ogre::Node::TransformSpace::TS_WORLD, Ogre::Vector3::NEGATIVE_UNIT_Z);
        }

    private:
        Timer reload_timer;
        float range = 0.0f;
        std::string missile_configuration;

        float distance_traveled = 0.0f;
        float pollution_produced = 0.0f;
        int missiles_fired = 0;

        Timer ai_timer = 1.0;
        Path::Points path;

        float fuel = 0.0f;
        float max_fuel = 0.0f;

        void attack_position(const Ogre::Vector3& position);
        void move_towards_position(const Ogre::Vector3& position, float time, bool remove_order = true);
    };
}
