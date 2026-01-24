#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Process :
        public Serializable
    {
    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        explicit Process(const std::string& name);

        void update(float time, Id id, const Ogre::Vector3& position, std::shared_ptr<Resources> resources);

    public:
        const auto& get_name() const
        {
            return m_name;
        }

        const std::shared_ptr<Resources>& get_in() const
        {
            return m_in;
        }

        const std::shared_ptr<Resources>& get_out() const
        {
            return m_out;
        }

    public:
        bool operator==(const Process& other) const;

    private:
        std::string m_name;

        std::shared_ptr<Resources> m_in;
        std::shared_ptr<Resources> m_out;

        Object* get_deposit(const Resource& in_resource, Id id, const Ogre::Vector3& position, float time) const;
        Object* get_storage(const Resource& out_resource, Id id, const Ogre::Vector3& position, float time) const;
    };
}
