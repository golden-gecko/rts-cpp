#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class SelectionBoxWidget :
        public Widget
    {
    // SelectionBoxWidget.
    public:
        explicit SelectionBoxWidget(const ScenePtr& scene);

        ~SelectionBoxWidget() override;

    public:
        void update();

    public:
        bool is_valid() const;
        bool is_visible() const;

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
        void set_start(const Ogre::Vector2& start)
        {
            m_start = start;
        }

        void set_end(const Ogre::Vector2& end)
        {
            m_end = end;
        }

        void set_color(const std::string& color);
        void set_visible(bool visible);

    private:
        ScenePtr m_scene;

        Ogre::ManualObject* m_manual_object = nullptr;
        Ogre::SceneNode*    m_scene_node    = nullptr;

        Ogre::Vector2 m_start = Ogre::Vector2::ZERO;
        Ogre::Vector2 m_end   = Ogre::Vector2::ZERO;
    };
}
