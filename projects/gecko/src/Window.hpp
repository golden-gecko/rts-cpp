#pragma once

#include "Cameras/Camera.hpp"

namespace Gecko
{
    class Window
    {
    public:
        explicit Window(const std::shared_ptr<Configuration>& configuration);

        virtual ~Window();

    public:
        auto is_active() const
        {
            return m_render_window->isActive();
        }

        void on_close();
        void on_resize();

    public:
        std::uint32_t get_handle() const
        {
            std::size_t render_window_handle = 0;

            m_render_window->getCustomAttribute("WINDOW", &render_window_handle);

            return render_window_handle;
        }

        auto get_height() const
        {
            return m_render_window->getHeight();
        }

        const auto& get_name() const
        {
            return m_name;
        }

        Ogre::RenderWindow* get_render_window() const
        {
            return m_render_window;
        }

        const Ogre::RenderTarget::FrameStats& get_statistics() const
        {
            return m_render_window->getStatistics();
        }

        const auto get_viewport() const
        {
            return m_viewport;
        }

        auto get_width() const
        {
            return m_render_window->getWidth();
        }

    public:
        void set_camera(std::shared_ptr<Camera> camera)
        {
            if (m_viewport)
            {
                m_viewport->setCamera(camera->get_camera());
            }
            else
            {
                // TODO: Read color from configuration.
                m_viewport = m_render_window->addViewport(camera->get_camera());
                m_viewport->setBackgroundColour(Ogre::ColourValue(0.25f, 0.5f, 0.75f));
            }
        }

    private:
        std::shared_ptr<Configuration> m_configuration;

        std::string m_name;

        // TODO: Fix.
        // SDL_Window* m_sdl_window = nullptr;

        Ogre::RenderWindow* m_render_window = nullptr;
        Ogre::Viewport* m_viewport = nullptr;
    };
}
