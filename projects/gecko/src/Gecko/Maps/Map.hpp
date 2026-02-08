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
        static MapPtr create();
        static MapPtr create(const ConfigurationPtr& configuration);
        static MapPtr create(MapPtr memory);
        static MapPtr create(MapPtr memory, const ConfigurationPtr& configuration);

    public:
        explicit Map() = default;
        explicit Map(const Map& other);

        void init() override;
        void deinit() override;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

        void update(float time) override;

    public:
        bool is_position_valid(const Ogre::Vector3& position) const;

        void show_data_layer(const std::string& layer_name, const std::string& data_layer_name);

    public:
        std::shared_ptr<Camera> get_camera(const std::string& name) const
        {
            auto camera = m_cameras.find(name);

            if (camera == m_cameras.end())
            {
                return nullptr;
            }

            return camera->second;
        }

        const auto& get_cameras() const
        {
            return m_cameras;
        }

        const auto& get_name() const
        {
            return m_name;
        }

        std::shared_ptr<Layer> get_layer(const std::string& name) const;

        const auto& get_layers() const
        {
            return m_layers;
        }

        std::shared_ptr<Season> get_season(const std::string& name) const;

        const auto& get_seasons() const
        {
            return m_seasons;
        }

    public:
        void set_visible(bool visible);

    protected:
        std::string m_name;

        std::map<std::string, std::shared_ptr<Camera>> m_cameras;
        std::map<std::string, std::shared_ptr<Layer>> m_layers;
        std::map<std::string, std::shared_ptr<Season>> m_seasons;

        virtual void init_cameras();
        virtual void init_layers();
        virtual void init_objects();
        virtual void init_players();
        virtual void init_seasons();

        virtual void deinit_cameras();
        virtual void deinit_layers();
        virtual void deinit_objects();
        virtual void deinit_players();
        virtual void deinit_seasons();
    };
}
