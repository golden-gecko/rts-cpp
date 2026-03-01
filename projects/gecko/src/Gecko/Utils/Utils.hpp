#pragma once

#include "Gecko/Navigation/Coordinate.hpp"
#include "Gecko/Settings.hpp"

#define TYPE_TO_STRING(space, name) (space::name, #name)

namespace Gecko::Utils
{
    void fire_missile(Object& owner, const std::string& configuration_name, const Ogre::Vector3& current_position, const Ogre::Vector3& original_target_direction);

    Navigation::Coordinate get_index_from_position(float x, float z, const Ogre::Vector3& scale);
    Navigation::Coordinate get_index_from_position(const Ogre::Vector3& position, const Ogre::Vector3& scale);

    std::optional<Ogre::Vector2> get_screenspace_coords(const Ogre::AxisAlignedBox& box, Ogre::Camera* camera, bool convert);

    std::uint64_t get_window_handle(Ogre::RenderWindow* render_window);

    bool is_close_enough(const Ogre::Vector3& position, const Ogre::Vector3& target, float distance = Settings::Game::DistanceComparisionMinValue);

    bool is_enough_to_process(float resource);

    void move_resources(const std::shared_ptr<Resources>& source, const std::shared_ptr<Resources>& destination, const std::string& name, float value);

    std::string object_id_to_string(const Id& object_id);
}
