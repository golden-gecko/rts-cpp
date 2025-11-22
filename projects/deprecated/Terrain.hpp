#pragma once

#include "Configuration.hpp"
#include "Exception.hpp"
#include "Search.hpp"
#include "Types.hpp"
#include "Utils.hpp"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace Gecko
{
    namespace LayerType
    {
        enum class Value : std::uint8_t
        {
            Height,
            Navigation,
            None,
            Occupied,
            PollutionAir,
            PollutionGround,
            PollutionWater,
            Roads,
            TemperatureAir,
            TemperatureGround,
            TemperatureWater,
            Traffic,
            Type
        };

        static const boost::bimap<Value, std::string> type_to_string = boost::assign::list_of<boost::bimap<Value, std::string>::relation>
            TYPE_TO_STRING(Value, Height)
            TYPE_TO_STRING(Value, Navigation)
            TYPE_TO_STRING(Value, None)
            TYPE_TO_STRING(Value, Occupied)
            TYPE_TO_STRING(Value, PollutionAir)
            TYPE_TO_STRING(Value, PollutionGround)
            TYPE_TO_STRING(Value, PollutionWater)
            TYPE_TO_STRING(Value, Roads)
            TYPE_TO_STRING(Value, TemperatureAir)
            TYPE_TO_STRING(Value, TemperatureGround)
            TYPE_TO_STRING(Value, TemperatureWater)
            TYPE_TO_STRING(Value, Traffic)
            TYPE_TO_STRING(Value, Type)
        ;

        std::string to_string(Value value);

        Value from_string(const std::string& value);
    }

    enum class TerrainLayerTypeType : std::uint8_t
    {
        Grass,
        Industrial,
        None,
        Residential,
        Road
    };

    class BaseTerrainLayer
    {
    public:
        virtual ~BaseTerrainLayer()
        {
        }

        virtual void update(float time) = 0;

        virtual std::uint32_t get_size() const = 0;

        virtual const std::string& get_source() const = 0;

        template <typename Type>
        Type* as()
        {
            return dynamic_cast<Type*>(this);
        }

        template <typename Type>
        const Type* as() const
        {
            return dynamic_cast<const Type*>(this);
        }
    };

    template<typename Type>
    class TerrainLayer : public BaseTerrainLayer
    {
    public:
        TerrainLayer(const Configuration& configuration)
        {
            source = configuration.get_string("source");
            type = configuration.get_string("type");
        }

        ~TerrainLayer() override
        {
        }

        void update(float time) override
        {
        }

        virtual void load(const Configuration& configuration)
        {
            Ogre::Image image;

            image.load(source, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

            if (image.getHeight() != image.getWidth())
            {
                throw Exception("File " + source + " is not square.");
            }

            if (image.getFormat() != Ogre::PixelFormat::PF_A8B8G8R8)
            {
                throw Exception("File " + source + " is not in ABGR format.");
            }

            auto size = static_cast<std::uint32_t>(image.getHeight());
            data.resize(size, std::vector<Type>(size, Type()));

            auto row_span = image.getRowSpan();
            auto bytes_per_pixel = row_span / image.getWidth();

            for (auto z = 0; z < size; ++z)
            {
                for (auto x = 0; x < size; ++x)
                {
                    auto r = image.getData()[z * row_span + x * bytes_per_pixel    ];
                    auto g = image.getData()[z * row_span + x * bytes_per_pixel + 1];
                    auto b = image.getData()[z * row_span + x * bytes_per_pixel + 2];
                    auto a = image.getData()[z * row_span + x * bytes_per_pixel + 3];

                    data[x][z] = static_cast<Type>(b);
                }
            }
        }

        void add(std::uint32_t x, std::uint32_t z, Type value)
        {
            changed = true;

            if (x >= 0 && x < get_size() && z >= 0 & z < get_size())
            {
                data[x][z] += value;
            }
        }

        void add(const Coordinate& index, Type value)
        {
            add(index.x, index.z, value);
        }

        void add(const Rectangle& index, Type value)
        {
            for (auto z = index.start.z; z <= index.end.z; ++z)
            {
                for (auto x = index.start.x; x <= index.end.x; ++x)
                {
                    add(x, z, value);
                }
            }
        }

        void substract(std::uint32_t x, std::uint32_t z, Type value)
        {
            changed = true;

            if (x >= 0 && x < get_size() && z >= 0 & z < get_size())
            {
                data[x][z] -= value;
            }
        }

        void substract(const Coordinate& index, Type value)
        {
            substract(index.x, index.z, value);
        }

        void substract(const Rectangle& index, Type value)
        {
            for (auto z = index.start.z; z <= index.end.z; ++z)
            {
                for (auto x = index.start.x; x <= index.end.x; ++x)
                {
                    substract(x, z, value);
                }
            }
        }

        const Type& get_data(std::uint32_t x, std::uint32_t z) const
        {
            return data[x][z];
        }

        const Type& get_data(const Coordinate& index) const
        {
            return get_data(index.x, index.z);
        }

        std::uint32_t get_size() const override
        {
            return static_cast<std::uint32_t>(data.size());
        }

        const std::string& get_source() const override
        {
            return source;
        }

        const std::string& get_type() const
        {
            return type;
        }

        void set_size(std::uint32_t x, std::uint32_t z, Type value)
        {
            data[x][z] = value;
        }

    protected:
        std::string source;
        std::string type;

        std::vector<std::vector<Type>> data;

        bool changed { false };
    };

    class TerrainLayerHeight : public TerrainLayer<std::uint8_t>
    {
    public:
        TerrainLayerHeight(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }
    };

    class TerrainLayerNavigation : public TerrainLayer<std::uint8_t>
    {
    public:
        TerrainLayerNavigation(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }
    };

    class TerrainLayerNone : public TerrainLayer<std::uint8_t>
    {
    public:
        TerrainLayerNone(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }
    };

    class TerrainLayerOccupied : public TerrainLayer<std::uint8_t>
    {
    public:
        TerrainLayerOccupied(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }

        void update(float time) override;
    };

    class TerrainLayerPollutionAir : public TerrainLayer<float>
    {
    public:
        TerrainLayerPollutionAir(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }

        void update(float time) override;
    };

    class TerrainLayerPollutionGround : public TerrainLayer<float>
    {
    public:
        TerrainLayerPollutionGround(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }
    };

    class TerrainLayerPollutionWater : public TerrainLayer<float>
    {
    public:
        TerrainLayerPollutionWater(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }
    };

    class TerrainLayerType : public TerrainLayer<TerrainLayerTypeType>
    {
    public:
        TerrainLayerType(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }

        void load(const Gecko::Configuration& configuration) override
        {
            std::map<TerrainLayerTypeType, Ogre::RGBA> colors;
            std::map<Ogre::RGBA, TerrainLayerTypeType> types;

            colors[TerrainLayerTypeType::Grass] = (181 << 24) + (230 << 16) + (29 << 8) + 255;
            colors[TerrainLayerTypeType::Industrial] = (185 << 24) + (122 << 16) + (87 << 8) + 255;
            colors[TerrainLayerTypeType::Residential] = (128 << 16) + 255;
            colors[TerrainLayerTypeType::Road] = (128 << 24) + (128 << 16) + (128 << 8) + 255;

            types[colors[TerrainLayerTypeType::Grass]] = TerrainLayerTypeType::Grass;
            types[colors[TerrainLayerTypeType::Industrial]] = TerrainLayerTypeType::Industrial;
            types[colors[TerrainLayerTypeType::Residential]] = TerrainLayerTypeType::Residential;
            types[colors[TerrainLayerTypeType::Road]] = TerrainLayerTypeType::Road;

            Ogre::Image image;

            image.load(source, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

            if (image.getHeight() != image.getWidth())
            {
                throw Exception("File " + source + " is not square.");
            }

            if (image.getFormat() != Ogre::PixelFormat::PF_A8B8G8R8)
            {
                throw Exception("File " + source + " is not in ABGR format.");
            }

            auto size = static_cast<std::uint32_t>(image.getHeight());
            data.resize(size, std::vector<TerrainLayerTypeType>(size, TerrainLayerTypeType::None));

            auto row_span = image.getRowSpan();
            auto bytes_per_pixel = row_span / image.getWidth();

            for (auto z = 0; z < size; ++z)
            {
                for (auto x = 0; x < size; ++x)
                {
                    auto r = image.getData()[z * row_span + x * bytes_per_pixel    ];
                    auto g = image.getData()[z * row_span + x * bytes_per_pixel + 1];
                    auto b = image.getData()[z * row_span + x * bytes_per_pixel + 2];
                    auto a = image.getData()[z * row_span + x * bytes_per_pixel + 3];

                    Ogre::RGBA color = (r << 24) + (g << 16) + (b << 8) + 255;

                    if (types.find(color) == types.end())
                    {
                        throw Exception("Color not found.");
                    }

                    data[x][z] = types[color];
                }
            }
        }
    };

    class TerrainLayerRoads : public TerrainLayer<std::uint16_t>
    {
    public:
        TerrainLayerRoads(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }
    };

    class TerrainLayerTraffic : public TerrainLayer<std::uint16_t>
    {
    public:
        TerrainLayerTraffic(const Gecko::Configuration& configuration) : 
            TerrainLayer(configuration)
        {
        }

        void update(float time) override;
    };

    class TerrainMesh
    {
    public:
        TerrainMesh();

        virtual void init();

    // TODO:: Make private and add methods.
    public:
        std::vector<Ogre::Vector3> vertices;
        std::vector<Ogre::ColourValue> colors;
        std::vector<std::vector<Ogre::Vector2>> textureCoordinates;
        std::vector<Ogre::Vector3> normals;
        std::vector<Ogre::uint32> indices;

        Ogre::RenderOperation::OperationType operationType;

        std::string materialName;

        Ogre::Entity* entity;
        Ogre::SceneNode* scene_node;
    };

    class Terrain : public TerrainMesh
    {
    private:
        using base_type = TerrainMesh;

    public:
        Terrain(const Gecko::Configuration& configuration);

        virtual ~Terrain()
        {
        }

        void init() override
        {
            base_type::init();

            entity->getUserObjectBindings().setUserAny(Ogre::Any(this));
        }

        virtual void update(float time);

        void show_grid(bool visible);

        bool is_grid_visible() const;

        bool is_index_valid(Index x, Index z) const
        {
            if (x < 0 || x >= get_size() - 1)
            {
                return false;
            }

            if (z < 0 || z >= get_size() - 1)
            {
                return false;
            }

            return true;
        }

        bool is_index_valid(const Coordinate& coordinate) const
        {
            return is_index_valid(coordinate.x, coordinate.z);
        }

        Coordinate validate_position(const Coordinate& position) const;
        Ogre::Vector3 validate_position(const Ogre::Vector3& position) const;
        Ogre::Vector3 validate_position(const Ogre::Vector3& position, float min_height, float max_height) const;

        Ogre::Vector3 get_position(float x, float z) const;

        Coordinate get_index(const Ogre::Vector3& position) const
        {
            return Utils::get_index_from_position(position, get_scale());
        }

        const Ogre::Vector3& get_scale() const
        {
            return scale;
        }

        std::uint32_t get_size() const
        {
            return layers.at(LayerType::Value::Height)->get_size();
        }

        BaseTerrainLayer* get_layer(LayerType::Value type)
        {
            // TODO: Fix.
            return layers.at(type).get();
        }

        const BaseTerrainLayer* get_layer(LayerType::Value type) const
        {
            // TODO: Fix.
            return layers.at(type).get();
        }

        void set_layer(const std::string& layer_name)
        {
            // TODO: Check if layer exists.
            auto layer = layers.find(LayerType::from_string(layer_name));

            material->getTechnique(0)->getPass(0)->getTextureUnitState("data")->setTexture(
                Utils::load_texture(layer->second->get_source())
            );
        }

        enum class SnapType : std::uint8_t
        {
            Center,
            LeftBottom,
            LeftTop,
            RightBottom,
            RightTop
        };

        Ogre::Vector3 snap_position_to_grid(const Ogre::Vector3& position, SnapType snap_type = SnapType::Center) const;

    private:
        Ogre::Vector3 scale;
        Ogre::MaterialPtr material;

        std::unordered_map<LayerType::Value, std::unique_ptr<BaseTerrainLayer>> layers;

    public:
        Path::Points get_path(const Ogre::Vector3& from, const Ogre::Vector3& to) const;

        void set_search_strategy(Search::Type type);

    private:
        std::unique_ptr<Search> search_strategy;
    };
}
