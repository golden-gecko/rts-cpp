#pragma once

#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    class Resource :
        public Serializable,
        public Updatable
    {
    public:
        // From Serializable.
        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        Resource(const std::string& name);

    public:
        float add(float value);
        float remove(float value);

    public:
        auto get_consumption() const
        {
            return consumption;
        }

        auto get_current() const
        {
            return current;
        }

        auto get_deposit_range() const
        {
            return deposit_range;
        }

        auto get_max() const
        {
            return maximal;
        }

        const auto& get_name() const
        {
            return name;
        }

        auto get_need_deposit() const
        {
            return need_deposit;
        }

        auto get_need_storage() const
        {
            return need_storage;
        }

        auto get_priority() const
        {
            return priority;
        }

        auto get_production() const
        {
            return production;
        }

        auto get_storage() const
        {
            return maximal - current;
        }

        auto get_storage_range() const
        {
            return storage_range;
        }

    public:
        bool operator==(const Resource& other) const
        {
            static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

            return std::abs(consumption - other.consumption) < epsilon
                && std::abs(production - other.production) < epsilon
                && std::abs(current - other.current) < epsilon
                && std::abs(maximal - other.maximal) < epsilon;
        }

    private:
        std::string name;

        float current = 0.0f;
        float maximal = 0.0f;
        float consumption = 0.0f;
        float production = 0.0f;

        bool need_deposit = false;
        bool need_storage = false;

        float deposit_range = 0.0f;
        float storage_range = 0.0f;

        float priority = Settings::Game::ResourcePriority;
    };
}
