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
        static Shield* create(Shield* memory, const ConfigurationPtr& configuration);

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        explicit Shield() = default;
        explicit Shield(const Shield& other);

    private:
        float m_defence = 0.0f;
    };
}
