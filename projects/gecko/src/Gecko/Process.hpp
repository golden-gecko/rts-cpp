#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Timer.hpp"

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

    public:
        void update(float time, const Id& owner_id, const Ogre::Vector3& owner_position, const ResourcesPtr& owner_resources);

    public:
        const std::string& get_name() const
        {
            return m_name;
        }

        const ResourcesPtr& get_in() const
        {
            return m_in;
        }

        const ResourcesPtr& get_out() const
        {
            return m_out;
        }

        const Timer& get_time() const
        {
            return m_time;
        }

    public:
        bool operator==(const Process& other) const;

    private:
        std::string m_name;

        ResourcesPtr m_in;
        ResourcesPtr m_out;

        Timer m_time;

        ObjectPtr get_deposit(const Resource& in_resource, const Ogre::Vector3& position, const Id& exclude) const;
        ObjectPtr get_storage(const Resource& out_resource, const Ogre::Vector3& position, const Id& exclude) const;
    };
}
