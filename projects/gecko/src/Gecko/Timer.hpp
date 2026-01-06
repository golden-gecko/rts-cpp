#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Timer :
        public Serializable
    {
    public:
        // From Serializable.
        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        explicit Timer(float maximal = std::numeric_limits<float>::max());
        explicit Timer(const Timer& other);

        Timer& update(float time);

    public:
        float get_current() const
        {
            return current;
        }

        float get_maximal() const
        {
            return maximal;
        }

        float get_progress() const
        {
            return get_current() / get_maximal();
        }

    public:
        void set_current(float _current)
        {
            current = _current;
        }

        void set_maximal(float _maximal)
        {
            maximal = _maximal;
        }

        void reset()
        {
            set_current(0.0f);
        }

    public:
        operator bool() const
        {
            static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

            return std::abs(maximal - current) < epsilon;
        }

    private:
        float current = 0.0f;
        float maximal = std::numeric_limits<float>::max();
    };
}
