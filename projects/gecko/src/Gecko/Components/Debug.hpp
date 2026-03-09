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
        // From Initializable.
        void init() override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        static Debug* create(Debug* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Debug() = default;
        Debug(const Debug& other);

    private:
        Ogre::ManualObject* m_manual_object = nullptr;
        Ogre::SceneNode*    m_scene_node    = nullptr;

        void draw_line(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& color);
    };
}
