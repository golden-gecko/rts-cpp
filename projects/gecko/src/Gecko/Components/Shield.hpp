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

    public:
        explicit Shield() = default;
        explicit Shield(const Shield& other);

    private:
        float m_defence = 0.0f;
    };
}
