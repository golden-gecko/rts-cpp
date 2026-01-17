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
        explicit Timer(float maximal = std::numeric_limits<float>::max());
        explicit Timer(const Timer& other);

        Timer& update(float time);

    public:
        float get_current() const
        {
            return m_current;
        }

        float get_maximal() const
        {
            return m_maximal;
        }

        float get_progress() const
        {
            return get_current() / get_maximal();
        }

    public:
        void set_current(float current)
        {
            m_current = current;
        }

        void set_maximal(float maximal)
        {
            m_maximal = maximal;
        }

        void reset()
        {
            set_current(0.0f);
        }

    public:
        operator bool() const
        {
            static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

            return std::abs(m_maximal - m_current) < epsilon;
        }

    private:
        float m_current = 0.0f;
        float m_maximal = std::numeric_limits<float>::max();
    };
}
