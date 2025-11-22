#include "Objects/Portal.hpp"

namespace Gecko
{
    Portal* Portal::create(Portal* memory, std::shared_ptr<Configuration> configuration)
    {
        auto map = new (memory) Portal();

        map->deserialize(configuration);

        return map;
    }

    Portal::Portal()
    {
    }

    Portal::Portal(const Portal& other) :
        base_type(other)
    {
    }

    void Portal::init()
    {
        base_type::init();

        // map->get_terrain().set_portal_position(get_position());
    }

    void Portal::deinit()
    {
        base_type::deinit();

        // map->get_terrain().remove_portal_from_position(get_position());
    }

    void Portal::set_position(const Ogre::Vector3& position)
    {
        base_type::set_position(position);

        /*
        auto old_position = get_position();

        base_type::set_position(position);

        map->get_terrain().update_portal_position(old_position, get_position());
        */
    }
}
