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
    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Item() = default;
        explicit Item(const Item& other);

        ~Item() override = default;

    public:
        Item& operator=(const Item& other);

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
