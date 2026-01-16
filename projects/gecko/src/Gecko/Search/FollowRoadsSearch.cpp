#include "Gecko/Search/FollowRoadsSearch.hpp"

#include "Gecko/DataLayers/DataLayer.hpp"
#include "Gecko/Layers/SquareLayer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Validators/SquareCenterPosition.hpp"
#include "Gecko/Validators/RoadPosition.hpp"

namespace Gecko
{
    std::optional<Path::Points> FollowRoadsSearch::get_path(const Ogre::Vector3& from, const Ogre::Vector3& to)
    {
        return Path::Points();

        /*
        auto from_index = m_layer.get_index(from);
        auto to_index = m_layer.get_index(to);

        auto pair = std::make_pair(from_index, to_index);
        auto points = m_cache.get(pair);

        // Start and destination are the same.
        if (from_index == to_index)
        {
            return Path::Points();
        }

        // Check cache.
        if (points)
        {
            L_INFO << "Cache hit for path " << from_index.to_string() << " " << to_index.to_string() << " " << points.value().size() << ".";

            return points.value();
        }

        // Go in each direction and find destination.
        std::vector<std::vector<Index>> visited(m_layer.get_size(), std::vector<Index>(m_layer.get_size(), 0));
        std::list<Coordinate> nodes;

        nodes.emplace_back(from_index);
        visited[from_index.x][from_index.z] = 1;

        std::array<Coordinate, 4> indices =
        {
            Coordinate(-1, 0,  0),
            Coordinate( 0, 0,  1),
            Coordinate( 0, 0, -1),
            Coordinate( 1, 0,  0)
        };

        //std::array<Coordinate, 8> indices =
        //{
            //Coordinate(-1, -1),
            //Coordinate(-1,  0),
            //Coordinate(-1,  1),
            //Coordinate( 0,  1),
            //Coordinate( 0, -1),
            //Coordinate( 1, -1),
            //Coordinate( 1,  0),
            //Coordinate( 1,  1)
        //};

        enum class PathPointSnapType :
            std::uint8_t
        {
            Center,
            Road
        };

        auto snap_type = PathPointSnapType::Road;
        auto data_layer = m_layer.get_data_layer("Roads"); // TODO: Remove hardcoded value.

        while (nodes.size() > 0)
        {
            const auto& current = nodes.front();

            if (current == to_index)
            {
                break;
            }

            for (size_t i = 0; i < indices.size(); i++)
            {
                Coordinate next(current.x + indices[i].x, current.z + indices[i].z);

                if (m_layer.is_index_valid(next) == false)
                {
                    continue;
                }

                //if (terrain.get_layer(LayerType::Value::Occupied)->as<TerrainLayerOccupied>()->get_data(next) == 0)
                //{
                    //if (visited[next.x][next.z] == 0)
                    //{
                        //nodes.emplace_back(next);

                        //visited[next.x][next.z] = visited[current.x][current.z] + 1;
                    //}
                //}

                auto data_layer_value = data_layer->get_data(next.x, next.z);

                // TODO: Remove hardcoded value.
                // TODO: Fix.
                if (data_layer_value == 255)
                {
                    if (visited[next.x][next.z] == 0)
                    {
                        nodes.emplace_back(next);
                        visited[next.x][next.z] = visited[current.x][current.z] + 1;
                    }
                }
            }

            nodes.pop_front();
        }

        // Go from destination to start.
        std::vector<Coordinate> node_path;

        if (nodes.size() == 0 || nodes.front() != to_index)
        {
            auto points = Path::Points();
            auto path = Path(from_index, to_index, points);

            save_cache(pair, points);

            return points;
        }

        auto node = to_index;

        node_path.emplace(node_path.begin(), to_index);

        bool found = false;

        while (found == false)
        {
            for (size_t i = 0; i < indices.size(); i++)
            {
                Index x = node.x + indices[i].x;
                Index z = node.z + indices[i].z;

                if (m_layer.is_index_valid(x, z) == false)
                {
                    continue;
                }

                Index current = visited[node.x][node.z];
                Index next = visited[x][z];

                if (current - next == 1)
                {
                    node_path.emplace(node_path.begin(), x, z);
                    node = Coordinate(x, 0, z);
                }

                if (next == 1)
                {
                    found = true;
                    break;
                }
            }
        }

        // Compute points.
        {
            Path::Points points;

            for (std::vector<Coordinate>::size_type i = 0; i < node_path.size(); i++)
            {
                Coordinate previous;
                Coordinate current = node_path[i];
                Coordinate next;

                Index previousDeltaX = 0;
                Index previousDeltaZ = 0;

                if (i > 0)
                {
                    previous = node_path[i - 1];

                    previousDeltaX = current.x - previous.x;
                    previousDeltaZ = current.z - previous.z;
                }

                Index nextDeltaX = 0;
                Index nextDeltaZ = 0;

                if (i < node_path.size() - 1)
                {
                    next = node_path[i + 1];

                    nextDeltaX = next.x - current.x;
                    nextDeltaZ = next.z - current.z;
                }

                Index deltaX = previousDeltaX + nextDeltaX;
                Index deltaZ = previousDeltaZ + nextDeltaZ;

                deltaX = std::clamp<Index>(deltaX, -1, 1);
                deltaZ = std::clamp<Index>(deltaZ, -1, 1);

                const auto& scale = m_layer.get_scale();

                switch (snap_type)
                {
                    case PathPointSnapType::Center:
                    {
                        auto validator = SquareCenterPosition(m_layer);
                        auto point = Ogre::Vector3(current.x * scale.x, 0.0f, current.z * scale.z);
                        auto validated = validator.validate(point, Ogre::Vector3(deltaX, 0.0f, deltaZ));

                        points.emplace_back(validated);

                        break;
                    }

                    case PathPointSnapType::Road:
                    {
                        auto validator = RoadPosition(m_layer);
                        auto point = Ogre::Vector3(current.x * scale.x, 0.0f, current.z * scale.z);
                        auto validated = validator.validate(point, Ogre::Vector3(deltaX, 0.0f, deltaZ));

                        points.emplace_back(validated);

                        break;
                    }
                }
            }

            save_cache(pair, points);

            return points;
        }
        */
    }
    
    FollowRoadsSearch::FollowRoadsSearch(const Layer& layer) :
        base_type(layer)
    {
    }
}
