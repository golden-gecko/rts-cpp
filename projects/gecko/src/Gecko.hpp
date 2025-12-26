#pragma once

#ifdef _MSC_VER
    #define IGNORE_WARNINGS_BEGIN __pragma(warning(push, 0))
    #define IGNORE_WARNINGS_END __pragma(warning(pop, 0))
#else
    #define IGNORE_WARNINGS_BEGIN _Pragma("GCC diagnostic push")
    #define IGNORE_WARNINGS_END _Pragma("GCC diagnostic pop")

    #pragma GCC system_header
#endif

IGNORE_WARNINGS_BEGIN

#include <json/json.h>

#include <Ogre.h>
#include <OgreBitesConfigDialog.h>
#include <OgreBitwise.h>
#include <OgreRectangle2D.h>
#include <OgreSGTechniqueResolverListener.h>
#include <OgreShaderGenerator.h>

#include <OIS.h>

#include <AppCore/App.h>
#include <AppCore/AppCore.h>
#include <AppCore/JSHelpers.h>
#include <AppCore/Window.h>
#include <JavaScriptCore/JavaScript.h>
#include <Ultralight/Ultralight.h>

/*
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <deque>
#include <filesystem>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

IGNORE_WARNINGS_END

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

#define L_WARNING std::cout
#define L_TRACE   std::cout
#define L_DEBUG   std::cout
#define L_INFO    std::cout
#define L_WARNING std::cout
#define L_ERROR   std::cout
#define L_FATAL   std::cout
