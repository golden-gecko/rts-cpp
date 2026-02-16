#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Game :
        public Ogre::Singleton<Game>,
        public Ogre::RenderQueueListener,
        public OgreBites::ApplicationContext,
        public Initializable,
        public Updatable
    {
    public:
        // From Ogre::RenderQueueListener.
        void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation) override;

    public:
        // From OgreBites::ApplicationContext.
        void windowResized(Ogre::RenderWindow* rw) override;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Game(const ConfigurationPtr& configuration);

        ~Game() override = default;

    public:
        Ogre::SceneManager* create_scene_manager() const;
        void destroy_scene_manager(Ogre::SceneManager* scene_manager) const;

        void load_map(const std::string& map_name);
        void load_save(const std::string& save_name);
        void quit();
        void run();
        void save();
        void stop();
        void unload_map();

        void save_options(const std::string& options);
        void load_options();

    public:
        MapPtr get_active_map() const;
        PlayerPtr get_active_player() const;

        auto get_active_player_id() const
        {
            return m_active_player_id;
        }

        const auto& get_configuration() const
        {
            return m_configuration;
        }

        unsigned long get_frame_number() const
        {
            return getRoot()->getNextFrameNumber();
        }

        std::vector<std::string> get_maps() const;

        const auto& get_name() const
        {
            return m_name;
        }

        std::vector<std::string> get_saves() const;

        const ScenePtr& get_map_scene()
        {
            return m_map_scene;
        }

    public:
        void set_active_player_id(const Id& id);

    private:
        ConfigurationPtr m_configuration;

        std::string m_name;

        Id m_active_map_id;
        Id m_active_player_id;

        MapPtr    m_active_map    = nullptr;
        PlayerPtr m_active_player = nullptr;

    private:
        ScenePtr m_map_scene;
        ScenePtr m_editor_scene;

        void init_meshes();
        void init_root();
        void init_scenes(const ConfigurationPtr& configuration);

        void deinit_maps();
        void deinit_root();
        void deinit_scenes();
    };
}
