#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class PreviewWidget :
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
        explicit PreviewWidget();

        ~PreviewWidget() override;

        void zoom_in();
        void zoom_out();

    public:
        std::shared_ptr<ObjectFollowCamera> get_camera() const;

    private:
        Ogre::TexturePtr     m_texture        = nullptr;
        Ogre::RenderTexture* m_render_texture = nullptr;
        Ogre::Viewport*      m_viewport       = nullptr;

        std::string   m_texture_name = "texture_preview";
        std::uint16_t m_texture_size = 256;
    };
}
