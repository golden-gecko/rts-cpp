#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Game :
        public Ogre::Singleton<Game>,
        public OgreBites::ApplicationContext,
        public Initializable,
        public Updatable
    {
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
        const ConfigurationPtr& get_configuration() const
        {
            return m_configuration;
        }

        const std::string& get_name() const
        {
            return m_name;
        }

        const Id& active_map_id() const
        {
            return m_active_map_id;
        }

        const Id& get_active_player_id() const
        {
            return m_active_player_id;
        }

        MapPtr get_active_map() const;
        PlayerPtr get_active_player() const;

        const ScenePtr& get_map_scene()
        {
            return m_map_scene;
        }

        const ScenePtr& get_editor_scene()
        {
            return m_editor_scene;
        }

        unsigned long get_frame_number() const
        {
            return getRoot()->getNextFrameNumber();
        }

        std::vector<std::string> get_maps() const;
        std::vector<std::string> get_saves() const;

    public:
        void set_active_player_id(const Id& active_player_id);

    private:
        ConfigurationPtr m_configuration;
        std::string      m_name;

        Id m_active_map_id;
        Id m_active_player_id;

        MapPtr    m_active_map    = nullptr;
        PlayerPtr m_active_player = nullptr;

        ScenePtr m_map_scene;
        ScenePtr m_editor_scene;

        void init_meshes();
        void init_render();
        void init_scenes(const ConfigurationPtr& configuration);

        void deinit_maps();
        void deinit_render();
        void deinit_scenes();
    };
}
