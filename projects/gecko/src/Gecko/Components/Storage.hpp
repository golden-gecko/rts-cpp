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
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        static Storage* create(Storage* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Storage();
        Storage(const Storage& other);

    public:
        Timer& get_load_time()
        {
            return m_load_time;
        }

        Timer& get_unload_time()
        {
            return m_unload_time;
        }

    private:
        Timer m_load_time;
        Timer m_unload_time;
    };
}
