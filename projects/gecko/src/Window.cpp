#include "Window.hpp"

#include "Configuration.hpp"
#include "Exception.hpp"
#include "Input.hpp"
#include "Settings.hpp"

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

        if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
        {
            if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
            {
                throw Exception("SDL_InitSubSystem() failed.");
            }
        }

        Uint32 flags = SDL_WINDOW_SHOWN;

        if (fullscreen)
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
        else
        {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        m_sdl_window = SDL_CreateWindow(m_name.c_str(), left, top, width, height, flags);

        SDL_SetWindowData(m_sdl_window, "window", this);

        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);

        if (SDL_GetWindowWMInfo(m_sdl_window, &wmInfo) == SDL_FALSE)
        {
            throw Exception("SDL_GetWindowWMInfo() failed.");
        }

        Ogre::NameValuePairList params =
        {
            { "externalWindowHandle", Ogre::StringConverter::toString(reinterpret_cast<std::size_t>(wmInfo.info.win.window)) }
        };

        m_render_window = Ogre::Root::getSingleton().createRenderWindow(m_name, width, height, fullscreen, &params);
    }

    Window::~Window()
    {
        on_close();
    }

    void Window::on_close()
    {
        Ogre::Root::getSingleton().destroyRenderTarget(get_render_window());

        SDL_DestroyWindow(m_sdl_window);
    }

    void Window::on_resize()
    {
        unsigned int width, height, depth;
        int top, left;

        m_render_window->getMetrics(width, height, depth, left, top);

        m_viewport->getCamera()->setAspectRatio(
            static_cast<Ogre::Real>(m_viewport->getActualWidth()) /
            static_cast<Ogre::Real>(m_viewport->getActualHeight())
        );

        Input::getSingleton().set_window_size(width, height);
    }
}
