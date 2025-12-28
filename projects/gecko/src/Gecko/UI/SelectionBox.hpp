#pragma once

#include "Gecko/Settings.hpp"

namespace Gecko
{
    class SelectionBox
    {
    public:
        explicit SelectionBox();

        virtual ~SelectionBox();

        const Ogre::Vector2& get_start() const
        {
            return start;
        }

        const Ogre::Vector2& get_end() const
        {
            return end;
        }

        bool is_valid() const
        {
            auto left = start.x;
            auto top = start.y;
            auto right = end.x;
            auto bottom = end.y;

            if (left > right)
            {
                std::swap(left, right);
            }

            if (top > bottom)
            {
                std::swap(top, bottom);
            }

            return (right - left) * (bottom - top) >= Settings::UI::SelectionBoxMinSize;
        }

        bool is_visible() const
        {
            return manual_object->isVisible();
        }

        void set_start(const Ogre::Vector2& _start)
        {
            start = _start;
        }

        void set_end(const Ogre::Vector2& _end)
        {
            end = _end;
        }

        void set_visible(bool visible)
        {
            scene_node->setVisible(visible);
        }

        void update();

    private:
        Ogre::ManualObject* manual_object { nullptr };
        Ogre::SceneNode* scene_node { nullptr };

        Ogre::Vector2 start { Ogre::Vector2::ZERO };
        Ogre::Vector2 end { Ogre::Vector2::ZERO };
    };
}
