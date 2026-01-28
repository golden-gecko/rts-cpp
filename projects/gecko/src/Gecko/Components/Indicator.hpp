#pragma once

#include "Gecko/Components/Mesh.hpp"

namespace Gecko
{
    class Indicator
    {
    public:
        explicit Indicator() = default;

        virtual ~Indicator() = default;

    public:
        void set_material_name(const std::string& material_name);
        void set_position(const Ogre::Vector3& position);

    protected:
        Ogre::Entity* m_entity = nullptr;
        Ogre::SceneNode* m_scene_node = nullptr;
    };

    class Cone :
        public Indicator
    {
    private:
        using base_type = Indicator;

    public:
        explicit Cone();

        ~Cone() override;
    };
}
