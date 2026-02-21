#include "Gecko/UI/RenderInterface.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/Utils/Time.hpp"

namespace Gecko
{
    RenderInterface::RenderInterface(unsigned int window_width, unsigned int window_height)
    {
        // L_TIME("RenderInterface::RenderInterface()");

        m_render_system = Ogre::Root::getSingletonPtr()->getRenderSystem();

        m_scissor_enable = false;

        m_window_width = window_width;
        m_window_height = window_height;

        m_scissor_rect[0] = 0;
        m_scissor_rect[1] = 0;
        m_scissor_rect[2] = window_width;
        m_scissor_rect[3] = window_height;

        m_group = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME;
    }

    Rml::CompiledGeometryHandle RenderInterface::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices)
    {
        // L_TIME("RenderInterface::CompileGeometry()");

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
            m_render_system->convertColourValue(diffuse, &ogre_vertices[i].diffuse);

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
        // L_TIME("RenderInterface::RenderGeometry()");

        Ogre::Matrix4 transform;
        transform.makeTrans(translation.x, translation.y, 0);
        m_render_system->_setWorldMatrix(transform);
        RocketCompiledGeometry* ogre3d_geometry = reinterpret_cast<RocketCompiledGeometry*>(geometry);

        //*
        auto ogre_resource = Ogre::TextureManager::getSingleton().getByHandle(texture);

        if (ogre_resource.isNull())
        {
            m_render_system->_disableTextureUnit(0);
        }
        else
        {
            auto ogre_texture = static_pointer_cast<Ogre::Texture>(ogre_resource);

            m_render_system->_setTexture(0, true, ogre_texture);
        }

        m_render_system->_render(ogre3d_geometry->mRenderOperation);
        //*/
    }

    void RenderInterface::ReleaseGeometry(Rml::CompiledGeometryHandle geometry)
    {
        // L_TIME("RenderInterface::ReleaseGeometry()");

        RocketCompiledGeometry* ogre3d_geometry = reinterpret_cast<RocketCompiledGeometry*>(geometry);

        delete ogre3d_geometry->mRenderOperation.vertexData;
        delete ogre3d_geometry->mRenderOperation.indexData;
        delete ogre3d_geometry;
    }

    Rml::TextureHandle RenderInterface::LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source)
    {
        // L_TIME("RenderInterface::LoadTexture()");

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
        // L_TIME("RenderInterface::GenerateTexture()");

        static int texture_id = 1;
        std::string texture_name = std::format("generated_texture_{}", texture_id++);

        // Create a memory file
        Ogre::DataStreamPtr dataStream(new Ogre::MemoryDataStream((void*)source.data(), source_dimensions.x * source_dimensions.y * sizeof(unsigned int)));

        // Try to create texture from memory
        Ogre::TexturePtr ogre_texture = Ogre::TextureManager::getSingleton().loadRawData(
            texture_name,
            m_group,
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
        // L_TIME("RenderInterface::ReleaseTexture()");
    }

    void RenderInterface::EnableScissorRegion(bool enable)
    {
        // L_TIME("RenderInterface::EnableScissorRegion()");

        m_scissor_enable = enable;

        if (m_scissor_enable)
        {
            m_render_system->setScissorTest(true, m_scissor_rect[0], m_scissor_rect[1], m_scissor_rect[2], m_scissor_rect[3]);
        }
        else
        {
            m_render_system->setScissorTest(false);
        }
    }

    void RenderInterface::SetScissorRegion(Rml::Rectanglei region)
    {
        // L_TIME("RenderInterface::SetScissorRegion()");

        m_scissor_rect[0] = std::max<int>(0, region.Position().x);
        m_scissor_rect[1] = std::max<int>(0, region.Position().y);

        m_scissor_rect[2] = std::min<int>(m_window_width, region.Position().x + region.Width());
        m_scissor_rect[3] = std::min<int>(m_window_height, region.Position().y + region.Height());

        if (m_scissor_enable)
        {
            m_render_system->setScissorTest(true, m_scissor_rect[0], m_scissor_rect[1], m_scissor_rect[2], m_scissor_rect[3]);
        }
    }

    void RenderInterface::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation)
    {
        // L_TIME("UI::render()");

        if (queueGroupId != Ogre::RENDER_QUEUE_OVERLAY)
        {
            return;
        }

        Ogre::RenderSystem* render_system = Game::getSingleton().getRoot()->getRenderSystem();

        if (render_system == nullptr)
        {
            return;
        }

        if (render_system->_getViewport()->getOverlaysEnabled() == false)
        {
            return;
        }

        Ogre::RenderWindow* render_window = Game::getSingleton().getRenderWindow();

        if (render_window == nullptr)
        {
            return;
        }

        float z_near = -1.0f;
        float z_far = 1.0f;

        Ogre::Matrix4 projection_matrix = Ogre::Matrix4::ZERO;

        projection_matrix[0][0] = 2.0f / (Ogre::Real)render_window->getWidth();
        projection_matrix[0][3] = -1.0000000f;
        projection_matrix[1][1] = -2.0f / (Ogre::Real)render_window->getHeight();
        projection_matrix[1][3] = 1.0000000f;
        projection_matrix[2][2] = -2.0f / (z_far - z_near);
        projection_matrix[3][3] = 1.0000000f;
        
        render_system->_disableTextureUnitsFrom(1);
        render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
        render_system->_setColourBufferWriteEnabled(true, true, true, true);
        render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
        render_system->_setDepthBias(0, 0);
        render_system->_setDepthBufferParams(false, false);
        render_system->_setFog(Ogre::FOG_NONE);
        render_system->_setProjectionMatrix(projection_matrix);
        render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
        render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
        render_system->_setTextureCoordSet(0, 0);
        render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
        render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);
        render_system->setLightingEnabled(false);
        render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
        render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

        UI::getSingleton().get_context()->Update();
        UI::getSingleton().get_context()->Render();
    }
}
