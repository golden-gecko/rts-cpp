#include "SeasonCycle.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    SeasonCycle::SeasonCycle(const std::string& name, const Configuration& configuration) :
        name(name)
    {
        /*
        for (const auto& season : configuration)
        {
            seasons.push_back(Season(season.first, season.second.get_child("")));
        }

        if (seasons.size())
        {
            seasons.front().apply();
        }
        */
    }

    void SeasonCycle::update(float time)
    {
        if (seasons.size())
        {
            seasons.front().timer.update(time);

            if (seasons.front().timer)
            {
                // Reset timer of current season.
                seasons.front().timer.reset();

                // Move seasons that ended to end of queue.
                seasons.push_back(seasons.front());
                seasons.pop_front();

                // Apply next season.
                seasons.front().apply();
            }
        }
    }
}
