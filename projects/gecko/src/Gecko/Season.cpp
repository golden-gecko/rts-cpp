#include "Gecko/Season.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Games/Game.hpp"

namespace Gecko
{
    Season::Season(const std::string& name, const Configuration& configuration) :
        m_name(name)
    {
        m_color = configuration.get_color("color", Ogre::ColourValue::White);
        m_timer.set_max(configuration.get_float("length", 0.0f));
    }

    void Season::init()
    {
    }

    void Season::deinit()
    {
    }

    void Season::update(float time)
    {
    }

    void Season::apply()
    {
    }
}
