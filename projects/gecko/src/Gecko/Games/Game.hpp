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

        virtual ~Game() = default;

    public:
        void load_map(const std::string& map_name);
        void load_save(const std::string& save_name);
        void quit();
        void run();
        void save();
        void stop();
        void unload_map();

        Ogre::Entity* create_entity(const std::string& name) const;
        void destroy_entity(Ogre::Entity* entity) const;

        Ogre::ManualObject* create_manual_object() const;
        void destroy_manual_object(Ogre::ManualObject* manual_object) const;

        Ogre::RaySceneQuery* create_ray_scene_query(const Ogre::Ray& ray) const;
        void destroy_ray_scene_query(Ogre::RaySceneQuery* ray_scene_query);

        Ogre::SceneNode* create_scene_node() const;
        void destroy_scene_node(Ogre::SceneNode* scene_node) const;

        void save_options(const std::string& options);
        void load_options();

    public:
        Map* get_active_map() const;
        Player* get_active_player() const;

        auto get_active_player_id() const
        {
            return m_active_player_id;
        }

        const auto& get_configuration() const
        {
            return m_configuration;
        }

        auto get_frame_number() const
        {
            return getRoot()->getNextFrameNumber() - 1;
        }

        std::vector<std::string> get_maps() const;

        const auto& get_name() const
        {
            return m_name;
        }

        std::vector<std::string> get_saves() const;

        Ogre::SceneManager* get_scene_manager() const
        {
            return m_scene_manager;
        }

    public:
        void set_active_player_id(const Id& id);

    private:
        ConfigurationPtr m_configuration;

        std::string m_name;

        Id m_active_map_id;
        Id m_active_player_id;

    private:
        Ogre::SceneManager* m_scene_manager    = nullptr;
        Ogre::Light*        m_light            = nullptr;
        Ogre::SceneNode*    m_light_scene_node = nullptr;

        MapPtr m_active_map = nullptr;

        void init_meshes();
        void init_root();
        void init_scene(const ConfigurationPtr& configuration);

        void deinit_maps();
        void deinit_root();
        void deinit_scene();
    };
}
