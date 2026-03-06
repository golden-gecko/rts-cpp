#pragma once

namespace Gecko
{
    namespace Settings
    {
        namespace Cache
        {
            const std::string ConfigurationsPath = "../cache/configurations";
            const std::string MeshesPath         = "../cache/meshes";
            const std::string ValuesPath         = "../cache/values";
        }

        namespace Camera
        {
            const float       FarClipDistance  = 1000.0f;
            const std::string Main             = "Main";
            const float       MaxHeight        = 100.0f;
            const float       MinHeight        = 10.0f;
            const std::string Minimap          = "Minimap";
            const float       NearClipDistance = 1.0f;
            const std::string PolygonMode      = "solid";
            const std::string Preview          = "Preview";
            const std::string ProjectionType   = "perspective";
        }

        namespace Configuration
        {
            const std::vector<std::string> Directories = { "../components", "../games", "../maps", "../objects", "../orders", "../players", "../skills", "../technologies" };
            const std::string              Extension   = ".json";
        }

        namespace Game
        {
            const float        DistanceComparisionMinValue = 0.01f;
            const float        FrameTime                   = 1.0f / 60.0f;
            const float        MaxSpeed                    = 4.0f;
            const float        MinCollisionDistance        = 0.1f;
            const float        MinSpeed                    = 0.0f;
            const float        MinWeaponAngle              = 1.0f;
            const std::string  SavesPath                   = "../saves";
        }

        namespace Input
        {
            const Ogre::Vector2 MouseSensivity = Ogre::Vector2(-0.2f, -0.2f);
            const unsigned int  TextMin        = 32;
            const unsigned int  TextMax        = 127;
        }

        namespace Layer
        {
            const std::string Air        = "Air";
            const std::string Navigation = "Navigation";
            const std::string Terrain    = "Terrain";
            const std::string Water      = "Water";
        }

        namespace Material
        {
            const std::string Default = "white";
        }

        namespace Order
        {
            const std::uint16_t MaxAttemptsToComplete      = 3;
            const float         ResourceLoadingMinDistance = 20.0f;
            const float         ResourceTransportMinValue  = 1.0f;
            const float         WaitTime                   = 3.0f;
        }

        namespace UI
        {
            const Ogre::Vector3 CursorOffset        = Ogre::Vector3(0.0f, 0.01f, 0.0f);
            const bool          CursorVisibility    = true;
            const Ogre::Vector3 LookAtObjectOffset  = Ogre::Vector3(100.0f, 100.0f, 100.0f);
            const bool          MinimapVisibility   = true;
            const bool          PreviewVisibility   = true;
            const float         RefreshInterval     = 1.0f / 10.0f;
            const float         SelectionBoxMinSize = 0.0001f;
            const std::string   SelectionMesh       = "selection.mesh";
            const Ogre::Vector3 SelectionOffset     = Ogre::Vector3(0.0f, 0.1f, 0.0f);
            const Ogre::Vector3 SelectionScale      = Ogre::Vector3(1.2f, 1.0f, 1.2f);
        }
    }
}
