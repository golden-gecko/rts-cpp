#pragma once

#include "Gecko/Cameras/ObjectFollowCamera.hpp"
#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Preview :
        public Rml::EventListener
    {
	public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

    public:
        explicit Preview();

        virtual ~Preview();

        void init_events(Rml::Element* element);

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
