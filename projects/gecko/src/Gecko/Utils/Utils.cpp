#include "Gecko/Utils/Utils.hpp"

#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"

namespace Gecko::Utils
{
    Ogre::Vector3 get_node_direction(Ogre::SceneNode& scene_node)
    {
        return scene_node._getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
    }

    const Ogre::Vector3& get_node_position(Ogre::SceneNode& scene_node)
    {
        return scene_node._getDerivedPosition();
    }

    Coordinate get_index_from_position(float x, float z, const Ogre::Vector3& scale)
    {
        auto x_index = static_cast<Index>(std::floorf(x / scale.x));
        auto z_index = static_cast<Index>(std::floorf(z / scale.z));

        return Coordinate(x_index, 0, z_index);
    }

    Coordinate get_index_from_position(const Ogre::Vector3& position, const Ogre::Vector3& scale)
    {
        return get_index_from_position(position.x, position.z, scale);
    }

    std::optional<Ogre::Vector2> get_screenspace_coords(const Ogre::Entity& object, Ogre::Camera* camera, bool convert)
    {
        if (object.isInScene() == false)
        {
            return {};
        }

        const auto& AABB = object.getWorldBoundingBox(true);

        /**
        * If you need the point above the object instead of the center point:
        * This snippet derives the average point between the top-most corners of the bounding box
        * Ogre::Vector3 point = (AABB.getCorner(AxisAlignedBox::FAR_LEFT_TOP)
        *    + AABB.getCorner(AxisAlignedBox::FAR_RIGHT_TOP)
        *    + AABB.getCorner(AxisAlignedBox::NEAR_LEFT_TOP)
        *    + AABB.getCorner(AxisAlignedBox::NEAR_RIGHT_TOP)) / 4;
        */
        auto point = AABB.getCenter();

        // Is the camera facing that point? If not, return false
        auto cameraPlane = Ogre::Plane(Ogre::Vector3(camera->getDerivedOrientation().zAxis()), camera->getDerivedPosition());

        if (cameraPlane.getSide(point) != Ogre::Plane::NEGATIVE_SIDE)
        {
            return {};
        }

        // Transform the 3D point into screen space
        point = camera->getProjectionMatrix() * (camera->getViewMatrix() * point);

        Ogre::Vector2 result;

        if (convert)
        {
            // Transform from coordinate space [-1, 1] to [0, 1] and update in-value
            result.x = (point.x / 2) + 0.5f;
            result.y = 1 - ((point.y / 2) + 0.5f);
        }
        else
        {
            result.x = point.x;
            result.y = point.y;
        }

        return result;
    }

    void rotate_node_towards_position(Ogre::SceneNode& scene_node, const Ogre::Vector3& position, float time)
    {
        auto current_position = get_node_position(scene_node);
        current_position.y = 0.0f;

        auto current_direction = get_node_direction(scene_node);
        current_direction.y = 0.0f;
        current_direction.normalise();

        auto target_direction = position - current_position;
        auto target_rotation = current_direction.getRotationTo(target_direction);

        auto y = target_rotation.getYaw().valueDegrees();
        auto p = target_rotation.getPitch().valueDegrees();

        if (std::fabsf(y) > 1.0f)
        {
            float y_speed = 10.0f;
            float y_direction = y > 0.0f ? 1.0f : -1.0f;
            float y_turn_value = y_direction * y_speed * time;

            scene_node.yaw(Ogre::Degree(y_turn_value));
        }

        if (std::fabsf(p) > 1.0f)
        {
            float p_speed = 10.0f;
            float p_direction = p > 0.0f ? 1.0f : -1.0f;
            float p_turn_value = p_direction * p_speed * time;

            // TODO: Fix.
            // scene_node.pitch(Ogre::Degree(p_turn_value));
        }
    }

    bool is_close_enough(const Ogre::Vector3& position, const Ogre::Vector3& target, float distance)
    {
        return position.distance(target) < distance;
    }

    bool is_enough_to_process(float resource)
    {
        return resource >= Settings::Game::ResourceTransportMinValue;
    }

    void move_resources(std::shared_ptr<Resources> source, std::shared_ptr<Resources> destination, const std::string& name, float value)
    {
        source->remove(name, value);
        destination->add(name, value);
    }

    std::string get_filename_from_date()
    {
        // TODO: Refactor.
        auto now = std::time(nullptr);
        auto local_now = std::localtime(&now);

        std::ostringstream oss;

        oss << std::put_time(local_now, "%Y_%m_%d_%H_%M_%S");

        return oss.str();
    }

    std::string json_to_string(const Json::Value& value, bool minify)
    {
        Json::StreamWriterBuilder builder;

        builder["commentStyle"] = "None";
        builder["indentation"] = "";

        return Json::writeString(builder, value);
    }

    void json_to_file(const Json::Value& value, const std::string& file_name, bool minify)
    {
        Json::StreamWriterBuilder builder;

        builder["commentStyle"] = "None";
        builder["indentation"] = "";

        std::ofstream stream(file_name, std::ifstream::binary);

        stream << Json::writeString(builder, value);
    }

    bool is_friendly(const Object& object, const Player& player)
    {
        // TODO: Use diplomacy and move to player class.
        return object.get_player_id() == player.get_id();
    }

    void fire_missile(Object& owner, const std::string& configuration_name, const Ogre::Vector3& current_position, const Ogre::Vector3& original_target_direction)
    {
        auto missile = ObjectManager::getSingleton().create(configuration_name);

        if (missile == nullptr)
        {
            throw Exception("Failed to create object '" + configuration_name + "'.");
        }

        // TODO: Move object manager to map?
        missile->set_owner(owner.get_owner());
        missile->init();
        missile->set_position(current_position);
        missile->set_direction(original_target_direction);
    }
}
