#pragma once

#include "Season.hpp"

namespace Gecko
{
    class SeasonCycle
    {
    public:
        SeasonCycle(const std::string& name, const Configuration& configuration);

        const Season& get_current() const
        {
            return seasons.front();
        }

        const auto& get_name() const
        {
            return name;
        }

        void update(float time);
    private:
        std::string name;

        std::list<Season> seasons;
    };
}
