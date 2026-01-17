#pragma once

#include "Gecko/Item.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    // TODO: Add costs.
    class Skill :
        public Item
    {
    private:
        using base_type = Item;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Skill() = default;
        explicit Skill(const Skill& other);

        virtual ~Skill() = default;

    public:
        virtual void activate(const Id& id) = 0;
        virtual void activate(const Ogre::Vector3& position) = 0;

    public:
        const auto& get_name() const
        {
            return m_name;
        }

    public:
        virtual void set_owner(Object* owner)
        {
            m_owner = owner;
        }

    protected:
        Object* m_owner = nullptr;
        std::string m_name;
        Timer m_refresh_timer;
        std::uint16_t m_activation_count = 0;
    };
}
