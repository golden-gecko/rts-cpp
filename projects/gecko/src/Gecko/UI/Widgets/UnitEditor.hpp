#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class UnitEditorWidget :
        public Widget
    {
	public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

    public:
        // From Widget.
        void init_data_bindigs(Rml::Context* context) override;

        void init_events(Rml::ElementDocument* document) override;
        void deinit_events(Rml::ElementDocument* document) override;

    public:
        explicit UnitEditorWidget();

        ~UnitEditorWidget() override;

    public:
        void click(float x, float y);
        void move(float x, float y);
        void zoom_in();
        void zoom_out();

    private:
        Ogre::TexturePtr     m_texture        = nullptr;
        Ogre::RenderTexture* m_render_texture = nullptr;
        Ogre::Viewport*      m_viewport       = nullptr;

        std::string   m_texture_name = "texture_minimap";
        std::uint16_t m_texture_size = 256;

        float m_min_zoom = 1.0f;
        float m_max_zoom = 16.0f;

        float m_scroll_sensivity = 0.005f;
        float m_zoom_sensivity   = 2.0f;
    };
}
