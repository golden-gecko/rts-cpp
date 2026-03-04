#pragma once

#include "Gecko/Components/Component.hpp"

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
        // From Update.
        void update(float time) override;

    public:
        static Producer* create(Producer* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Producer();
        explicit Producer(const Producer& other);

    private:
        ProcessesPtr m_processes;

        float m_produce_time = 0.0f;
    };
}
