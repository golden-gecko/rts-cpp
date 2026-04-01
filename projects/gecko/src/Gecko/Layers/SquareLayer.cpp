#include "Gecko/Layers/SquareLayer.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/DataLayers/DataLayer.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Scenes/Scene.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/Math.hpp"
#include "Gecko/Utils/Texture.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    void SquareLayer::init()
    {
        for (auto& z : tiles)
        {
            for (auto& x : z)
            {
                x.init();
            }
        }

        for (const auto& [name, data_layer] : m_data_layers)
        {
            data_layer->init();
        }
    }

    void SquareLayer::deinit()
    {
        for (const auto& [name, data_layer] : m_data_layers)
        {
            data_layer->deinit();
        }

        for (auto& z : tiles)
        {
            for (auto& x : z)
            {
                x.deinit();
            }
        }
    }

    void SquareLayer::update(float time)
    {
        for (const auto& [_, data_layer] : m_data_layers)
        {
            data_layer->update(time);
        }
    }

    SquareLayer::SquareLayer(MapPtr owner, const std::string& name, const ConfigurationPtr& configuration) :
        base_type(owner, name, configuration)
    {
        // Get material name.
        m_material_name = configuration->get_string("material");

        // Get scale.
        m_scale = configuration->get_vector3("scale", Ogre::Vector3::UNIT_SCALE);

        if (m_scale.x <= 0.0f || m_scale.y <= 0.0f || m_scale.z <= 0.0f)
        {
            throw Exception("Scale cannot be lower then zero. Setting to 1.0:1.0:1.0.");
        }

        // Get grid scale.
        m_grid_scale = configuration->get_vector3("grid.scale", Ogre::Vector3::UNIT_SCALE);

        // Do not check Y, because grid is two-dimensional.
        if (m_grid_scale.x <= 0.0f || m_grid_scale.z <= 0.0f)
        {
            throw Exception("Grid scale cannot be lower then zero. Setting to 1.0:1.0:1.0.");
        }

        // Do not check Y, because grid is two-dimensional.
        if (m_grid_scale.x > m_scale.x || m_grid_scale.z > m_scale.z)
        {
            throw Exception("Grid scale cannot be greater than scale. Setting to 1.0:1.0:1.0.");
        }

        deserialize_data_layers();

        // Create scene node.
        m_scene_node = m_owner->get_scene()->create_scene_node();

        init_raw_data();
        init_mesh_data();
        init_tiles();
    }

    SquareLayer::~SquareLayer()
    {
        m_owner->get_scene()->destroy_scene_node(m_scene_node);
    }

    bool SquareLayer::is_index_valid(Index x, Index z) const
    {
        if (x < 0 || x >= get_size() - 1)
        {
            return false;
        }

        if (z < 0 || z >= get_size() - 1)
        {
            return false;
        }

        return true;
    }

    bool SquareLayer::is_index_valid(const Navigation::Coordinate& coordinate) const
    {
        return is_index_valid(coordinate.x, coordinate.z);
    }

    bool SquareLayer::is_position_valid(float x, float z) const
    {
        if (x < 0.0f || z < 0.0f)
        {
            return false;
        }

        auto size_x = get_size() * get_scale().x;
        auto size_z = get_size() * get_scale().z;

        if (x > size_x || z > size_z)
        {
            return false;
        }

        return true;
    }

    bool SquareLayer::is_position_valid(const Ogre::Vector3& position) const
    {
        // TODO: Check y.
        return is_position_valid(position.x, position.z);
    }

    Navigation::Coordinate SquareLayer::get_index(float x, float z) const
    {
        return Utils::get_index_from_position(x, z, get_grid_scale());
    }

    Navigation::Coordinate SquareLayer::get_index(const Ogre::Vector3& position) const
    {
        return get_index(position.x, position.z);
    }

    Ogre::Vector3 SquareLayer::get_normal(float x, float z) const
    {
        x = std::clamp(x, 0.0f, get_size() * m_scale.x);
        z = std::clamp(z, 0.0f, get_size() * m_scale.z);

        auto x_index = std::clamp<int>(x / m_scale.x, 0, get_size() - 1);
        auto z_index = std::clamp<int>(z / m_scale.z, 0, get_size() - 1);

        auto x_rest = x - (x_index * m_scale.x);
        auto z_rest = z - (z_index * m_scale.z);

        auto ray_length = 1000.0f;
        auto ray = Ogre::Ray(Ogre::Vector3(x, ray_length, z), Ogre::Vector3::NEGATIVE_UNIT_Y);

        if (x_rest + z_rest < m_scale.x) // TODO: Make scale scalar instead of vector.
        {
            Ogre::Vector3 p[3];

            p[0] = m_vertices[x_index    ][z_index    ];
            p[1] = m_vertices[x_index    ][z_index + 1];
            p[2] = m_vertices[x_index + 1][z_index    ];

            auto hit = Ogre::Math::intersects(ray, Ogre::Plane(p[0], p[1], p[2]));

            if (hit.first)
            {
                auto v1 = p[1] - p[0];
                auto v2 = p[2] - p[1];

                auto normal = v1.crossProduct(v2);

                return normal;
            }
        }
        else
        {
            Ogre::Vector3 p[3];

            p[0] = m_vertices[x_index + 1][z_index    ];
            p[1] = m_vertices[x_index    ][z_index + 1];
            p[2] = m_vertices[x_index + 1][z_index + 1];

            auto hit = Ogre::Math::intersects(ray, Ogre::Plane(p[0], p[1], p[2]));

            if (hit.first)
            {
                auto v1 = p[1] - p[0];
                auto v2 = p[2] - p[1];

                auto normal = v1.crossProduct(v2);

                return normal;
            }
        }

        return Ogre::Vector3(x, 0.0f, z);
    }

    Ogre::Vector3 SquareLayer::get_normal(const Ogre::Vector3& position) const
    {
        return get_normal(position.x, position.z);
    }

    Ogre::Vector3 SquareLayer::get_position(float x, float z, bool validate) const
    {
        x = std::clamp(x, 0.0f, get_size() * m_scale.x);
        z = std::clamp(z, 0.0f, get_size() * m_scale.z);

        auto x_index = x / m_scale.x;
        auto z_index = z / m_scale.z;

        x_index = std::clamp<int>(x_index, 0, get_size() - 1);
        z_index = std::clamp<int>(z_index, 0, get_size() - 1);

        auto x_rest = x - (x_index * m_scale.x);
        auto z_rest = z - (z_index * m_scale.z);

        auto ray_length = 1000.0f;
        auto ray = Ogre::Ray(Ogre::Vector3(x, ray_length, z), Ogre::Vector3::NEGATIVE_UNIT_Y);

        Ogre::Vector3 p[3];

        if (x_rest + z_rest < m_scale.x) // TODO: Make scale scalar instead of vector.
        {
            p[0] = m_vertices[x_index    ][z_index    ];
            p[1] = m_vertices[x_index    ][z_index + 1];
            p[2] = m_vertices[x_index + 1][z_index    ];

            Ogre::RayTestResult hit = Ogre::Math::intersects(ray, Ogre::Plane(p[0], p[1], p[2]));

            if (hit.first)
            {
                Ogre::Vector3 result = Ogre::Vector3(x, ray_length - hit.second, z);

                return validate ? validate_position(result) : result;
            }
        }
        else
        {
            p[0] = m_vertices[x_index + 1][z_index    ];
            p[1] = m_vertices[x_index    ][z_index + 1];
            p[2] = m_vertices[x_index + 1][z_index + 1];

            Ogre::RayTestResult hit = Ogre::Math::intersects(ray, Ogre::Plane(p[0], p[1], p[2]));

            if (hit.first)
            {
                Ogre::Vector3 result = Ogre::Vector3(x, ray_length - hit.second, z);

                return validate ? validate_position(result) : result;
            }
        }

        return Ogre::Vector3(x, 0.0f, z);
    }

    Ogre::Vector3 SquareLayer::get_position(const Ogre::Vector3& position, bool validate) const
    {
        return get_position(position.x, position.z, validate);
    }

    void SquareLayer::init_mesh_data()
    {
        auto size = m_data.size() + 1;

        m_vertices.resize(size, std::vector<Ogre::Vector3>(size));
        m_texture_coordinates.resize(size, std::vector<std::vector<Ogre::Vector2>>(size, std::vector<Ogre::Vector2>(3)));
        m_normals.resize(size, std::vector<Ogre::Vector3>(size));

        for (auto z = 0; z < size; ++z)
        {
            for (auto x = 0; x < size; ++x)
            {
                // Compute height based on neighbours.
                int indices[4][2] =
                {
                    { -1, -1 },
                    {  0, -1 },
                    { -1,  0 },
                    {  0,  0 }
                };

                int count = 0;
                float height = 0.0f;

                for (int i = 0; i < 4; i++)
                {
                    int xx = x + indices[i][0];
                    int zz = z + indices[i][1];

                    if (xx < 0 || xx > size - 2 || zz < 0 || zz > size - 2)
                    {
                        continue;
                    }

                    ++count;
                    height += static_cast<float>(m_data[xx][zz]);
                }

                m_vertices[x][z] = Ogre::Vector3(
                    static_cast<float>(x) * m_scale.x,
                    height / static_cast<float>(count) * m_scale.y,
                    static_cast<float>(z) * m_scale.z
                );

                // Global texture coordinates.
                m_texture_coordinates[x][z][0] = Ogre::Vector2(
                    x / static_cast<float>(size - 1),
                    z / static_cast<float>(size - 1)
                );

                // Detail texture coordinates.
                m_texture_coordinates[x][z][1] = Ogre::Vector2(
                    static_cast<float>(x),
                    static_cast<float>(z)
                );

                // Grid texture coordinates.
                m_texture_coordinates[x][z][2] = Ogre::Vector2(
                    static_cast<float>(x * (m_scale.x / m_grid_scale.x)),
                    static_cast<float>(z * (m_scale.z / m_grid_scale.z))
                );

                m_normals[x][z] = Ogre::Vector3(0.0f, 1.0f, 0.0f); // TODO: Compute normals.
            }
        }
    }

    void SquareLayer::init_raw_data()
    {
        auto z_row = m_configuration->get_child("data");
        auto z_row_size = z_row->size();

        auto heightmap_size = get_heightmap_size();
        auto data_size = z_row_size * heightmap_size;

        // Allocate memory.
        m_data.resize(data_size, std::vector<std::uint8_t>(data_size));

        // Load data.
        for (Json::Value::ArrayIndex z = 0; z < z_row_size; ++z)
        {
            auto x_row = z_row->get_element(z);
            auto x_row_size = x_row->size();

            if (z_row_size != x_row_size)
            {
                throw Exception(
                    "Tiles does not form a square ("
                    + Utils::Convert::to_string(z_row->size())
                    + " != "
                    + Utils::Convert::to_string(x_row->size())
                    + ")."
                );
            }

            for (Json::Value::ArrayIndex x = 0; x < x_row_size; ++x)
            {
                load_file(x_row->get_element(x)->get_string(), x, z);
            }
        }
    }

    void SquareLayer::init_tiles()
    {
        // Get shards.
        std::uint16_t shards = m_configuration->get_int<std::uint16_t>("shards", 1);
        std::uint16_t shards_per_row = static_cast<std::uint16_t>(std::sqrt(shards));

        if (shards < 1)
        {
            throw Exception("'shards' must be greater than 0.");
        }

        if (Utils::Math::is_power_2(shards) == false)
        {
            throw Exception("'shards' must be one of 1, 4, 16 or 64.");
        }

        if (shards_per_row < 1)
        {
            throw Exception("'shards_per_row' must be greater than 0.");
        }

        // Get heightmap size.
        auto heightmap_size = get_heightmap_size();

        if (heightmap_size % shards_per_row != 0)
        {
            throw Exception("'shards' must be a multiple of heightmap size.");
        }

        // Get tiles.
        auto z_row = m_configuration->get_child("data");
        auto z_row_size = z_row->size();

        // Allocate memory.
        tiles.resize(shards_per_row, std::vector<SquareTile>(shards_per_row));

        for (auto z = 0; z < shards_per_row; ++z)
        {
            for (auto x = 0; x < shards_per_row; ++x)
            {
                tiles[x][z].set_owner(this);
                tiles[x][z].set_tile_position(Navigation::Coordinate(x, 0, z));
            }
        }

        // Save tile size.
        m_tile_size = m_data.size() / shards_per_row;
    }

    void SquareLayer::load_file(const std::string& heightmap, int position_x, int position_z)
    {
        auto image = Utils::Texture::load_square_image(heightmap);
        auto image_size = image.getWidth();

        std::size_t row_span = image.getRowSpan();
        std::size_t bytes_per_pixel = row_span / image.getWidth();

        auto image_data = image.getData();

        auto start_x = position_x * image_size;
        auto start_z = position_z * image_size;

        for (auto z = 0; z < image_size; ++z)
        {
            for (auto x = 0; x < image_size; ++x)
            {
                // TODO: Save images as grayscale.
                auto image_offset = z * row_span + x * bytes_per_pixel;
                auto r = image_data[image_offset];

                m_data[start_x + x][start_z + z] = static_cast<std::uint8_t>(r);
            }
        }
    }

    void SquareLayer::deserialize_data_layers()
    {
        auto data_layers_configuration = m_configuration->get_child_optional("data_layers");

        if (data_layers_configuration)
        {
            for (auto i = data_layers_configuration.value()->begin(); i != data_layers_configuration.value()->end(); i++)
            {
                auto data_layer_name = i.key().asString();

                m_data_layers.emplace(data_layer_name, std::make_unique<DataLayer>(this, data_layer_name, Configuration(*i)));
            }
        }
    }

    std::size_t SquareLayer::get_heightmap_size() const
    {
        if (m_configuration->has_member("data") == false)
        {
            return 0;
        }

        ConfigurationPtr sources = m_configuration->get_child("data");

        if (sources->size() <= 0)
        {
            return 0;
        }

        ConfigurationPtr element = sources->get_element(0);

        if (element->size() <= 0)
        {
            return 0;
        }

        return Utils::Texture::load_square_image(element->get_element(0)->get_string()).getWidth();
    }
}
