#pragma once

#include "Gecko/Navigation/Coordinate.hpp"
#include "Gecko/Settings.hpp"

#define TYPE_TO_STRING(space, name) (space::name, #name)

namespace Gecko::Utils
{
    Ogre::Vector3 get_node_direction(Ogre::SceneNode& scene_node);

    const Ogre::Vector3& get_node_position(Ogre::SceneNode& scene_node);

    Coordinate get_index_from_position(float x, float z, const Ogre::Vector3& scale);
    Coordinate get_index_from_position(const Ogre::Vector3& position, const Ogre::Vector3& scale);

    std::optional<Ogre::Vector2> get_screenspace_coords(const Ogre::AxisAlignedBox& box, Ogre::Camera* camera, bool convert);

    std::size_t get_window_handle(Ogre::RenderWindow* render_window);

    void rotate_node_towards_position(Ogre::SceneNode& scene_node, const Ogre::Vector3& position, float time);

    bool is_close_enough(const Ogre::Vector3& position, const Ogre::Vector3& target, float distance = Settings::Game::DistanceComparisionMinValue);

    bool is_enough_to_process(float resource);

    void move_resources(std::shared_ptr<Resources> source, std::shared_ptr<Resources> destination, const std::string& name, float value);

    std::string get_filename_from_date();

    std::string json_to_string(const Json::Value& value, bool minify = false);

    void json_to_file(const Json::Value& value, const std::string& file_name, bool minify = false);

    bool is_friendly(const Object& object, const Player& player);

    void fire_missile(Object& owner, const std::string& configuration_name, const Ogre::Vector3& current_position, const Ogre::Vector3& original_target_direction);
}
