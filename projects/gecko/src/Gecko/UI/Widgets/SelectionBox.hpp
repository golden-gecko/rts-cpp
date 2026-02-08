#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class SelectionBoxWidget :
        public Widget
    {
    public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

    public:
        explicit SelectionBoxWidget();

        ~SelectionBoxWidget() override;

    public:
        void update();

    public:
        const Ogre::Vector2& get_start() const
        {
            return m_start;
        }

        const Ogre::Vector2& get_end() const
        {
            return m_end;
        }

    public:
        bool is_valid() const;
        bool is_visible() const;

    public:
        void set_start(const Ogre::Vector2& start)
        {
            m_start = start;
        }

        void set_end(const Ogre::Vector2& end)
        {
            m_end = end;
        }

        void set_visible(bool visible)
        {
            m_scene_node->setVisible(visible);
        }

    private:
        Ogre::ManualObject* m_manual_object { nullptr };
        Ogre::SceneNode* m_scene_node { nullptr };

        Ogre::Vector2 m_start { Ogre::Vector2::ZERO };
        Ogre::Vector2 m_end { Ogre::Vector2::ZERO };
    };
}
