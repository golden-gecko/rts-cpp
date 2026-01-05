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
        static Shield* create(Shield* memory, const std::shared_ptr<Configuration>& configuration);

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        explicit Shield() = default;
        explicit Shield(const Shield& other);

    private:
        float defence = 0.0f;
    };
}
