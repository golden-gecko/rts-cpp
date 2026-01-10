#pragma once

#include "Gecko/Coordinate.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"

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
        const Coordinate& get_position() const
        {
            return position;
        }

        bool is_visible() const
        {
            return visible;
        }

    public:
        void set_mesh(const ConfigurationPtr& configuration);
        void set_type(Type type);
        void set_visible(bool visible);

    private:
        Coordinate position;

        bool visible = false;
        Type type = Type::Square;
        std::string mesh_name;

        Ogre::SceneNode* scene_node = nullptr;
        Ogre::ManualObject* square = nullptr;

        std::map<std::string, std::unique_ptr<Component>> components;
        std::map<std::string, std::unique_ptr<Object>> objects;
    };
}
