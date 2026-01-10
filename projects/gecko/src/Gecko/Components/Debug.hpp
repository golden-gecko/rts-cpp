#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Debug :
        public Component
    {
    private:
        using base_type = Component;

    public:
        static Debug* create(Debug* memory, const ConfigurationPtr& configuration);

    public:
        explicit Debug();
        explicit Debug(const Debug& other);

        void init() override;

        void update(float time) override;

    private:
        Ogre::ManualObject* m_manual_object = nullptr;
        Ogre::SceneNode* m_scene_node = nullptr;

        void draw_line(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& color);
    };
}
