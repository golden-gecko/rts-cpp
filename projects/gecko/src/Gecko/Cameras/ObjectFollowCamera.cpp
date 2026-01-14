#include "Gecko/Cameras/ObjectFollowCamera.hpp"

#include "Gecko/Input.hpp"
#include "Gecko/Layers/SquareLayer.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    void ObjectFollowCamera::update(float time)
    {
        Object* target = ObjectManager::getSingleton().get(m_target_id);

        if (target)
        {
            look_at(target->get_position());
            set_position(target->get_position() + Ogre::Vector3::UNIT_SCALE * m_distance);
        }
    }

    ObjectFollowCamera::ObjectFollowCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const ConfigurationPtr& configuration) :
        base_type(root, scene_manager, name, configuration)
    {
    }
}
