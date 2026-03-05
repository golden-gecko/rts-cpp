#include "Gecko/Item.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Item::Item(const Item& other) :
        m_configuration(other.m_configuration),
        m_id(other.m_id)
    {
    }

    Item& Item::operator=(const Item& other)
    {
        m_configuration = other.m_configuration;
        m_id = other.m_id;

        return *this;
    }

    void Item::init()
    {
    }

    void Item::deinit()
    {
    }

    ConfigurationPtr Item::serialize() const
    {
        ConfigurationPtr configuration = std::make_shared<Configuration>();

        configuration->set("configuration", m_configuration->get_name());
        configuration->set("id", m_id.get());

        return configuration;
    }

    void Item::deserialize(const ConfigurationPtr& configuration)
    {
        m_configuration = configuration;
        m_id = configuration->get_int("id", Id::Empty.get());
    }

    void Item::update(float time)
    {
    }
}
