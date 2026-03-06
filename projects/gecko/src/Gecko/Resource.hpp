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
        explicit Resource(const std::string& name);

    public:
        std::uint64_t add(std::uint64_t value);
        std::uint64_t remove(std::uint64_t value);

    public:
        float get_consumption() const
        {
            return m_consumption;
        }

        std::uint64_t get_current() const
        {
            return m_current;
        }

        float get_deposit_range() const
        {
            return m_deposit_range;
        }

        std::uint64_t get_max() const
        {
            return m_max;
        }

        const std::string& get_name() const
        {
            return m_name;
        }

        bool get_need_deposit() const
        {
            return m_need_deposit;
        }

        bool get_need_storage() const
        {
            return m_need_storage;
        }

        float get_production() const
        {
            return m_production;
        }

        std::uint64_t get_storage() const
        {
            return m_max - m_current;
        }

        float get_storage_range() const
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

        void set_current(std::uint64_t current)
        {
            m_current = current;
        }

        void set_max(std::uint64_t max)
        {
            m_max = max;
        }

    public:
        bool operator==(const Resource& other) const
        {
            return m_current == other.m_current && m_max == other.m_max;

            /*
            static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

            return std::abs(m_consumption - other.m_consumption) < epsilon
                && std::abs(m_production - other.m_production) < epsilon
                && std::abs(m_current - other.m_current) < epsilon
                && std::abs(m_max - other.m_max) < epsilon;
            */
        }

    private:
        std::string m_name;

        std::uint64_t m_current = 0;
        std::uint64_t m_max = 0;

        float m_consumption = 0.0f;
        float m_production = 0.0f;

        bool m_need_deposit = false;
        bool m_need_storage = false;

        float m_deposit_range = 0.0f;
        float m_storage_range = 0.0f;
    };
}
