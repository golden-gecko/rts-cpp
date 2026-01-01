#include "Gecko/UI/UI.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Containers/Selected.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Input.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/System.hpp"
#include "Gecko/Technologies/TechnologyTree.hpp"
#include "Gecko/UI/Cursor.hpp"
#include "Gecko/UI/Minimap.hpp"
#include "Gecko/UI/Preview.hpp"
#include "Gecko/UI/SelectionBox.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Window.hpp"

Gecko::UI* Ogre::Singleton<Gecko::UI>::msSingleton = nullptr;

namespace Gecko
{
    RenderInterface::RenderInterface(unsigned int window_width, unsigned int window_height)
    {
        mRenderSystem = Ogre::Root::getSingletonPtr()->getRenderSystem();

        mColourBlendMode.blendType = Ogre::LBT_COLOUR;
        mColourBlendMode.source1 = Ogre::LBS_DIFFUSE;
        mColourBlendMode.source2 = Ogre::LBS_TEXTURE;
        mColourBlendMode.operation = Ogre::LBX_MODULATE;

        mAlphaBlendMode.blendType = Ogre::LBT_ALPHA;
        mAlphaBlendMode.source1 = Ogre::LBS_DIFFUSE;
        mAlphaBlendMode.source2 = Ogre::LBS_TEXTURE;
        mAlphaBlendMode.operation = Ogre::LBX_MODULATE;

        mScissorEnable = false;

        mScissorRect[0] = 0;
        mScissorRect[1] = 0;
        mScissorRect[2] = window_width;
        mScissorRect[3] = window_height;

        mGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME;
    }

    Rml::CompiledGeometryHandle RenderInterface::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices)
    {
        RocketCompiledGeometry* geometry = new RocketCompiledGeometry();
        // geometry->mTexture = (texture == NULL) ? NULL : (RocketTexture*)texture;

        // Add vertex buffer
        geometry->mRenderOperation.vertexData = new Ogre::VertexData();
        geometry->mRenderOperation.vertexData->vertexStart = 0;
        geometry->mRenderOperation.vertexData->vertexCount = vertices.size();

        // Add index buffer
        geometry->mRenderOperation.indexData = new Ogre::IndexData();
        geometry->mRenderOperation.indexData->indexStart = 0;
        geometry->mRenderOperation.indexData->indexCount = indices.size();

        geometry->mRenderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

        // Set up the vertex declaration.
        Ogre::VertexDeclaration* vertex_declaration = geometry->mRenderOperation.vertexData->vertexDeclaration;
        size_t element_offset = 0;
        vertex_declaration->addElement(0, element_offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        element_offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        vertex_declaration->addElement(0, element_offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
        element_offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
        vertex_declaration->addElement(0, element_offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

        // Create the vertex buffer.
        Ogre::HardwareVertexBufferSharedPtr vertex_buffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertex_declaration->getVertexSize(0), vertices.size(), Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        geometry->mRenderOperation.vertexData->vertexBufferBinding->setBinding(0, vertex_buffer);

        // Fill the vertex buffer.
        RocketVertex* ogre_vertices = (RocketVertex*)vertex_buffer->lock(0, vertex_buffer->getSizeInBytes(), Ogre::HardwareBuffer::HBL_NORMAL);

        for (int i = 0; i < vertices.size(); ++i)
        {
            ogre_vertices[i].x = vertices[i].position.x;
            ogre_vertices[i].y = vertices[i].position.y;
            ogre_vertices[i].z = 0;

            Ogre::ColourValue diffuse(vertices[i].colour.red / 255.0f, vertices[i].colour.green / 255.0f, vertices[i].colour.blue / 255.0f, vertices[i].colour.alpha / 255.0f);
            mRenderSystem->convertColourValue(diffuse, &ogre_vertices[i].diffuse);

            ogre_vertices[i].u = vertices[i].tex_coord[0];
            ogre_vertices[i].v = vertices[i].tex_coord[1];
        }
        vertex_buffer->unlock();

        // Create the index buffer.
        Ogre::HardwareIndexBufferSharedPtr index_buffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_32BIT, indices.size(), Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        geometry->mRenderOperation.indexData->indexBuffer = index_buffer;
        geometry->mRenderOperation.useIndexes = true;

        // Fill the index buffer.
        void* ogre_indices = index_buffer->lock(0, index_buffer->getSizeInBytes(), Ogre::HardwareBuffer::HBL_NORMAL);
        memcpy(ogre_indices, indices.data(), sizeof(unsigned int) * indices.size());
        index_buffer->unlock();

        return reinterpret_cast<Rml::CompiledGeometryHandle>(geometry);
    }

    void RenderInterface::RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture)
    {
        Ogre::Matrix4 transform;
        transform.makeTrans(translation.x, translation.y, 0);
        mRenderSystem->_setWorldMatrix(transform);
        RocketCompiledGeometry* ogre3d_geometry = reinterpret_cast<RocketCompiledGeometry*>(geometry);

        //*
        auto ogre_resource = Ogre::TextureManager::getSingleton().getByHandle(texture);

        if (ogre_resource.isNull())
        {
            mRenderSystem->_disableTextureUnit(0);
        }
        else
        {
            auto ogre_texture = static_pointer_cast<Ogre::Texture>(ogre_resource);

            mRenderSystem->_setTexture(0, true, ogre_texture);
            mRenderSystem->_setTextureBlendMode(0, mColourBlendMode);
            mRenderSystem->_setTextureBlendMode(0, mAlphaBlendMode);
        }

        mRenderSystem->_render(ogre3d_geometry->mRenderOperation);
        //*/
    }

    void RenderInterface::ReleaseGeometry(Rml::CompiledGeometryHandle geometry)
    {
        int i = 0;

        /*
        RocketCompiledGeometry* ogre3d_geometry = reinterpret_cast<RocketCompiledGeometry*>(geometry);
        delete ogre3d_geometry->mRenderOperation.vertexData;
        delete ogre3d_geometry->mRenderOperation.indexData;
        delete ogre3d_geometry;
        */
    }

    Rml::TextureHandle RenderInterface::LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source)
    {
        std::filesystem::path path(source);
        std::string filename = path.filename().string();

        Ogre::TextureManager* texture_manager = Ogre::TextureManager::getSingletonPtr();

        /*
        // Get the file name
        Rocket::Core::String::size_type lc = source.RFind("/");
        Rocket::Core::String::size_type lc_win = source.RFind("\\");
        if (lc_win != Rocket::Core::String::npos && (lc == Rocket::Core::String::npos || lc < lc_win))
            lc = lc_win;
        Ogre::String file = (lc != Rocket::Core::String::npos) ? source.Substring(lc + 1).CString() : source.CString();

        // Try to find resource group
        Ogre::String group = Ogre::ResourceGroupManager::getSingletonPtr()->findGroupContainingResource(file);

        // If mGroup is set to autodetect use the resource group of the given texture as default group
        if (mGroup == Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME && !group.empty()) mGroup = group;
        */

        // Try to get loaded texture
        Ogre::TexturePtr ogre_texture = texture_manager->getByName(filename);

        // Try to load texture if necessary
        if (ogre_texture.isNull())
        {
            ogre_texture = texture_manager->load(filename, "General", Ogre::TEX_TYPE_2D, 0);
        }

        // Error
        if (ogre_texture.isNull())
        {
            return 0;
        }

        // Texture size
        texture_dimensions.x = ogre_texture->getWidth();
        texture_dimensions.y = ogre_texture->getHeight();

        // Create handle for the texture
        // texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(new RocketTexture(ogre_texture));

        return ogre_texture->getHandle(); // reinterpret_cast<Rml::TextureHandle>(ogre_texture.get());
    }

    Rml::TextureHandle RenderInterface::GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions)
    {
        static int texture_id = 1;
        std::string texture_name = std::format("generated_texture_{}", texture_id++);

        // Create a memory file
        Ogre::DataStreamPtr dataStream(new Ogre::MemoryDataStream((void*)source.data(), source_dimensions.x * source_dimensions.y * sizeof(unsigned int)));

        // Try to create texture from memory
        Ogre::TexturePtr ogre_texture = Ogre::TextureManager::getSingleton().loadRawData(
            texture_name,
            mGroup,
            dataStream,
            source_dimensions.x,
            source_dimensions.y,
            Ogre::PF_A8B8G8R8,
            Ogre::TEX_TYPE_2D,
            0);

        // Error
        if (ogre_texture.isNull())
        {
            return 0;
        }

        return ogre_texture->getHandle(); // reinterpret_cast<Rml::TextureHandle>(ogre_texture.get());
    }

    void RenderInterface::ReleaseTexture(Rml::TextureHandle texture)
    {
        int i = 0;

        // delete ((RocketTexture*)texture);
    }

    void RenderInterface::EnableScissorRegion(bool enable)
    {
        mScissorEnable = enable;

        if (!mScissorEnable)
            mRenderSystem->setScissorTest(false);
        else
            mRenderSystem->setScissorTest(true, mScissorRect[0], mScissorRect[1], mScissorRect[2], mScissorRect[3]);
    }

    void RenderInterface::SetScissorRegion(Rml::Rectanglei region)
    {
        mScissorRect[0] = std::max<int>(0, region.Position().x);
        mScissorRect[1] = std::max<int>(0, region.Position().y);

        /*
        mScissorRect[2] = x + width;
        mScissorRect[3] = y + height;

        if (mScissorEnable)
            mRenderSystem->setScissorTest(true, mScissorRect[0], mScissorRect[1], mScissorRect[2], mScissorRect[3]);
        */
    }








    UI::UI(const std::shared_ptr<Configuration>& configuration) :
        configuration(configuration)
    {
    }

    UI::~UI()
    {
    }

    void UI::init()
    {
        // TODO: Get window size.
        render_interface = std::make_shared<RenderInterface>(1920, 1200);
        system_interface = std::make_shared<SystemInterface>();

        Rml::SetRenderInterface(render_interface.get());
        Rml::SetSystemInterface(system_interface.get());

        Rml::Initialise();

        // TODO: Get window size.
        context = Rml::CreateContext("main", Rml::Vector2i(1920, 1200));

        // TODO: Move to configuration.
        if (true)
        {
            Rml::Debugger::Initialise(context);
        }

        // Load fonts.
        init_fonts();

        // Setup events.
        // DemoEventListenerInstancer event_listener_instancer{ &demo_window };
        // Rml::Factory::RegisterEventListenerInstancer(&event_listener_instancer);

        // demo_window.GetDocument()->AddEventListener(Rml::EventId::Keydown, &demo_window);
        // demo_window.GetDocument()->AddEventListener(Rml::EventId::Keyup, &demo_window);
        // demo_window.GetDocument()->AddEventListener(Rml::EventId::Animationend, &demo_window);

        // Load cursor.
        /*
        Rml::ElementDocument* cursor = context->LoadMouseCursor(getResourceFullPath("cursor.rml").c_str());

        if (cursor)
        {
            // cursor->RemoveReference();
        }
        */

        init_components();
        init_documents();
        // init_events();
        init_visibility_types();
    }

    void UI::deinit()
    {
        cursor.reset();
        minimap.reset();
        preview.reset();
        selection_box.reset();

        Rml::Shutdown();
    }

    void UI::render(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation)
    {
        if (queueGroupId != Ogre::RENDER_QUEUE_OVERLAY)
        {
            return;
        }

        if (Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled() == false)
        {
            return;
        }

        Ogre::RenderSystem* render_system = Game::getSingleton().get_root()->getRenderSystem();

        if (render_system == nullptr)
        {
            return;
        }

        auto window = Game::getSingleton().get_window(Gecko::Settings::Window::MainName);

        if (window == nullptr)
        {
            return;
        }

        auto render_window = window->get_render_window();

        if (render_window == nullptr)
        {
            return;
        }

        context->Update();

        // Set up the projection and view matrices.
        float z_near = -1;
        float z_far = 1;

        Ogre::Matrix4 projection_matrix = Ogre::Matrix4::ZERO;

        projection_matrix[0][0] = 2.0f / (Ogre::Real)render_window->getWidth();
        projection_matrix[0][3] = -1.0000000f;
        projection_matrix[1][1] = -2.0f / (Ogre::Real)render_window->getHeight();
        projection_matrix[1][3] = 1.0000000f;
        projection_matrix[2][2] = -2.0f / (z_far - z_near);
        projection_matrix[3][3] = 1.0000000f;
        
        render_system->_setProjectionMatrix(projection_matrix);
        render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);

        render_system->setLightingEnabled(false);
        render_system->_setDepthBufferParams(false, false);
        render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
        render_system->_setFog(Ogre::FOG_NONE);
        render_system->_setColourBufferWriteEnabled(true, true, true, true);
        render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
        render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

        // TODO: Investigate.
        /*
        Ogre::TextureUnitState::UVWAddressingMode addressing_mode;

        addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
        addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
        addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;

        render_system->_setTextureAddressingMode(0, addressing_mode);
        */

        render_system->_setTextureCoordSet(0, 0);
        render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);

        // TODO: Investigate.
        // render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);

        render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
        render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
        render_system->_disableTextureUnitsFrom(1);
        render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
        render_system->_setDepthBias(0, 0);

        context->Render();
    }

    void UI::update(float time)
    {
        if (refresh_time.update(time))
        {
            refresh_time.reset();

            auto hovered_object = ObjectManager::getSingleton().get(hovered_object_id);

            if (hovered_object)
            {
                set_info(hovered_object->serialize());
            }
            else if (Game::getSingleton().get_active_player() && Game::getSingleton().get_active_player()->get_selected()->size())
            {
                // TODO: Optimize. Get only first.
                for (const auto& id : *(Game::getSingleton().get_active_player()->get_selected()))
                {
                    auto object = ObjectManager::getSingleton().get(id);

                    if (object)
                    {
                        set_info(object->serialize());

                        break;
                    }
                }
            }
            else
            {
                auto memory = System::get_memory_usage();
                auto& game = Game::getSingleton();

                Json::Value info;

                if (game.get_windows().empty() == false)
                {
                    const auto& window_statistics = game.get_windows().begin()->second->get_statistics();

                    info["Average FPS"] = Utils::Convert::to_string(window_statistics.avgFPS, 2);
                    info["Last FPS"] = Utils::Convert::to_string(window_statistics.lastFPS, 2);
                    info["Triangles"] = window_statistics.triangleCount;
                }

                info["Active player ID"] = Game::getSingleton().get_active_player_id();
                info["Cursor"] = get_cursor().get_position().to_string();
                info["Maps"] = MapManager::getSingleton().size();
                info["Name"] = Game::getSingleton().get_name();
                info["Objects"] = ObjectManager::getSingleton().size();
                info["Orders"] = OrderManager::getSingleton().size();
                info["Players"] = PlayerManager::getSingleton().size();

                if (Game::getSingleton().get_active_player())
                {
                    info["Active player name"] = Game::getSingleton().get_active_player()->get_name();
                    info["Selected"] = Game::getSingleton().get_active_player()->get_selected()->size();
                }

                info["Frame number"] = Utils::Convert::to_string(Game::getSingleton().get_frame_number());
                info["CPU"] = Utils::Convert::to_string(System::get_cpu_usage()) + "%";
                info["Memory"] = Utils::Convert::to_string(static_cast<float>(memory.virtual_memory) / 1024.0f / 1024.0f) + " MB";

                /*
                const auto& seasons = MapManager::getSingleton().get_items().begin()->second->get_seasons();

                for (const auto& i : seasons)
                {
                    info.emplace(i.get_name(), i.get_current().get_name());
                }
                */

                set_info(std::make_shared<Configuration>(info));

                std::map<std::string, std::string> statistics;

                for (const auto& i : Statistics::getSingleton())
                {
                    statistics.emplace(i.first, Utils::Convert::to_string(i.second));
                }

                set_statistics(statistics);
            }

            // TODO: Optimize.
            // set_floating_descriptions(objects);
            auto& game = Game::getSingleton();

            // TODO: First map.
            set_layers(MapManager::getSingleton().begin()->second->get_layers());
            set_maps(game.get_maps());
            set_objects_admin();
            set_orders_admin();
            set_players();

            if (Game::getSingleton().get_active_player())
            {
                set_resources(Game::getSingleton().get_active_player()->get_resources());
            }

            set_saves(Game::getSingleton().get_saves());

            get_minimap().update();
            get_preview().update();
        }
    }

    void UI::change_visibility(const std::string& type, bool visible)
    {
        /*
        auto objects = ObjectManager::getSingleton().get_by_configuration_name(type);

        for (const auto& [id, object] : objects)
        {
        object->set_visible(visible);
        }
        */
    }

    /*
    void UI::engine_application_save_options(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            Game::getSingleton().save_options(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_application_quit(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        Game::getSingleton().shutdown();
    }

    void UI::engine_game_new(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            Game::getSingleton().load_map(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_game_load(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsNumber() && args[1].IsString())
        {
            Game::getSingleton().load_save(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_game_save(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        Game::getSingleton().save();
    }

    void UI::engine_game_quit(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        Game::getSingleton().quit();
    }

    void UI::engine_map_set_data_layer(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 2 && args[0].IsString() && args[1].IsString())
        {
            // TODO: First map.
            MapManager::getSingleton().begin()->second->show_data_layer(
                Utils::Convert::to_string(args[0]), Utils::Convert::to_string(args[1])
            );
        }
    }

    void UI::engine_minimap_click(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 2 && args[0].IsNumber() && args[1].IsNumber())
        {
            get_minimap().click(args[0].ToInteger(), args[1].ToInteger());
        }
    }

    void UI::engine_minimap_move(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 2 && args[0].IsNumber() && args[1].IsNumber())
        {
            get_minimap().move(args[0].ToInteger(), args[1].ToInteger());
        }
    }

    void UI::engine_minimap_zoom_in(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        get_minimap().zoom_in();
    }

    void UI::engine_minimap_zoom_out(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        get_minimap().zoom_out();
    }

    void UI::engine_preview_hide(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        get_preview().set_visible(false);
    }

    void UI::engine_preview_set_position(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 3 && args[0].IsNumber() && args[1].IsNumber() && args[2].IsNumber())
        {
            get_preview().set_position(args[0].ToInteger(), args[1].ToInteger(), args[2].ToInteger());
        }
    }

    void UI::engine_preview_show(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        get_preview().set_visible(true);
    }

    void UI::engine_technology_research(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            TechnologyTree::getSingleton().research(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_ui_change_visibility(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 2 && args[0].IsString() && args[1].IsBoolean())
        {
            change_visibility(Utils::Convert::to_string(args[0]), args[1].ToBoolean());
        }
    }

    void UI::engine_ui_look_at_object(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsNumber())
        {
            auto object_id = args[0].ToInteger();
            auto object = ObjectManager::getSingleton().get(object_id);

            if (object)
            {
                auto map = MapManager::getSingleton().begin()->second;

                Game::getSingleton().get_active_player()->get_selected()->select(object_id);

                auto camera = map->get_camera(Settings::Camera::MainName)->get_camera_node();

                camera->setPosition(object->get_position() + Settings::UI::LookAtObjectOffset);
                camera->lookAt(object->get_position(), Ogre::Node::TransformSpace::TS_PARENT);
            }
        }
    }

    void UI::engine_ui_select_object(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsNumber())
        {
            Game::getSingleton().get_active_player()->get_selected()->select(args[0].ToNumber());
        }
    }

    void UI::engine_ui_set_configuration(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            set_configuration_name(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_ui_set_order(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        static auto destroy_order = []()
        {
            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    auto order = OrderManager::getSingleton().order_destroy(Id::Empty, object_id);

                    // TODO: Throw exception.

                    if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
                    {
                        object->get_orders()->remove_all_orders();
                    }

                    object->get_orders()->add_last(order->get_id());
                }
            }
        };

        static auto disable_order = []()
        {
            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
                    {
                        object->get_orders()->remove_all_orders();
                    }

                    auto order = OrderManager::getSingleton().order_disable(Id::Empty, object_id);

                    // TODO: Throw exception.

                    object->get_orders()->add_last(order->get_id());
                }
            }
        };

        static auto enable_order = []()
        {
            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
                    {
                        object->get_orders()->remove_all_orders();
                    }

                    auto order = OrderManager::getSingleton().order_enable(Id::Empty, object_id);

                    // TODO: Throw exception.

                    object->get_orders()->add_last(order->get_id());
                }
            }
        };

        static auto stop_order = []()
        {
            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
                    {
                        object->get_orders()->remove_all_orders();
                    }

                    auto order = OrderManager::getSingleton().order_stop(Id::Empty, object_id);

                    // TODO: Throw exception.

                    object->get_orders()->add_last(order->get_id());
                }
            }
        };

        static const std::map<std::string, std::function<void()>> functions =
        {
            { "Destroy", std::bind(destroy_order) },
            { "Disable", std::bind(disable_order) },
            { "Enable", std::bind(enable_order) },
            { "Stop", std::bind(stop_order) }
        };

        if (args.size() == 1 && args[0].IsString())
        {
            auto order = Utils::Convert::to_string(args[0]);
            auto function = functions.find(order);

            if (function != functions.end())
            {
                function->second();
            }
            else
            {
                set_order_name(order_type::from_string(order));
            }
        }
    }

    void UI::engine_ui_set_skill(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            set_skill_name(Utils::Convert::to_string(args[0]));
        }
    }
    */

    void UI::inject_key_press(char key_code)
    {
        // TODO: Fill the last argument.
        context->ProcessKeyDown(Utils::Convert::to_rmlui_key(key_code), 0);

        switch (Utils::Convert::to_rmlui_key(key_code))
        {
            case Rml::Input::KeyIdentifier::KI_F5:
                init_documents();
                break;

            case Rml::Input::KeyIdentifier::KI_F8:
                Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
                break;
        }
    }

    void UI::inject_key_release(char key_code)
    {
        // TODO: Fill the last argument.
        context->ProcessKeyUp(Utils::Convert::to_rmlui_key(key_code), 0);
    }

    void UI::inject_mouse_move(std::size_t x, std::size_t y)
    {
        // TODO: Convert types and fill the last argument.
        context->ProcessMouseMove(x, y, 0);
    }

    void UI::inject_mouse_press(std::size_t x, std::size_t y, OIS::MouseButtonID id)
    {
        // TODO: Fill the last argument.
        context->ProcessMouseButtonDown(Utils::Convert::to_rmlui_button(id), 0);
    }

    void UI::inject_mouse_release(std::size_t x, std::size_t y, OIS::MouseButtonID id)
    {
        // TODO: Fill the last argument.
        context->ProcessMouseButtonUp(Utils::Convert::to_rmlui_button(id), 0);
    }

    bool UI::is_mouse_inside(std::size_t x, std::size_t y)
    {
        return true;

        /*
        static std::stringstream stream;

        stream.str("");
        stream << "app.ui.is_mouse_inside(";
        stream << x;
        stream << ", ";
        stream << y;
        stream << ")";

        auto value = view->EvaluateScript(stream.str().c_str());

        return Utils::Convert::to_string(value) == "true";
        */
    }

    void UI::log_error(const std::string& text, Id id)
    {
        log_write(text, "error", id);
    }

    void UI::log_info(const std::string& text, Id id)
    {
        log_write(text, "info", id);
    }

    void UI::reset()
    {
        reset_configuration();
        reset_order();
        reset_skill();
    }

    void UI::reset_configuration()
    {
        if (get_configuration_name() != "none")
        {
            set_configuration_name("none");
        }
    }

    void UI::reset_order()
    {
        if (get_order_name() != order_type::Value::None)
        {
            set_order_name(order_type::Value::None);
        }
    }

    void UI::reset_skill()
    {
        if (get_skill_name() != "none")
        {
            set_skill_name("none");
        }
    }

    void UI::restore_visibility(const std::map<std::string, bool>& state)
    {
        auto cursor_state = state.find("cursor");

        if (cursor_state != state.end())
        {
            get_cursor().set_visible(cursor_state->second);
        }

        auto minimap_state = state.find("minimap");

        if (minimap_state != state.end())
        {
            get_minimap().set_visible(minimap_state->second);
        }

        auto preview_state = state.find("preview");

        if (preview_state != state.end())
        {
            get_preview().set_visible(preview_state->second);
        }

        auto selection_box_state = state.find("selection_box");

        if (selection_box_state != state.end())
        {
            selection_box->set_visible(selection_box_state->second);
        }

        auto ui_layers_state = state.find("ui_layers");

        if (ui_layers_state != state.end())
        {
            show_layers(ui_layers_state->second);
        }
    }

    void UI::show_layers(bool visible)
    {
        /*
        for (const auto& [name, surface] : surface_factory->get_surfaces())
        {
        surface->set_visible(visible);
        }
        */
    }

    void UI::show_menu()
    {
        // Update UI.
        // view->EvaluateScript("app.menu.show()");
    }

    void UI::toggle_floating_description()
    {
        floating_description = !floating_description;
    }

    void UI::set_visible(bool visible)
    {
        m_visible = visible;

        show_layers(visible);

        get_cursor().set_visible(visible);
        get_minimap().set_visible(visible);
        get_preview().set_visible(visible);
    }

    void UI::set_configuration_name(const std::string& _configuration_name)
    {
        reset();

        configuration_name = _configuration_name;

        // Update UI.
        set_configurations_header(_configuration_name);

        // Update cursor.
        if (_configuration_name == "none")
        {
            get_cursor().set_type(Cursor::Type::Square);
        }
        else
        {
            auto configuration = ConfigurationManager::getSingleton().get(_configuration_name);

            get_cursor().set_mesh(configuration);
            get_cursor().set_type(Cursor::Type::Object);
        }

        get_cursor().set_visible(false);

        // Reset order.
        if (order_name != order_type::Value::None)
        {
            reset_order();
        }
    }

    void UI::set_configurations(const std::set<std::string>& configurations)
    {
        // Update cache.
        static std::set<std::string> configurations_cache;

        if (configurations_cache == configurations)
        {
            return;
        }

        configurations_cache = configurations;

        // Create JSON.
        Json::Value json_configurations;

        for (const auto& configuration : configurations)
        {
            json_configurations.append(configuration);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.configurations.set(";
        stream << Utils::Convert::to_string(json_configurations);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_configurations_header(const std::string& _configuration_name)
    {
        // Update cache.
        static std::string configuration_name_cache = "none";

        if (configuration_name_cache == _configuration_name)
        {
            return;
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.configurations.set_header('";
        stream << _configuration_name;
        stream << "')";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_floating_descriptions(const std::vector<Id>& objects)
    {
        // TODO: Refactor? Fix? Remove?
        /*
        // Update cache.
        static std::vector<Id> json_floating_descriptions_cache;

        if (json_floating_descriptions_cache == objects)
        {
            return;
        }

        json_floating_descriptions_cache = objects;

        // Create JSON.
        Json::Value json_floating_descriptions;

        auto camera = Game::getSingleton().get_camera(Settings::Camera::MainName)->get_camera();
        auto window = Game::getSingleton().get_window(Settings::Window::MainName);

        for (const auto& id : objects)
        {
            auto object = ObjectManager::getSingleton().get(id);
            auto coordinates = Utils::get_screenspace_coords(object->get_mesh().get_entity(), camera, true);

            if (coordinates)
            {
                auto width = static_cast<int>(std::roundf(coordinates->x * window->get_width()));
                auto height = static_cast<int>(std::roundf(coordinates->y * window->get_height()));

                Json::Value json_object;

                json_object["id"] = id;
                json_object["name"] = object->get_name();
                json_object["left"] = width;
                json_object["top"] = height;

                for (const auto& [name, value] : object->get_progress_bars())
                {
                    json_object["bars"][name] = value;
                }

                json_floating_descriptions.append(json_object);
            }
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.ui.set_floating_descriptions(";
        stream << Utils::Convert::to_string(json_floating_descriptions);
        stream << ")";

        evaluate_with_timeout(stream.str());
        */
    }

    void UI::set_layers(const std::map<std::string, std::shared_ptr<Layer>>& layers)
    {
        // Update cache.
        static std::map<std::string, std::shared_ptr<Layer>> layers_cache;

        if (layers_cache == layers)
        {
            return;
        }

        layers_cache = layers;

        // Create JSON.
        Json::Value json_layers;

        for (const auto& [name, layer] : layers)
        {
            Json::Value json_layer;

            for (const auto& [name, data_layer] : layer->get_data_layers())
            {
                Json::Value json_data_layer;

                json_data_layer["name"] = name;

                json_layer[name] = json_data_layer;
            }

            json_layers[name] = json_layer;
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.layers.set(";
        stream << Utils::Convert::to_string(json_layers);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_maps(const std::vector<std::string>& maps)
    {
        // Update cache.
        static std::vector<std::string> maps_cache;

        if (maps_cache == maps)
        {
            return;
        }

        maps_cache = maps;

        // Create JSON.
        Json::Value json_maps;

        for (const auto& i : maps)
        {
            json_maps.append(i);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.map_menu.set_maps(";
        stream << Utils::Convert::to_string(json_maps);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_hovered_object_id(Id object_id)
    {
        hovered_object_id = object_id;
    }

    void UI::set_info(const std::shared_ptr<Configuration>& info)
    {
        // Update cache.
        static std::shared_ptr<Configuration> info_cache;

        if (info_cache == info)
        {
            return;
        }

        info_cache = info;

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.info.set(";
        stream << info_cache->to_string();
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_objects_admin()
    {
        // TODO: Fix cache.
        /*
        // Update cache.
        static ObjectManager::Container objects_cache;

        if (objects_cache == objects)
        {
            return;
        }

        objects_cache = objects;
        */

        // Create JSON.
        Json::Value json_objects;

        for (const auto& [id, object] : ObjectManager::getSingleton())
        {
            Json::Value json_object;

            json_object["id"] = id;
            json_object["name"] = object->get_name();
            json_object["order_count"] = object->get_orders()->size();

            if (object->get_orders()->size())
            {
                auto order = OrderManager::getSingleton().get(object->get_orders()->front());

                if (order)
                {
                    json_object["current_order"] = order_type::to_string(order->get_type());
                }
                else
                {
                    json_object["current_order"] = "-";
                }
            }
            else
            {
                json_object["current_order"] = "-";
            }

            json_objects.append(json_object);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.objects.set(";
        stream << Utils::Convert::to_string(json_objects);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_orders_admin()
    {
        /* TODO: Fix cache.
        // Update cache.
        static OrderManager::Container orders_cache;

        if (orders_cache == orders)
        {
            return;
        }

        orders_cache = orders;
        */

        // Create JSON.
        Json::Value json_orders;

        for (const auto& [id, order] : OrderManager::getSingleton())
        {
            Json::Value json_order;

            json_order["id"] = id;
            json_order["sender_id"] = order->get_sender_id();
            json_order["receiver_id"] = order->get_receiver_id();
            json_order["type"] = order_type::to_string(order->get_type());
            json_order["attempts_to_complete"]
                = Utils::Convert::to_string(order->get_attempts_to_complete())
                + "/"
                + Utils::Convert::to_string(Settings::Game::OrderMaxAttemptsToComplete);

            json_orders.append(json_order);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.orders.set(";
        stream << Utils::Convert::to_string(json_orders);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_players()
    {
        // TODO: Fix cache.
        /*
        // Update cache.
        static PlayerManager::Container players_cache;

        if (players_cache == players)
        {
            return;
        }

        players_cache = players;
        */

        // Create JSON.
        Json::Value json_players;

        for (const auto& [id, player] : PlayerManager::getSingleton())
        {
            Json::Value json_player;

            json_player["id"] = id;
            json_player["name"] = player->get_name();
            json_player["color"] = player->get_color();

            json_players.append(json_player);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.players.set(";
        stream << Utils::Convert::to_string(json_players);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_order_name(order_type::Value _order_name)
    {
        reset();

        order_name = _order_name;

        // Update UI.
        set_orders_header(order_name);
    }

    void UI::set_orders(const std::set<std::string>& orders)
    {
        // Update cache.
        static std::set<std::string> orders_cache;

        if (orders_cache == orders)
        {
            return;
        }

        orders_cache = orders;

        // Create JSON.
        Json::Value json_orders;

        for (const auto& order : orders)
        {
            json_orders.append(order);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.orders.set(";
        stream << Utils::Convert::to_string(json_orders);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_orders_header(order_type::Value order_type)
    {
        // Update cache.
        static order_type::Value order_type_cache = order_type::Value::None;

        if (order_type_cache == order_type)
        {
            return;
        }

        order_type_cache = order_type;

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.orders.set_header('";
        stream << order_type::to_string(order_type);
        stream << "')";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_resources(std::shared_ptr<Resources> resources)
    {
        // Update cache.
        static Resources resources_cache;

        if (resources_cache == (*(resources.get())))
        {
            return;
        }

        resources_cache = (*(resources.get()));

        // Create JSON.
        std::string rml;

        for (const auto& resource : resources_cache)
        {
            rml += std::format("<p><span class=\"name\">{}</span>: <span class=\"value\">{}/{}</span></p>",
                resource.first, resource.second.get_current(), resource.second.get_max()
            );

            // json_resource["consumption"] = resource.second.get_consumption();
            // json_resource["production"] = resource.second.get_production();
        }

        // Update UI.
        this->resources->GetElementById("title")->SetInnerRML(rml);
    }

    void UI::set_saves(const std::vector<std::string>& saves)
    {
        // Update cache.
        static std::vector<std::string> saves_cache;

        if (saves_cache == saves)
        {
            return;
        }

        saves_cache = saves;

        // Create JSON.
        Json::Value json_saves;

        for (const auto& i : saves)
        {
            json_saves.append(i);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.load_menu.set_saves(";
        stream << Utils::Convert::to_string(json_saves);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_skill_name(const std::string& _skill_name)
    {
        reset();

        skill_name = _skill_name;

        // Update UI.
        set_skills_header(skill_name);
    }

    void UI::set_skills(const std::set<std::string>& skills)
    {
        // Update cache.
        static std::set<std::string> skills_cache;

        if (skills_cache == skills)
        {
            return;
        }

        skills_cache = skills;

        // Create JSON.
        Json::Value json_skills;

        for (const auto& i : skills)
        {
            json_skills.append(i);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.skills.set(";
        stream << Utils::Convert::to_string(json_skills);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_skills_header(const std::string& skill_name)
    {
        // Update cache.
        static std::string skill_name_cache = "none";

        if (skill_name_cache == skill_name)
        {
            return;
        }

        skill_name_cache = skill_name;

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.skills.set_header('";
        stream << skill_name;
        stream << "')";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_statistics(const std::map<std::string, std::string>& statistics)
    {
        // Update cache.
        static std::map<std::string, std::string> statistics_cache;

        if (statistics_cache == statistics)
        {
            return;
        }

        statistics_cache = statistics;

        // Create JSON.
        Json::Value json_statistics;

        for (const auto& i : statistics)
        {
            Json::Value json_statistics_item;

            json_statistics_item["name"] = i.first;
            json_statistics_item["value"] = i.second;

            json_statistics.append(json_statistics_item);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.statistics.set(";
        stream << Utils::Convert::to_string(json_statistics);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_terrain_layers(const std::set<std::string>& layers)
    {
        // Update cache.
        static std::set<std::string> layers_cache;

        if (layers_cache == layers)
        {
            return;
        }

        // Create JSON.
        Json::Value json_layers;

        for (const auto& layer : layers)
        {
            json_layers.append(layer);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.terrain.set(";
        stream << Utils::Convert::to_string(json_layers);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_water_layers(const std::set<std::string>& layers)
    {
        // Update cache.
        static std::set<std::string> layers_cache;

        if (layers_cache == layers)
        {
            return;
        }

        // Create JSON.
        Json::Value json_layers;

        for (const auto& layer : layers)
        {
            json_layers.append(layer);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.water.set(";
        stream << Utils::Convert::to_string(json_layers);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::init_components()
    {
        cursor = std::make_unique<Cursor>();
        minimap = std::make_unique<Minimap>();
        preview = std::make_unique<Preview>();
        selection_box = std::make_unique<SelectionBox>();
    }

    void UI::init_documents()
    {
        context->UnloadAllDocuments();

        // TODO: Move list of documents to configuration.
        log = context->LoadDocument("../ui/log.rml");
        log->Show();

        orders = context->LoadDocument("../ui/orders.rml");
        orders->Show();

        resources = context->LoadDocument("../ui/resources.rml");
        resources->Show();
    }

    void UI::init_events()
    {
        /*
        events = std::make_shared<Events>();
        instancer = std::make_shared<Instancer>();

        Rml::Factory::RegisterEventListenerInstancer(instancer.get());

        document->AddEventListener(Rml::EventId::Blur, events.get());
        document->AddEventListener(Rml::EventId::Focus, events.get());

        document->AddEventListener(Rml::EventId::Keydown, events.get());
        document->AddEventListener(Rml::EventId::Keyup, events.get());

        document->AddEventListener(Rml::EventId::Mousedown, events.get());
        document->AddEventListener(Rml::EventId::Mouseup, events.get());

        document->AddEventListener(Rml::EventId::Mousemove, events.get());

        document->AddEventListener(Rml::EventId::Drag, events.get());
        document->AddEventListener(Rml::EventId::Dragstart, events.get());
        document->AddEventListener(Rml::EventId::Dragend, events.get());
        */
    }

    void UI::init_fonts()
    {
        // TODO: Move to configuration.
        Rml::String directory = "../ui";

        // TODO: Move to configuration.
        std::vector<Rml::String> fonts =
        {
            "LatoLatin-Regular.ttf",
            "LatoLatin-Italic.ttf",
            "LatoLatin-Bold.ttf",
            "LatoLatin-BoldItalic.ttf",
            "NotoEmoji-Regular.ttf",
        };

        for (const Rml::String& font : fonts)
        {
            // TODO: Use STL to join paths.
            Rml::LoadFontFace(directory + "/" + font, false);
        }
    }

    void UI::init_visibility_types()
    {
        /*
        ObjectAttributes visibility_types;

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton().get_configurations(ConfigurationType::Object))
        {
            visibility_types[configuration->get_string("type")].append(name);
        }

        set_visibility_types(visibility_types);
        */
    }

    void UI::evaluate_with_timeout(const std::string& js)
    {
        // view->EvaluateScript(("setTimeout(() => {" + js + ";}, 0)").c_str());
    }

    void UI::log_write(const std::string& text, const std::string& type, Id id)
    {
        // TODO: Refactor. Make class.
        static std::vector<std::string> lines;

        lines.push_back(text);

        if (lines.size() > 10)
        {
            lines.erase(lines.cbegin(), lines.cbegin() + (lines.size() - 10));
        }

        // TODO: Remove if.
        if (log)
        {
            std::string rml;

            for (const std::string& line : lines)
            {
                rml += std::format("<p>{}</p>", line);
            }

            log->GetElementById("content")->SetInnerRML(rml);
        }
    }
}
