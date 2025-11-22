#pragma once

namespace Gecko
{
    namespace Settings
    {
        namespace Cache
        {
            const std::string BlueprintsPath = "../cache/configurations";
            const std::string GpuPath        = "../cache/gpu_cache.bin";
            const std::string MeshesPath     = "../cache/meshes";
            const std::string ValuesPath     = "../cache/values";
        }

        namespace Camera
        {
            const float       FarClipDistance  = 1000.0f;
            const std::string MainName         = "Main";
            const float       MaxHeight        = 100.0f;
            const float       MinHeight        = 10.0f;
            const float       NearClipDistance = 1.0f;
            const std::string PolygonMode      = "solid";
            const std::string ProjectionType   = "perspective";
        }

        namespace Game
        {
            const float       DistanceComparisionMinValue = 0.01f;
            const float       FrameTime                   = 1.0f / 60.0f;
            const float       MinCollisionDistance        = 0.1f;
            const float       MinWeaponAngle              = 1.0f;
            const std::string OgrePluginsFile             = "plugins.cfg";
            const std::string OgreConfigurationFile       = "ogre.cfg";
            const std::string OgreLogFile                 = "ogre.log";
            const std::size_t OrderMaxAttemptsToComplete  = 3;
            const float       OrderWaitTime               = 3.0f;
            const float       ResourceLoadingMinDistance  = 20.0f;
            const float       ResourcePriority            = 1.0f;
            const float       ResourceTransportMinValue   = 1.0f;
            const std::string SavesPath                   = "../saves";
        }

        namespace Input
        {
            const Ogre::Vector2 MouseSensivity = Ogre::Vector2(-0.2f, -0.2f);
        }

        namespace UI
        {
            const Ogre::Vector3 CursorOffset        = Ogre::Vector3(0.0f, 0.01f, 0.0f);
            const bool          CursorVisibility    = true;
            const Ogre::Vector3 LookAtObjectOffset  = Ogre::Vector3(100.0f, 100.0f, 100.0f);
            const bool          MinimapVisibility   = true;
            const bool          PreviewVisibility   = true;
            const float         RefreshInterval     = 0.1f;
            const float         SelectionBoxMinSize = 0.0001f;
            const std::string   SelectionMesh       = "selection.mesh";
            const Ogre::Vector3 SelectionOffset     = Ogre::Vector3(0.0f, 0.1f, 0.0f);
            const Ogre::Vector3 SelectionScale      = Ogre::Vector3(1.2f, 1.0f, 1.2f);
        }

        namespace Window
        {
            const bool        Fullscreen   = false;
            const std::string MainName     = "Main";
            const int         PositionLeft = 0;
            const int         PositionTop  = 0;
            const int         SizeWidth    = 1024;
            const int         SizeHeight   = 768;
        }
    }
}
