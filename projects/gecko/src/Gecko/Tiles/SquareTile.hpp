#pragma once

#include "Gecko/Tiles/Tile.hpp"

#include "Gecko/Coordinate.hpp"

namespace Gecko
{
    class SquareTile :
        public Tile
    {
    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        std::string get_mesh_name() const;

    public:
        void set_tile_position(const Coordinate& tile_position)
        {
            m_tile_position = tile_position;
        }

    private:
        Coordinate m_tile_position;

        void create_mesh();
    };
}
