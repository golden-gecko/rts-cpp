#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Shield :
        public Component
    {
    private:
        using base_type = Component;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        static Shield* create(Shield* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    private:
        float m_defence = 0.0f;
    };
}
