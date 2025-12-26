#include "Gecko/Resource.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Resource::Resource(const std::string& name) :
        name(name)
    {

    }

    std::shared_ptr<Configuration> Resource::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("name", name);

        configuration->set("current", current);
        configuration->set("maximal", maximal);
        configuration->set("consumption", consumption);
        configuration->set("production", production);

        configuration->set("need_deposit", need_deposit);
        configuration->set("need_storage", need_storage);

        configuration->set("deposit_range", deposit_range);
        configuration->set("storage_range", storage_range);

        configuration->set("priority", priority);

        return configuration;
    }

    void Resource::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        current = configuration->get_float("current", 0.0f);
        maximal = configuration->get_float("maximal", 0.0f);
        consumption = configuration->get_float("consumption", 0.0f);
        production = configuration->get_float("production", 0.0f);

        need_deposit = configuration->get_bool("need_deposit", false);
        need_storage = configuration->get_bool("need_storage", false);

        deposit_range = configuration->get_float("deposit_range", 0.0f);
        storage_range = configuration->get_float("storage_range", 0.0f);

        priority = configuration->get_float("priority", Settings::Game::ResourcePriority);
    }

    void Resource::update(float time)
    {
        // TODO: Implement resource degradation and expansion (spreading).
    }

    float Resource::add(float value)
    {
        if (current + value > maximal)
        {
            value = maximal - current;
        }

        current += value;
        current = std::clamp(current, 0.0f, maximal);

        return value;
    }

    float Resource::remove(float value)
    {
        if (value > current)
        {
            value = current;
        }

        current -= value;
        current = std::clamp(current, 0.0f, maximal);

        return value;
    }
}
