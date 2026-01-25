#pragma once

#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Navigation/Coordinate.hpp"
#include "Gecko/Tiles/SquareTile.hpp"

namespace Gecko
{
    class SquareLayer :
        public Layer
    {
    private:
        using base_type = Layer;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        // From Layer.
        bool is_index_valid(Index x, Index z) const override;
        bool is_index_valid(const Coordinate& coordinate) const override;

        bool is_position_valid(float x, float z) const override;
        bool is_position_valid(const Ogre::Vector3& position) const override;

        Coordinate get_index(float x, float z) const override;
        Coordinate get_index(const Ogre::Vector3& position) const override;

        Ogre::Vector3 get_normal(float x, float z) const override;
        Ogre::Vector3 get_normal(const Ogre::Vector3& position) const override;

        Ogre::Vector3 get_position(float x, float z, bool validate = true) const override;
        Ogre::Vector3 get_position(const Ogre::Vector3& position, bool validate = true) const override;

    public:
        explicit SquareLayer(Map* owner, const std::string& name, const Configuration& configuration);

        ~SquareLayer() override;

    private:
        std::vector<std::vector<SquareTile>> tiles;

        void init_mesh_data();
        void init_raw_data(const Configuration& configuration);
        void init_tiles(const Configuration& configuration);

        void load_file(const std::string& heightmap, int position_x, int position_z);

        void deserialize_data_layers(const Configuration& configuration);

        std::size_t get_heightmap_size(const Configuration& configuration) const;
    };
}
