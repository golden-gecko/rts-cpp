#pragma once

#include "Gecko/Coordinate.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Path.hpp"

namespace Gecko
{
    class Layer :
        public Initializable,
        public Updatable
    {
    public:
        explicit Layer(Map* owner, const std::string& name, const Configuration& configuration);

    public:
        virtual bool is_index_valid(Index x, Index z) const = 0;
        virtual bool is_index_valid(const Coordinate& coordinate) const = 0;

        virtual bool is_position_valid(float x, float z) const = 0;
        virtual bool is_position_valid(const Ogre::Vector3& position) const = 0;

        std::optional<Path::Points> search(const Ogre::Vector3& from, const Ogre::Vector3& to) const;

        void show_data_layer(const std::string& data_layer_name);

        Ogre::Vector3 validate_position(const Ogre::Vector3& position) const;

    public:
        std::shared_ptr<DataLayer> get_data_layer(const std::string& name) const;

        const auto& get_data_layers() const
        {
            return m_data_layers;
        }

        const auto& get_grid_scale() const
        {
            return m_grid_scale;
        }

        virtual Coordinate get_index(float x, float z) const = 0;
        virtual Coordinate get_index(const Ogre::Vector3& position) const = 0;

        const auto& get_material_name() const
        {
            return m_material_name;
        }

        const auto& get_name() const
        {
            return m_name;
        }

        virtual Ogre::Vector3 get_normal(float x, float z) const = 0;
        virtual Ogre::Vector3 get_normal(const Ogre::Vector3& position) const = 0;

        const Ogre::Vector3& get_normal_data(int x, int z) const
        {
            return m_normals[x][z];
        }

        auto get_owner() const
        {
            return m_owner;
        }

        virtual Ogre::Vector3 get_position(float x, float z, bool validate = true) const = 0;
        virtual Ogre::Vector3 get_position(const Ogre::Vector3& position, bool validate = true) const = 0;

        const auto& get_scale() const
        {
            return m_scale;
        }

        Ogre::SceneNode& get_scene_node() const
        {
            return *m_scene_node;
        }

        std::size_t get_size() const
        {
            return m_data.size();
        }

        const std::vector<Ogre::Vector2>& get_texture_coordinate_data(int x, int z) const
        {
            return m_texture_coordinates[x][z];
        }

        const Ogre::Vector2& get_texture_coordinate_data(int x, int z, int i) const
        {
            return m_texture_coordinates[x][z][i];
        }

        std::size_t get_tile_size() const
        {
            return m_tile_size;
        }

        const Ogre::Vector3& get_vertex_data(int x, int z) const
        {
            return m_vertices[x][z];
        }

    public:
        void set_visible(bool visible)
        {
            m_scene_node->setVisible(visible);
        }

    protected:
        Map* m_owner = nullptr;

        std::string m_name;
        std::string m_material_name;

        std::vector<std::vector<std::uint8_t>> m_data;

        Indices m_indices;
        Normals m_normals;
        TextureCoordinates m_texture_coordinates;
        Vertices m_vertices;

        int m_tile_size = 0;

        Ogre::Vector3 m_scale = Ogre::Vector3::UNIT_SCALE;
        Ogre::Vector3 m_grid_scale = Ogre::Vector3::UNIT_SCALE;

        std::map<std::string, std::shared_ptr<DataLayer>> m_data_layers;
        std::shared_ptr<PositionValidator> m_position_validator;
        std::shared_ptr<Search> m_search;

        Ogre::SceneNode* m_scene_node = nullptr;

    private:
        void deserialize_position_validator(const Configuration& configuration);
        void deserialize_search(const Configuration& configuration);
    };
}
