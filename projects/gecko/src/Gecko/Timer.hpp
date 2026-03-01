#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Timer :
        public Serializable
    {
    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        explicit Timer(float max = std::numeric_limits<float>::max());
        explicit Timer(const Timer& other);

        Timer& update(float time);

        void reset();

    public:
        float get_current() const
        {
            return m_current;
        }

        float get_max() const
        {
            return m_max;
        }

        float get_progress() const
        {
            return get_current() / get_max();
        }

    public:
        void set_current(float current)
        {
            m_current = current;
        }

        void set_max(float max)
        {
            m_max = max;
        }

    public:
        operator bool() const
        {
            static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

            return std::abs(m_max - m_current) < epsilon;
        }

    private:
        float m_current = 0.0f;
        float m_max     = std::numeric_limits<float>::max();
    };
}
