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
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

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
            return m_consumption;
        }

        auto get_current() const
        {
            return m_current;
        }

        auto get_deposit_range() const
        {
            return m_deposit_range;
        }

        // TODO: Unify naming (max vs maximal).
        auto get_max() const
        {
            return m_maximal;
        }

        const auto& get_name() const
        {
            return m_name;
        }

        auto get_need_deposit() const
        {
            return m_need_deposit;
        }

        auto get_need_storage() const
        {
            return m_need_storage;
        }

        auto get_priority() const
        {
            return m_priority;
        }

        auto get_production() const
        {
            return m_production;
        }

        auto get_storage() const
        {
            return m_maximal - m_current;
        }

        auto get_storage_range() const
        {
            return m_storage_range;
        }

    public:
        void set_consumption(float consumption)
        {
            m_consumption = consumption;
        }

        void set_production(float production)
        {
            m_production = production;
        }

        void set_current(float current)
        {
            m_current = current;
        }

        void set_maximal(float maximal)
        {
            m_maximal = maximal;
        }

    public:
        bool operator==(const Resource& other) const
        {
            static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

            return std::abs(m_consumption - other.m_consumption) < epsilon
                && std::abs(m_production - other.m_production) < epsilon
                && std::abs(m_current - other.m_current) < epsilon
                && std::abs(m_maximal - other.m_maximal) < epsilon;
        }

    private:
        std::string m_name;

        float m_current = 0.0f;
        float m_maximal = 0.0f;
        float m_consumption = 0.0f;
        float m_production = 0.0f;

        bool m_need_deposit = false;
        bool m_need_storage = false;

        float m_deposit_range = 0.0f;
        float m_storage_range = 0.0f;

        float m_priority = Settings::Game::ResourcePriority;
    };
}
