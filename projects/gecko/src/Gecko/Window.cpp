#include "Gecko/Window.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Input.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    Window::Window(const std::shared_ptr<Configuration>& configuration) :
        m_configuration(configuration)
    {
        m_name = configuration->get_string("name");

        auto left = configuration->get_int<int>("left", Settings::Window::PositionLeft);
        auto top = configuration->get_int<int>("top", Settings::Window::PositionTop);
        auto width = configuration->get_int<int>("width", Settings::Window::SizeWidth);
        auto height = configuration->get_int<int>("height", Settings::Window::SizeHeight);
        auto fullscreen = configuration->get_bool("fullscreen", Settings::Window::Fullscreen);

        m_render_window = Game::getSingletonPtr()->context->getRenderWindow();
    }

    Window::~Window()
    {
        on_close();
    }

    void Window::on_close()
    {
        // Ogre::Root::getSingleton().destroyRenderTarget(get_render_window());
    }

    void Window::on_resize()
    {
        unsigned int width, height;
        int top, left;

        m_render_window->getMetrics(width, height, left, top);

        m_viewport->getCamera()->setAspectRatio(
            static_cast<Ogre::Real>(m_viewport->getActualWidth()) / static_cast<Ogre::Real>(m_viewport->getActualHeight())
        );

        Input::getSingleton().set_window_size(static_cast<int>(width), static_cast<int>(height));
    }
}
