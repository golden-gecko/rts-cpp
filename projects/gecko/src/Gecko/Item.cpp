#include "Gecko/Item.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
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
        m_id = configuration->get_int("id", decltype(m_id)());
    }

    void Item::update(float time)
    {
    }
}
