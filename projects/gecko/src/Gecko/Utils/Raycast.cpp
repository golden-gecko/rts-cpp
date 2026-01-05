#include "Gecko/Utils/Raycast.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/QueryFlags.hpp"
#include "Gecko/Settings.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/Mesh.hpp"

namespace Gecko
{
    Ogre::Ray create_ray(const OIS::MouseEvent& arg)
    {
        auto x = static_cast<float>(arg.state.X.abs) / static_cast<float>(arg.state.width);
        auto y = static_cast<float>(arg.state.Y.abs) / static_cast<float>(arg.state.height);

        return MapManager::getSingleton().begin()->second->get_camera(Settings::Camera::MainName)->get_camera()->getCameraToViewportRay(x, y);
    }

    std::set<Id> from_plane(const Ogre::Vector2& start, const Ogre::Vector2& end, Ogre::uint32 query_mask)
    {
        float left = start.x;
        float right = end.x;
        float top = start.y;
        float bottom = end.y;

        if (left > right)
        {
            std::swap(left, right);
        }

        if (top > bottom)
        {
            std::swap(top, bottom);
        }

        if ((right - left) * (bottom - top) < Settings::UI::SelectionBoxMinSize)
        {
            return std::set<Id>();
        }

        auto camera = MapManager::getSingleton().begin()->second->get_camera(Settings::Camera::MainName)->get_camera();

        auto topLeft = camera->getCameraToViewportRay(left, top);
        auto topRight = camera->getCameraToViewportRay(right, top);
        auto bottomLeft = camera->getCameraToViewportRay(left, bottom);
        auto bottomRight = camera->getCameraToViewportRay(right, bottom);

        auto frontPlane = Ogre::Plane(topLeft.getOrigin(), topRight.getOrigin(), bottomRight.getOrigin());
        auto topPlane = Ogre::Plane(topLeft.getOrigin(),topLeft.getPoint(10), topRight.getPoint(10));
        auto leftPlane = Ogre::Plane(topLeft.getOrigin(), bottomLeft.getPoint(10), topLeft.getPoint(10));
        auto bottomPlane = Ogre::Plane(bottomLeft.getOrigin(), bottomRight.getPoint(10), bottomLeft.getPoint(10));
        auto rightPlane = Ogre::Plane(topRight.getOrigin(), topRight.getPoint(10), bottomRight.getPoint(10));

        Ogre::PlaneBoundedVolume vol;
        vol.planes.push_back(frontPlane);
        vol.planes.push_back(topPlane);
        vol.planes.push_back(leftPlane);
        vol.planes.push_back(bottomPlane);
        vol.planes.push_back(rightPlane);

        Ogre::PlaneBoundedVolumeList volList;
        volList.push_back(vol);

        // TODO: Move to game class.
        static auto query = Game::getSingleton().get_scene_manager()->createPlaneBoundedVolumeQuery(volList, query_mask);

        query->setVolumes(volList);
        query->setQueryMask(query_mask);

        auto& result = query->execute();
        std::set<Id> objects;

        for (const auto& i : result.movables)
        {
            auto object_id = Utils::Convert::to_id(*i);

            if (object_id.is_valid())
            {
                objects.emplace(object_id);
            }
        }

        // Game::getSingleton().get_scene_manager()->destroyQuery(query);

        return objects;
    }

    std::optional<std::pair<Ogre::Entity*, Ogre::Vector3>> from_point(const Ogre::Ray& ray, Ogre::uint32 query_mask)
    {
        static auto m_pray_scene_query = Game::getSingleton().create_ray_scene_query(ray);

        m_pray_scene_query->setRay(ray);
        m_pray_scene_query->setSortByDistance(true);
        m_pray_scene_query->setQueryMask(query_mask);

        auto& query_result = m_pray_scene_query->execute();

        // execute the query, returns a vector of hits
        if (query_result.size() <= 0)
        {
            // raycast did not hit an objects bounding box
            return std::make_pair(nullptr, Ogre::Vector3::ZERO);
        }

        // at this point we have raycast to a series of different objects bounding boxes.
        // we need to test these different objects to see which is the first polygon hit.
        // there are some minor optimizations (distance based) that mean we wont have to
        // check all of the objects most of the time, but the worst case scenario is that
        // we need to test every triangle of every object.
        Ogre::Real closest_distance = -1.0f;
        Ogre::Vector3 closest_result = Ogre::Vector3::ZERO;
        Ogre::Entity* closest_pentity = nullptr;

        for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
        {
            // stop checking if we have found a raycast hit that is closer
            // than all remaining entities
            if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance))
            {
                break;
            }

            // only check this result if its a hit against an entity
            if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
            {
                // get the entity to check
                auto pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

                if (pentity->isVisible() == false)
                {
                    continue;
                }

                // mesh data to retrieve
                size_t vertex_count = 0;
                size_t index_count = 0;
                Ogre::Vector3 *vertices = nullptr;
                unsigned long *indices = nullptr;

                // get the mesh information
                Utils::Mesh::get_information(
                    pentity->getMesh(),
                    vertex_count,
                    vertices,
                    index_count,
                    indices,
                    pentity->getParentNode()->_getDerivedPosition(),
                    pentity->getParentNode()->_getDerivedOrientation(),
                    pentity->getParentNode()->_getDerivedScale()
                );

                // test for hitting individual triangles on the mesh
                bool new_closest_found = false;

                for (int i = 0; i < static_cast<int>(index_count); i += 3)
                {
                    // check for a hit against this triangle
                    std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray,
                        vertices[indices[i]],
                        vertices[indices[i + 1]],
                        vertices[indices[i + 2]], true, false);

                    // if it was a hit check if its the closest
                    if (hit.first)
                    {
                        if ((closest_distance < 0.0f) ||
                            (hit.second < closest_distance))
                        {
                            // this is the closest so far, save it off
                            closest_distance = hit.second;
                            new_closest_found = true;
                        }
                    }
                }

                // free the verticies and indicies memory
                delete[] vertices;
                delete[] indices;

                // if we found a new closest raycast for this object, update the
                // closest_result before moving on to the next object.
                if (new_closest_found)
                {
                    closest_result = ray.getPoint(closest_distance);
                    closest_pentity = pentity;
                }
            }
        }

        // return the result
        if (closest_distance >= 0.0f)
        {
            // raycast success
            return std::make_pair(closest_pentity, closest_result);
        }
        else
        {
            // raycast failed
            return std::make_pair(nullptr, Ogre::Vector3::ZERO);
        }
    }
}

namespace Gecko::Utils::Raycast
{
    std::optional<std::pair<Layer*, Ogre::Vector3>> to_layer(const OIS::MouseEvent& arg)
    {
        return to_layer(create_ray(arg));
    }

    std::optional<std::pair<Id, Ogre::Vector3>> to_object(const OIS::MouseEvent& arg)
    {
        return to_object(create_ray(arg));
    }

    std::optional<std::pair<Layer*, Ogre::Vector3>> to_layer(const Ogre::Ray& ray)
    {
        auto result = from_point(ray, QueryFlags::QF_Layer);

        static auto m_pray_scene_query = Game::getSingleton().create_ray_scene_query(ray);

        m_pray_scene_query->setRay(ray);
        m_pray_scene_query->setSortByDistance(true);
        m_pray_scene_query->setQueryMask(QueryFlags::QF_Layer);

        auto& query_result = m_pray_scene_query->execute();

        // execute the query, returns a vector of hits
        if (query_result.size() <= 0)
        {
            // raycast did not hit an objects bounding box
            return std::make_pair(nullptr, Ogre::Vector3::ZERO);
        }

        // at this point we have raycast to a series of different objects bounding boxes.
        // we need to test these different objects to see which is the first polygon hit.
        // there are some minor optimizations (distance based) that mean we wont have to
        // check all of the objects most of the time, but the worst case scenario is that
        // we need to test every triangle of every object.
        Ogre::Real closest_distance = -1.0f;
        Ogre::Vector3 closest_result = Ogre::Vector3::ZERO;
        Ogre::Entity* closest_pentity = nullptr;

        for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
        {
            // stop checking if we have found a raycast hit that is closer
            // than all remaining entities
            if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance))
            {
                break;
            }

            // only check this result if its a hit against an entity
            if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
            {
                // get the entity to check
                auto pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

                if (pentity->isVisible() == false)
                {
                    continue;
                }

                // mesh data to retrieve
                size_t vertex_count = 0;
                size_t index_count = 0;
                Ogre::Vector3 *vertices = nullptr;
                unsigned long *indices = nullptr;

                // get the mesh information
                Mesh::get_information(
                    pentity->getMesh(),
                    vertex_count,
                    vertices,
                    index_count,
                    indices,
                    pentity->getParentNode()->_getDerivedPosition(),
                    pentity->getParentNode()->_getDerivedOrientation(),
                    pentity->getParentNode()->_getDerivedScale()
                );

                // test for hitting individual triangles on the mesh
                bool new_closest_found = false;

                for (int i = 0; i < static_cast<int>(index_count); i += 3)
                {
                    // check for a hit against this triangle
                    std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray,
                        vertices[indices[i]],
                        vertices[indices[i + 1]],
                        vertices[indices[i + 2]], true, false);

                    // if it was a hit check if its the closest
                    if (hit.first)
                    {
                        if ((closest_distance < 0.0f) ||
                            (hit.second < closest_distance))
                        {
                            // this is the closest so far, save it off
                            closest_distance = hit.second;
                            new_closest_found = true;
                        }
                    }
                }

                // free the verticies and indicies memory
                delete[] vertices;
                delete[] indices;

                // if we found a new closest raycast for this object, update the
                // closest_result before moving on to the next object.
                if (new_closest_found)
                {
                    closest_result = ray.getPoint(closest_distance);
                    closest_pentity = pentity;
                }
            }
        }

        // return the result
        if (closest_distance >= 0.0f)
        {
            auto layer = Convert::to_layer(closest_pentity);

            if (layer)
            {
                // TODO: Restore.
                return std::make_pair(layer, closest_result);
            }

            // raycast failed
            return {};
        }
        else
        {
            // raycast failed
            return {};
        }
    }

    std::optional<std::pair<Id, Ogre::Vector3>> to_object(const Ogre::Ray& ray)
    {
        static auto m_pray_scene_query = Game::getSingleton().create_ray_scene_query(ray);

        m_pray_scene_query->setRay(ray);
        m_pray_scene_query->setSortByDistance(true);
        m_pray_scene_query->setQueryMask(QueryFlags::QF_Object);

        auto& query_result = m_pray_scene_query->execute();

        // execute the query, returns a vector of hits
        if (query_result.size() <= 0)
        {
            // raycast did not hit an objects bounding box
            return {};
        }

        // at this point we have raycast to a series of different objects bounding boxes.
        // we need to test these different objects to see which is the first polygon hit.
        // there are some minor optimizations (distance based) that mean we wont have to
        // check all of the objects most of the time, but the worst case scenario is that
        // we need to test every triangle of every object.
        Ogre::Real closest_distance = -1.0f;
        Ogre::Vector3 closest_result = Ogre::Vector3::ZERO;
        Ogre::Entity* closest_pentity = nullptr;

        for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
        {
            // stop checking if we have found a raycast hit that is closer
            // than all remaining entities
            if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance))
            {
                break;
            }

            // only check this result if its a hit against an entity
            if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
            {
                // get the entity to check
                auto pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

                if (pentity->isVisible() == false)
                {
                    continue;
                }

                // mesh data to retrieve
                size_t vertex_count = 0;
                size_t index_count = 0;
                Ogre::Vector3 *vertices = nullptr;
                unsigned long *indices = nullptr;

                // get the mesh information
                Mesh::get_information(
                    pentity->getMesh(),
                    vertex_count,
                    vertices,
                    index_count,
                    indices,
                    pentity->getParentNode()->_getDerivedPosition(),
                    pentity->getParentNode()->_getDerivedOrientation(),
                    pentity->getParentNode()->_getDerivedScale()
                );

                // test for hitting individual triangles on the mesh
                bool new_closest_found = false;

                for (std::size_t i = 0; i < index_count; i += 3)
                {
                    // check for a hit against this triangle
                    std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray,
                        vertices[indices[i]],
                        vertices[indices[i + 1]],
                        vertices[indices[i + 2]], true, false);

                    // if it was a hit check if its the closest
                    if (hit.first)
                    {
                        if ((closest_distance < 0.0f) ||
                            (hit.second < closest_distance))
                        {
                            // this is the closest so far, save it off
                            closest_distance = hit.second;
                            new_closest_found = true;
                        }
                    }
                }

                // free the verticies and indicies memory
                delete[] vertices;
                delete[] indices;

                // if we found a new closest raycast for this object, update the
                // closest_result before moving on to the next object.
                if (new_closest_found)
                {
                    closest_result = ray.getPoint(closest_distance);
                    closest_pentity = pentity;
                }
            }
        }

        // return the result
        if (closest_distance >= 0.0f)
        {
            auto object_id = Convert::to_id(*closest_pentity);

            if (object_id.is_valid())
            {
                return std::make_pair(object_id, closest_result);
            }

            return {};
        }
        else
        {
            // raycast failed
            return {};
        }
    }

    std::set<Id> to_objects(const Ogre::Vector2& start, const Ogre::Vector2& end)
    {
        return from_plane(start, end, QueryFlags::QF_Object);
    }
}
