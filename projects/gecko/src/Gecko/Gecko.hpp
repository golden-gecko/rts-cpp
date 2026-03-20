#pragma once

#pragma warning(push, 0)

// STL
#include <ctime>
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

// PathFinder
#include <AStar.h>
#include <PathFinder.h>

// RmlUi
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

// Windows
#include <Windows.h>
#include <Psapi.h>

#pragma warning(pop)

// Macros.
#define TO_STRING(x) #x

// Classes.
namespace Gecko
{
    class Application;
    class Area;
    class Buff;
    class Camera;
    class Component;
    class Configuration;
    class Cursor;
    class DataLayer;
    class Entrance;
    class EventListener;
    class EventListenerInstancer;
    class Exception;
    class Factory;
    class FireMissile;
    class FreeCamera;
    class Game;
    class Id;
    class Indicator;
    class Input;
    class Item;
    class Log;
    class Map;
    class Mesh;
    class Minimap;
    class MinimapCamera;
    class ObjectFollowCamera;
    class Path;
    class Player;
    class Point;
    class PositionValidator;
    class Preview;
    class Process;
    class RenderInterface;
    class Repair;
    class Resource;
    class RoadPosition;
    class Season;
    class Skill;
    class SkillManager;
    class SquareCenterPosition;
    class Statistics;
    class System;
    class SystemInterface;
    class Technology;
    class TechnologyTree;
    class Timer;
    class TopDownCamera;
    class UI;

    // Containers
    class Components;
    class Configurations;
    class Diplomacy;
    class Layers;
    class Orders;
    class Processes;
    class Resources;
    class Selection;
    class Skills;

    // Components
    class Armour;
    class Debug;
    class Drive;
    class Hull;
    class Radar;
    class Shield;
    class Storage;
    class Weapon;

    // Interfaces
    class Initializable;
    class Serializable;
    class Updatable;

    // Managers
    class ComponentManager;
    class ConfigurationManager;
    class JobManager;
    class MapManager;
    class ObjectManager;
    class PlayerManager;

    // Layers
    class Layer;
    class SquareLayer;

    // Objects
    class Missile;
    class Object;
    class Vehicle;

    // Orders
    class Order;
    class OrderAttack;
    class OrderCreate;
    class OrderDestroy;
    class OrderFollow;
    class OrderGuard;
    class OrderLoad;
    class OrderManager;
    class OrderMove;
    class OrderPatrol;
    class OrderRally;
    class OrderStop;
    class OrderUnload;
    class OrderWait;

    // Scenes
    class EditorScene;
    class MapScene;
    class Scene;

    // Tiles
    class SquareTile;
    class Tile;

    // Widgets
    class ConfigurationsWidget;
    class ConsoleWidget;
    class CursorWidget;
    class DiplomacyWidget;
    class GameMenuWidget;
    class InfoWidget;
    class LogWidget;
    class MinimapWidget;
    class OrdersWidget;
    class PlayersWidget;
    class PreviewWidget;
    class ResourcesWidget;
    class SelectionBoxWidget;
    class SkillsWidget;
    class StatisticsWidget;
    class TechonologiesWidget;
    class Widget;

    // Pointers
    using ComponentPtr = Component*;
    using CameraPtr = std::shared_ptr<Camera>;
    using ComponentsPtr = std::shared_ptr<Components>;
    using ConfigurationPtr = std::shared_ptr<Configuration>;
    using ConfigurationsPtr = std::shared_ptr<Configurations>;
    using DataLayerPtr = std::shared_ptr<DataLayer>;
    using LayerPtr = std::shared_ptr<Layer>;
    using LayersPtr = std::shared_ptr<Layers>;
    using MapPtr = Map*;
    using ObjectPtr = Object*;
    using OrderPtr = Order*;
    using OrdersPtr  = std::shared_ptr<Orders>;
    using PlayerPtr = Player*;
    using ProcessesPtr = std::shared_ptr<Processes>;
    using ResourcePtr = std::shared_ptr<Resource>;
    using ResourcesPtr = std::shared_ptr<Resources>;
    using ScenePtr = std::shared_ptr<Scene>;
    using SeasonPtr = std::shared_ptr<Season>;
    using SelectionPtr = std::shared_ptr<Selection>;
    using SkillPtr = Skill*;
    using SkillsPtr = std::shared_ptr<Skills>;
    using TechnologyPtr = std::shared_ptr<Technology>;

    // Types
    using Index = std::uint32_t;
    using Indices = std::vector<Ogre::uint32>;
    using Normals = std::vector<std::vector<Ogre::Vector3>>;
    using TextureCoordinates = std::vector<std::vector<std::vector<Ogre::Vector2>>>;
    using Vertices = std::vector<std::vector<Ogre::Vector3>>;
}
