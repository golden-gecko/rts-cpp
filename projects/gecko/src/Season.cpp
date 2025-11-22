#include "Season.hpp"

#include "Configuration.hpp"
#include "Games/Game.hpp"

namespace Gecko
{
    Season::Season(const std::string& name, const Configuration& configuration) :
        m_name(name)
    {
        if (configuration.get_child_optional("color"))
        {
            m_color = configuration.get_color("color");
        }

        m_timer.set_maximal(configuration.get_float("length", 10.0f));
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
        // TODO: Fix.
        // Game::getSingleton().get_scene_manager()->setAmbientLight(color);
    }
}
