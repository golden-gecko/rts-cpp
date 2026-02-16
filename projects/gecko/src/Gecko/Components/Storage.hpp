#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Storage :
        public Component
    {
    private:
        using base_type = Component;

    public:
        static Storage* create(Storage* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Storage() = default;
        explicit Storage(const Storage& other) = default;
    };
}
