#include "Gecko/Statistics.hpp"

Gecko::Statistics* Ogre::Singleton<Gecko::Statistics>::msSingleton = nullptr;

namespace Gecko
{
    bool Statistics::keyPressed(const OIS::KeyEvent& arg)
    {
        add("Pressed keys", 1.0f);

        return true;
    }

    bool Statistics::keyReleased(const OIS::KeyEvent& arg)
    {
        return true;
    }

    bool Statistics::mouseMoved(const OIS::MouseEvent& arg)
    {
        add("Mouse distance", std::abs(arg.state.X.rel) + std::abs(arg.state.Y.rel));

        return true;
    }

	bool Statistics::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
    {
        add("Mouse clicks", 1.0f);

        return true;
    }

	bool Statistics::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
    {
        return true;
    }

    void Statistics::add(const std::string& name, float value)
    {
        m_values[name] += value;
    }
}
