#pragma once

#include "Gecko/Components/Mesh.hpp"

namespace Gecko
{
    class Indicator :
        public Mesh
    {
    private:
        using base_type = Mesh;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    private:
        Ogre::ManualObject* m_manual_object = nullptr;
    };
    
    class Circle :
        public Indicator
    {
    private:
        using base_type = Mesh;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    private:
        Ogre::ManualObject* m_manual_object = nullptr;
    };
    
    class Path :
        public Indicator
    {
    private:
        using base_type = Mesh;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    private:
        Ogre::ManualObject* m_manual_object = nullptr;
    };
}
