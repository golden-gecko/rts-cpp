#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Radar :
        public Component
    {
    private:
        using base_type = Component;

    public:
        static Radar* create(Radar* memory, const ConfigurationPtr& configuration);

    public:
        explicit Radar() = default;
        explicit Radar(const Radar& other) = default;
    };
}
