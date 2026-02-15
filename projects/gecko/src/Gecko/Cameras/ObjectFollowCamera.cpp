#include "Gecko/Cameras/ObjectFollowCamera.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    ConfigurationPtr ObjectFollowCamera::serialize() const
    {
        ConfigurationPtr configuration = base_type::serialize();

        configuration->set("distance", m_distance);

        return configuration;
    }
        
    void ObjectFollowCamera::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_distance = configuration->get_float("distance", 0.0f);
    }

    void ObjectFollowCamera::update(float time)
    {
        if (ObjectPtr target = ObjectManager::getSingleton().get(m_target_id))
        {
            look_at(target->get_position());
            set_position(target->get_position() + Ogre::Vector3::UNIT_SCALE * m_distance);
        }
    }

    ObjectFollowCamera::ObjectFollowCamera(Scene* scene, const std::string& name, const ConfigurationPtr& configuration) :
        base_type(scene, name, configuration)
    {
        m_distance = configuration->get_float("distance", 0.0f);
    }
}
