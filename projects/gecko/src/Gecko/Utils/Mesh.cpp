#include "Gecko/Utils/Mesh.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Exception.hpp"

namespace Gecko::Utils::Mesh
{
    Ogre::Entity* copy_entity(const Ogre::Entity& other)
    {
        if (other.getMesh().isNull())
        {
            throw Exception("Entity '" + other.getName() + "'does not have mesh.");
        }

        auto entity = Game::getSingleton().create_entity(other.getMesh()->getName());

        entity->setCastShadows(other.getCastShadows());
        entity->setQueryFlags(other.getQueryFlags());
        entity->setVisibilityFlags(other.getVisibilityFlags());
        entity->setVisible(other.getVisible());

        return entity;
    }

    Ogre::ManualObject* copy_manual_object(const Ogre::ManualObject& other)
    {
        auto manual_object = Game::getSingleton().create_manual_object();

        manual_object->setCastShadows(other.getCastShadows());
        manual_object->setQueryFlags(other.getQueryFlags());
        manual_object->setVisibilityFlags(other.getVisibilityFlags());
        manual_object->setVisible(other.getVisible());

        return manual_object;
    }

    Ogre::SceneNode* copy_scene_node(const Ogre::SceneNode& other)
    {
        auto scene_node = other.getParentSceneNode()->createChildSceneNode();

        scene_node->setFixedYawAxis(true);
        scene_node->setInheritOrientation(other.getInheritOrientation());
        scene_node->setInheritScale(other.getInheritScale());
        scene_node->setOrientation(other.getOrientation());
        scene_node->setScale(other.getScale());

        return scene_node;
    }

    void get_information(const Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale)
    {
        bool added_shared = false;
        size_t current_offset = 0;
        size_t shared_offset = 0;
        size_t next_offset = 0;
        size_t index_offset = 0;

        vertex_count = index_count = 0;

        // Calculate how many vertices and indices we're going to need
        for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh(i);

            // We only need to add the shared vertices once
            if (submesh->useSharedVertices)
            {
                if (!added_shared)
                {
                    vertex_count += mesh->sharedVertexData->vertexCount;
                    added_shared = true;
                }
            }
            else
            {
                vertex_count += submesh->vertexData->vertexCount;
            }

            // Add the indices
            index_count += submesh->indexData->indexCount;
        }

        // Allocate space for the vertices and indices
        vertices = new Ogre::Vector3[vertex_count];
        indices = new unsigned long[index_count];

        added_shared = false;

        // Run through the submeshes again, adding the data into the arrays
        for (auto const& submesh : mesh->getSubMeshes())
        {
            auto vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

            if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
            {
                if (submesh->useSharedVertices)
                {
                    added_shared = true;
                    shared_offset = current_offset;
                }

                auto posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                const auto& vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
                auto vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

                // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
                //  as second argument. So make it float, to avoid trouble when Ogre::Real will
                //  be comiled/typedefed as double:
                //      Ogre::Real* pReal;
                float* pReal;

                for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
                {
                    posElem->baseVertexPointerToElement(vertex, &pReal);

                    Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                    vertices[current_offset + j] = (orient * (pt * scale)) + position;
                }

                vbuf->unlock();
                next_offset += vertex_data->vertexCount;
            }

            auto index_data = submesh->indexData;
            auto numTris = index_data->indexCount / 3;
            auto ibuf = index_data->indexBuffer;

            if (ibuf)
            {
                bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

                auto pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                auto pShort = reinterpret_cast<unsigned short*>(pLong);

                size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;
                size_t index_start = index_data->indexStart;
                size_t last_index = numTris * 3 + index_start;

                if (use32bitindexes)
                {
                    for (size_t k = index_start; k < last_index; ++k)
                    {
                        indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
                    }
                }
                else
                {
                    for (size_t k = index_start; k < last_index; ++k)
                    {
                        indices[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
                    }
                }

                ibuf->unlock();
                current_offset = next_offset;
            }
        }
    }
}
