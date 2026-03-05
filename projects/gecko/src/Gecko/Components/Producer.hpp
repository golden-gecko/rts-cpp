#pragma once

#include "Gecko/Components/Component.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    class Producer :
        public Component
    {
    private:
        using base_type = Component;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        static Producer* create(Producer* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Producer();
        explicit Producer(const Producer& other);

    public:
        float get_produce_time() const
        {
            return m_produce_time;
        }

    private:
        float m_produce_time = 0.0f;
    };
}
