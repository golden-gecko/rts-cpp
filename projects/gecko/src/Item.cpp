#include "Item.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    Item::Item(const Item& other)
    {
        m_configuration = other.m_configuration;
        m_id = other.m_id;
    }

    void Item::init()
    {
    }

    void Item::deinit()
    {
    }

    std::shared_ptr<Configuration> Item::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("configuration", m_configuration->get_name());
        configuration->set("id", m_id.get());

        return configuration;
    }

    void Item::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        m_configuration = configuration;
        m_id = configuration->get_int("id", Id::Empty.get());
    }
}
