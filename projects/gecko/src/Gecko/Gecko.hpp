#pragma once

#pragma warning(push, 0)

// STL
#include <filesystem>
#include <random>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>

// JsonCpp
#include <json/json.h>

// Ogre
#include <Ogre.h>
#include <OgreBitesConfigDialog.h>
#include <OgreBitwise.h>
#include <OgreRectangle2D.h>
#include <OgreSGTechniqueResolverListener.h>
#include <OgreShaderGenerator.h>

#include <Bites/OgreApplicationContext.h>

// OIS
#include <OIS.h>

// RmlUi
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/EventListener.h>
#include <RmlUi/Core/EventListenerInstancer.h>
#include <RmlUi/Core/Factory.h>
#include <RmlUi/Core/RenderInterface.h>
#include <RmlUi/Core/SystemInterface.h>
#include <RmlUi/Core/Types.h>
#include <RmlUi/Debugger.h>

// Windows
#include <Windows.h>
#include <Psapi.h>

#pragma warning(pop)

// TODO: Add all classes.
namespace Gecko
{
    class Application;
    class Area;
    class Camera;
    class Component;
    class Components;
    class Configuration;
    class Configurations;
    class Cursor;
    class DataLayer;
    class Diplomacy;
    class Entrance;
    class EventListenerInstancer;
    class Game;
    class Id;
    class Initializable;
    class Layer;
    class Layers;
    class Map;
    class Mesh;
    class Minimap;
    class Object;
    class Order;
    class Orders;
    class Player;
    class PositionValidator;
    class Preview;
    class Process;
    class Processes;
    class RenderInterface;
    class Resource;
    class Resources;
    class Search;
    class Season;
    class Selected;
    class SelectionBox;
    class Serializable;
    class Skill;
    class Skills;
    class SystemInterface;
    class Technology;
    class Tile;
    class Updatable;
    class Window;

    using CameraPtr = std::shared_ptr<Camera>;
    using ConfigurationPtr = std::shared_ptr<Configuration>;
    using LayerPtr = std::shared_ptr<Layer>;
    using ResourcePtr = std::shared_ptr<Resource>;
    using SeasonPtr = std::shared_ptr<Season>;
    using TechnologyPtr = std::shared_ptr<Technology>;

    typedef std::vector<Ogre::uint32> Indices;
    typedef std::vector<std::vector<Ogre::Vector3>> Normals;
    typedef std::vector<std::vector<std::vector<Ogre::Vector2>>> TextureCoordinates;
    typedef std::vector<std::vector<Ogre::Vector3>> Vertices;

    typedef int Index;
}
