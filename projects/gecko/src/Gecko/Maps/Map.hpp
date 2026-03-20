#pragma once

#include "Gecko/Item.hpp"

namespace Gecko
{
    class Map :
        public Item
    {
    private:
        using base_type = Item;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        static MapPtr create(MapPtr memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Map(const ScenePtr& scene);
        Map(const Map& other);

    public:
        Map& operator=(const Map&) = delete;

    public:
        bool is_position_valid(const Ogre::Vector3& position) const;

        void show_data_layer(const std::string& layer_name, const std::string& data_layer_name);

    public:
        CameraPtr get_camera(const std::string& name);

        const auto& get_name() const
        {
            return m_name;
        }

        std::shared_ptr<Layer> get_layer(const std::string& name) const;

        auto& get_layers()
        {
            return m_layers;
        }

        const auto& get_layers() const
        {
            return m_layers;
        }

        ScenePtr get_scene()
        {
            return m_scene;
        }

        std::shared_ptr<Season> get_season(const std::string& name) const;

        const auto& get_seasons() const
        {
            return m_seasons;
        }

    public:
        void set_visible(bool visible);

    protected:
        ScenePtr    m_scene;
        std::string m_name;

        std::map<std::string, std::shared_ptr<Layer>>  m_layers;
        std::map<std::string, std::shared_ptr<Season>> m_seasons;

        virtual void init_layers();
        virtual void init_objects();
        virtual void init_players();
        virtual void init_seasons();

        virtual void deinit_layers();
        virtual void deinit_objects();
        virtual void deinit_players();
        virtual void deinit_seasons();
    };
}
