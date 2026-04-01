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
        Timer& get_produce_time()
        {
            return m_produce_time;
        }

    private:
        Timer m_produce_time;
    };
}
