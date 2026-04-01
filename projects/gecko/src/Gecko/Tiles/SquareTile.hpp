#pragma once

#include "Gecko/Tiles/Tile.hpp"

#include "Gecko/Navigation/Coordinate.hpp"

namespace Gecko
{
    class SquareTile :
        public Tile
    {
    // From Initializable.
    public:
        void init() override;
        void deinit() override;

    // SquareTile.
    public:
        std::string get_mesh_name() const;

    public:
        void set_tile_position(const Navigation::Coordinate& tile_position)
        {
            m_tile_position = tile_position;
        }

    private:
        Navigation::Coordinate m_tile_position;

        void create_mesh();
    };
}
