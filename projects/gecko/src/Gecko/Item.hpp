#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Item :
        public Initializable,
        public Serializable,
        public Updatable
    {
    // From Initializable.
    public:
        void init() override;
        void deinit() override;

    // From Serializable.
    public:
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    // From Updatable.
    public:
        void update(float time) override;

    // Item.
    public:
        Item() = default;
        Item(const Item& other) = delete;

    public:
        Item& operator=(const Item& other) = delete;

    public:
        const ConfigurationPtr& get_configuration() const
        {
            return m_configuration;
        }

        const Id& get_id() const
        {
            return m_id;
        }

    public:
        void set_configuration(const ConfigurationPtr& configuration)
        {
            m_configuration = configuration;
        }

        void set_id(const Id& id)
        {
            m_id = id;
        }

    protected:
        ConfigurationPtr m_configuration;

    private:
        Id m_id;
    };
}
