#pragma once

namespace Gecko::Utils::Mesh
{
    Ogre::Entity* copy_entity(const ScenePtr& scene, Ogre::Entity* other);
    Ogre::ManualObject* copy_manual_object(const ScenePtr& scene, Ogre::ManualObject* other);
    Ogre::SceneNode* copy_scene_node(Ogre::SceneNode* other);

    void get_information(const Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
}
