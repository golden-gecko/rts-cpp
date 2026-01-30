#pragma once

#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Navigation/Coordinate.hpp"

namespace Gecko
{
    struct CursorMesh
    {
        Ogre::Entity* entity = nullptr;
        Ogre::Vector3 scale = Ogre::Vector3::ZERO;
    };

    class Cursor
    {
    public:
        enum class Type
        {
            Component,
            Object,
            Square
        };

    public:
        explicit Cursor();

        virtual ~Cursor();

        void update(const std::pair<Layer*, Ogre::Vector3>& cast);

    public:
        const Navigation::Coordinate& get_position() const
        {
            return m_position;
        }

        bool is_visible() const
        {
            return m_visible;
        }

    public:
        void set_mesh(const ConfigurationPtr& configuration);
        void set_type(Type type);
        void set_visible(bool visible);

    private:
        Navigation::Coordinate m_position;

        bool m_visible = false;
        Type m_type = Type::Square;
        std::string m_mesh_name;

        Ogre::SceneNode* m_scene_node = nullptr;
        Ogre::ManualObject* m_square = nullptr;

        std::map<std::string, std::unique_ptr<Component>> m_components;
        std::map<std::string, std::unique_ptr<Object>> m_objects;
    };
}
