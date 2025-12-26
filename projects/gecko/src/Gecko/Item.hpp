#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    // TODO: This class probably should be removed or converted to interfaces.
    class Item :
        public Initializable,
        public Serializable,
        public Updatable
    {
    public:
        explicit Item() = default;
        explicit Item(const Item& other);

        virtual ~Item() = default;

        void init() override;
        void deinit() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        const auto& get_configuration() const
        {
            return m_configuration;
        }

        const auto& get_id() const
        {
            return m_id;
        }

    public:
        void set_configuration(const std::shared_ptr<Configuration>& configuration)
        {
            m_configuration = configuration;
        }

        void set_id(Id id)
        {
            m_id = id;
        }

    protected:
        std::shared_ptr<Configuration> m_configuration;

    private:
        Id m_id;
    };
}
