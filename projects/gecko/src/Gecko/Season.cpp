#include "Gecko/Season.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    void Season::init()
    {
    }

    void Season::deinit()
    {
    }

    void Season::update(float time)
    {
    }

    Season::Season(const std::string& name, const ConfigurationPtr& configuration) :
        m_name(name)
    {
        m_color = configuration->get_color("color", Ogre::ColourValue::White);

        if (configuration->has_member("length"))
        {
            m_timer.deserialize(configuration->get_child("length"));
        }
    }

    void Season::apply()
    {
    }
}
