#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Process : public Serializable
    {
    public:
        explicit Process(const std::string& name);

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

        void update(float time, Id id, const Ogre::Vector3& position, std::shared_ptr<Resources> resources);

    public:
        const auto& get_name() const
        {
            return name;
        }

    public:
        bool operator==(const Process& other) const;

    private:
        std::string name;

        std::shared_ptr<Resources> in;
        std::shared_ptr<Resources> out;

        Object* get_deposit(const Resource& in_resource, Id id, const Ogre::Vector3& position, float time) const;
        Object* get_storage(const Resource& out_resource, Id id, const Ogre::Vector3& position, float time) const;
    };
}
