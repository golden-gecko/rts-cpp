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
        explicit Storage() = default;
        explicit Storage(const Storage& other) = default;

    public:
        float get_load_time() const
        {
            return m_load_time;
        }

        float get_unload_time() const
        {
            return m_unload_time;
        }

    private:
        float m_load_time = 0.0f;
        float m_unload_time = 0.0f;
    };
}
