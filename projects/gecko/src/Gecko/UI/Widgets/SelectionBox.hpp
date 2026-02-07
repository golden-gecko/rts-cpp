#pragma once

#include "Gecko/Settings.hpp"
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

        const Ogre::Vector2& get_start() const
        {
            return m_start;
        }

        const Ogre::Vector2& get_end() const
        {
            return m_end;
        }

        bool is_valid() const
        {
            auto left = m_start.x;
            auto top = m_start.y;
            auto right = m_end.x;
            auto bottom = m_end.y;

            if (left > right)
            {
                std::swap(left, right);
            }

            if (top > bottom)
            {
                std::swap(top, bottom);
            }

            return (right - left) * (bottom - top) >= Settings::UI::SelectionBoxWidgetMinSize;
        }

        bool is_visible() const
        {
            return m_manual_object->isVisible();
        }

        void set_start(const Ogre::Vector2& _start)
        {
            m_start = _start;
        }

        void set_end(const Ogre::Vector2& _end)
        {
            m_end = _end;
        }

        void set_visible(bool visible)
        {
            m_scene_node->setVisible(visible);
        }

        void update();

    private:
        Ogre::ManualObject* m_manual_object { nullptr };
        Ogre::SceneNode* m_scene_node { nullptr };

        Ogre::Vector2 m_start { Ogre::Vector2::ZERO };
        Ogre::Vector2 m_end { Ogre::Vector2::ZERO };
    };
}
