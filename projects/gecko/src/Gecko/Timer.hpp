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
        Timer(const Timer& other);

    public:
        bool finished() const
        {
            static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

            return std::abs(m_max - m_current) < epsilon;
        }

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
            if (m_max == 0.0f)
            {
                return 0.0f;
            }

            return m_current / m_max;
        }

        std::string get_progress_as_string(std::uint8_t precision = 1) const
        {
            std::stringstream stream;

            stream << std::fixed << std::setprecision(precision);
            stream << get_current() << "/" << get_max();

            return stream.str();
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
            return finished();
        }

    private:
        float m_current = 0.0f;
        float m_max     = std::numeric_limits<float>::max();
    };
}
