#include "Gecko/Utils/Utils.hpp"

#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"

namespace Gecko::Utils
{
    void fire_missile(Object& owner, const std::string& configuration_name, const Ogre::Vector3& current_position, const Ogre::Vector3& original_target_direction)
    {
        ObjectPtr missile = ObjectManager::getSingleton().create(configuration_name);

        if (missile == nullptr)
        {
            L_WARNING << "Failed to create object '" << configuration_name << "'.";
        }

        // TODO: Move object manager to map?
        missile->set_owner(owner.get_owner());
        missile->init();
        missile->set_position(current_position);
        missile->set_direction(original_target_direction);
    }

    Navigation::Coordinate get_index_from_position(float x, float z, const Ogre::Vector3& scale)
    {
        auto x_index = static_cast<Index>(std::floorf(x / scale.x));
        auto z_index = static_cast<Index>(std::floorf(z / scale.z));

        return Navigation::Coordinate(x_index, 0, z_index);
    }

    Navigation::Coordinate get_index_from_position(const Ogre::Vector3& position, const Ogre::Vector3& scale)
    {
        return get_index_from_position(position.x, position.z, scale);
    }

    std::optional<Ogre::Vector2> get_screenspace_coords(const Ogre::AxisAlignedBox& box, Ogre::Camera* camera, bool convert)
    {
        /**
        * If you need the point above the object instead of the center point:
        * This snippet derives the average point between the top-most corners of the bounding box
        * Ogre::Vector3 point = (AABB.getCorner(AxisAlignedBox::FAR_LEFT_TOP)
        *    + AABB.getCorner(AxisAlignedBox::FAR_RIGHT_TOP)
        *    + AABB.getCorner(AxisAlignedBox::NEAR_LEFT_TOP)
        *    + AABB.getCorner(AxisAlignedBox::NEAR_RIGHT_TOP)) / 4;
        */
        auto point = box.getCenter();

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

    std::uint64_t get_window_handle(Ogre::RenderWindow* render_window)
    {
        std::uint64_t render_window_handle = 0;

        render_window->getCustomAttribute("WINDOW", &render_window_handle);

        return render_window_handle;
    }

    bool is_close_enough(const Ogre::Vector3& position, const Ogre::Vector3& target, float distance)
    {
        return position.distance(target) < distance;
    }

    bool is_enough_to_process(float resource)
    {
        return resource >= Settings::Order::ResourceTransportMinValue;
    }

    void move_resources(const std::shared_ptr<Resources>& source, const std::shared_ptr<Resources>& destination, const std::string& name, float value)
    {
        source->remove(name, value);
        destination->add(name, value);
    }

    std::string object_id_to_string(const Id& object_id)
    {
        std::stringstream stream;

        if (ObjectPtr object = ObjectManager::getSingleton().get(object_id))
        {
            stream << object->get_name() << " (" << object_id << ")";
        }

        return stream.str();
    }
}
