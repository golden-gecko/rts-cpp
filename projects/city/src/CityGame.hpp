#pragma once

#include "Game.hpp"

namespace City
{
    class Game : public Gecko::Game
    {
    private:
        using base_type = Gecko::Game;

    public:
        explicit Game(std::shared_ptr<Gecko::Configuration> configuration);

    protected:
        void init_maps(std::size_t max_size) override;
        void init_objects(std::size_t max_size) override;
    };
}
