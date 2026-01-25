#include "Gecko/Navigation/Search/AvoidOccupiedSearch.hpp"

#include "Gecko/DataLayers/DataLayer.hpp"
#include "Gecko/Layers/Layer.hpp"

namespace Gecko
{
    std::optional<Path::Points> AvoidOccupiedSearch::get_path(const Ogre::Vector3& from, const Ogre::Vector3& to)
    {
        auto from_index = m_layer.get_index(from);
        auto to_index = m_layer.get_index(to);

        if (from_index == to_index)
        {
            return Path::Points();
        }

        struct node_t
        {
            Coordinate position;

            int g = 0; // from start
            int h = 0; // to end
            int f = 0; // g + h

            Coordinate parent;
        };

        auto get_g_cost = [&](node_t* node)
        {
            return (node->position.x - from_index.x) * (node->position.x - from_index.x)
                 + (node->position.y - from_index.y) * (node->position.y - from_index.y)
                 + (node->position.z - from_index.z) * (node->position.z - from_index.z);
        };

        auto get_h_cost = [&](node_t* node)
        {
            return (node->position.x - to_index.x) * (node->position.x - to_index.x)
                 + (node->position.y - to_index.y) * (node->position.y - to_index.y)
                 + (node->position.z - to_index.z) * (node->position.z - to_index.z);
        };

        auto comparator = [&](node_t* left, node_t* right)
        {
            return left->f < right->f;
        };

        int size = m_layer.get_size();
        std::vector<std::vector<node_t>> map(size, std::vector<node_t>(size));

        for (int x = 0; x < size; ++x)
        {
            for (int z = 0; z < size; ++z)
            {
                map[x][z].position.x = x;
                map[x][z].position.z = z;
            }
        }

        std::array<Coordinate, 8> indices =
        {
            Coordinate(-1, 0, -1),
            Coordinate(-1, 0,  0),
            Coordinate(-1, 0,  1),
            Coordinate( 0, 0,  1),
            Coordinate( 0, 0, -1),
            Coordinate( 1, 0, -1),
            Coordinate( 1, 0,  0),
            Coordinate( 1, 0,  1)
        };

        auto* from_node = &map[from_index.x][from_index.z];
        auto* to_node = &map[to_index.x][to_index.z];

        std::list<node_t*> open;
        std::list<node_t*> closed;

        from_node->g = get_g_cost(from_node);
        from_node->h = get_g_cost(to_node);
        from_node->f = from_node->g + from_node->h;

        open.push_back(from_node);

        // TODO: Hardcoded.
        auto navigation_layer = m_layer.get_data_layer("Navigation");
        bool target_found = false;

        // TODO: If target is blocked, find nearest available one.
        if (navigation_layer->get_data(to_index.x, to_index.z) > 0)
        {
            return {};
        }

        while (open.empty() == false)
        {
            open.sort(comparator);

            auto current = open.front();
            open.pop_front();
            closed.push_back(current);

            if (current == to_node)
            {
                target_found = true;
                break;
            }

            for (int i = 0; i < indices.size(); i++)
            {
                int x = current->position.x;
                int z = current->position.z;

                int xx = x + indices[i].x;
                int zz = z + indices[i].z;

                if (m_layer.is_index_valid(xx, zz) == false)
                {
                    continue;
                }

                auto* adjacent = &map[xx][zz];

                if (std::ranges::find(closed, adjacent) != closed.end())
                {
                    continue;
                }

                auto value = navigation_layer->get_data(xx, zz);

                // TODO: Check adjacent tiles if movement is possible.
                // TODO: For example, object cannot move diagonal if adjacent tiles are blocked.
                if (value > 0)
                {
                    continue;
                }

                adjacent->g = get_g_cost(adjacent);
                adjacent->h = get_g_cost(adjacent);
                adjacent->f = adjacent->g + adjacent->h;

                auto is_in_open = std::ranges::find(open, adjacent) != open.end();

                if (adjacent->f < current->f || is_in_open == false)
                {
                    adjacent->parent = current->position;

                    if (is_in_open == false)
                    {
                        open.push_back(adjacent);
                    }
                }
            }
        }

        if (target_found == false)
        {
            return {};
        }

        // Go from destination to start.
        Path::Points node_path;

        node_path.push_back(to_index);

        bool found = false;
        auto current = to_node;

        while (current != from_node)
        {
            node_path.emplace_front(current->position);

            int parent_x = current->parent.x;
            int parent_z = current->parent.z;

            current = &map[parent_x][parent_z];
        }

        return node_path;
    }

    AvoidOccupiedSearch::AvoidOccupiedSearch(const Layer& layer) :
        base_type(layer)
    {
    }
}
