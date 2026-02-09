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

// RmlUi
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

// Windows
#include <Windows.h>
#include <Psapi.h>

#pragma warning(pop)

namespace Gecko
{
    class Application;
    class Area;
    class Buff;
    class Camera;
    class Component;
    class ComponentManager;
    class Components;
    class Configuration;
    class ConfigurationManager;
    class Configurations;
    class Cursor;
    class DataLayer;
    class Diplomacy;
    class Entrance;
    class EventListener;
    class EventListenerInstancer;
    class Exception;
    class Factory;
    class FireMissile;
    class FreeCamera;
    class Game;
    class HexCenterPosition;
    class HexLayer;
    class HexTile;
    class Id;
    class Indicator;
    class Initializable;
    class Input;
    class Item;
    class JobManager;
    class Layer;
    class Layers;
    class Log;
    class Map;
    class MapManager;
    class Mesh;
    class Minimap;
    class MinimapCamera;
    class ObjectFollowCamera;
    class ObjectManager;
    class Path;
    class Player;
    class PlayerManager;
    class Point;
    class PositionValidator;
    class Preview;
    class Process;
    class Processes;
    class RenderInterface;
    class Repair;
    class Resource;
    class Resources;
    class RoadPosition;
    class Season;
    class Selected;
    class Serializable;
    class Skill;
    class SkillManager;
    class Skills;
    class SquareCenterPosition;
    class SquareLayer;
    class SquareTile;
    class Statistics;
    class System;
    class SystemInterface;
    class Technology;
    class TechnologyTree;
    class Tile;
    class Timer;
    class TopDownCamera;
    class UI;
    class Updatable;

    // Components
    class Armour;
    class Debug;
    class Drive;
    class Hull;
    class Radar;
    class Shield;
    class Storage;
    class Weapon;

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
    class Orders;
    class OrderStop;
    class OrderUnload;
    class OrderWait;

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
    using CameraPtr = std::shared_ptr<Camera>;
    using ConfigurationPtr = std::shared_ptr<Configuration>;
    using DataLayerPtr = std::shared_ptr<DataLayer>;
    using LayerPtr = std::shared_ptr<Layer>;
    using MapPtr = Map*;
    using ObjectPtr = Object*;
    using OrderPtr = Order*;
    using PlayerPtr = Player*;
    using ResourcePtr = std::shared_ptr<Resource>;
    using ResourcesPtr = std::shared_ptr<Resources>;
    using SeasonPtr = std::shared_ptr<Season>;
    using TechnologyPtr = std::shared_ptr<Technology>;

    // Types
    typedef std::vector<Ogre::uint32> Indices;
    typedef std::vector<std::vector<Ogre::Vector3>> Normals;
    typedef std::vector<std::vector<std::vector<Ogre::Vector2>>> TextureCoordinates;
    typedef std::vector<std::vector<Ogre::Vector3>> Vertices;

    typedef std::uint32_t Index;
}
