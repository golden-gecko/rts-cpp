#pragma once

#pragma warning(push, 0)

#include <filesystem>
#include <random>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>

#include <json/json.h>

#include <Ogre.h>
#include <OgreBitesConfigDialog.h>
#include <OgreBitwise.h>
#include <OgreRectangle2D.h>
#include <OgreSGTechniqueResolverListener.h>
#include <OgreShaderGenerator.h>

#include <Bites/OgreApplicationContext.h>

#include <OIS.h>

#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Factory.h>
#include <RmlUi/Debugger.h>

#include <AppCore/App.h>
#include <AppCore/AppCore.h>
#include <AppCore/JSHelpers.h>
#include <AppCore/Window.h>
#include <JavaScriptCore/JavaScript.h>
#include <Ultralight/Ultralight.h>

#pragma warning(pop)

// TODO: Remove or add all classes.
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
    class Game;
    class Id;
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
    class Resource;
    class Resources;
    class Search;
    class Season;
    class Selected;
    class SelectionBox;
    class Skill;
    class Skills;
    class Technology;
    class Tile;
    class Window;
}
