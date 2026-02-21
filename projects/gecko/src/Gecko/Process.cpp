#include "Gecko/Process.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/JobManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    Process::Process(const std::string& name) :
        m_name(name)
    {
    }

    ConfigurationPtr Process::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("name", m_name);

        configuration->set("in", m_in->serialize());
        configuration->set("out", m_out->serialize());

        return configuration;
    }

    void Process::deserialize(const ConfigurationPtr& configuration)
    {
        m_in = std::make_shared<Resources>();

        if (configuration->has_member("in"))
        {
            m_in->deserialize(configuration->get_child("in"));
        }

        m_out = std::make_shared<Resources>();

        if (configuration->has_member("out"))
        {
            m_out->deserialize(configuration->get_child("out"));
        }
    }

    void Process::update(float time, const Id& id, const Ogre::Vector3& position, std::shared_ptr<Resources> resources)
    {
        bool in_resources_available = true;
        bool out_resources_available = true;

        // Check if input resources are available to remove.
        if (m_in->empty() == false)
        {
            for (const auto& [in_resource_name, in_resource] : *(m_in))
            {
                if (in_resource.get_need_deposit())
                {
                    if (get_deposit(in_resource, id, position, time) == nullptr)
                    {
                        in_resources_available = false;

                        break;
                    }
                }
                // TODO: If out_resource.get_consumption() * time is greater than storage this will fail.
                else if (resources->has_resource(in_resource_name, in_resource.get_consumption() * time) == false)
                {
                    in_resources_available = false;

                    break;
                }
            }
        }

        // Check if output resources are available to add.
        if (in_resources_available && m_out->empty() == false)
        {
            for (const auto& [out_resource_name, out_resource] : *(m_out))
            {
                if (out_resource.get_need_storage())
                {
                    if (get_storage(out_resource, id, position, time) == nullptr)
                    {
                        in_resources_available = false;

                        break;
                    }
                }
                // TODO: If out_resource.get_production() * time is greater than storage this will fail.
                else if (resources->has_storage(out_resource_name, out_resource.get_production() * time) == false)
                {
                    out_resources_available = false;

                    break;
                }
            }
        }

        if (in_resources_available && out_resources_available)
        {
            // Remove resources.
            for (const auto& [in_resource_name, in_resource] : *(m_in))
            {
                if (in_resource.get_need_deposit())
                {
                    auto deposit = get_deposit(in_resource, id, position, time);

                    if (deposit)
                    {
                        deposit->get_resources()->remove(in_resource_name, in_resource.get_consumption() * time);
                    }
                    else
                    {
                        L_ERROR << "Deposit for resource " << in_resource_name << " not found.";
                    }
                }
                else
                {
                    resources->remove(in_resource_name, in_resource.get_consumption() * time);
                }
            }

            // Add resources.
            for (const auto& [out_resource_name, out_resource] : *(m_out))
            {
                if (out_resource.get_need_storage())
                {
                    auto storage = get_storage(out_resource, id, position, time);

                    if (storage)
                    {
                        storage->get_resources()->remove(out_resource_name, out_resource.get_production() * time);
                    }
                    else
                    {
                        L_ERROR << "Storage for resource " << out_resource_name << " not found.";
                    }
                }
                else if (resources->has_storage(out_resource_name, out_resource.get_production() * time))
                {
                    resources->add(out_resource_name, out_resource.get_production() * time);
                }
            }
        }

        // Notify request manager about available resources.
        auto request_manager = JobManager::getSingletonPtr();

        for (const auto& [in_resource_name, in_resource] : *(m_in))
        {
            auto storage = resources->get_storage(in_resource_name);

            if (Utils::is_enough_to_process(storage))
            {
                request_manager->add_in(id, in_resource_name, storage, in_resource.get_priority());
            }
            else
            {
                request_manager->remove_in(id, in_resource_name);
            }
        }

        for (const auto& [out_resource_name, out_resource] : *(m_out))
        {
            auto current = resources->get_current(out_resource_name);

            if (Utils::is_enough_to_process(current))
            {
                request_manager->add_out(id, out_resource_name, current, out_resource.get_priority());
            }
            else
            {
                request_manager->remove_out(id, out_resource_name);
            }
        }
    }

    bool Process::operator==(const Process& other) const
    {
        return m_name == other.m_name && m_in == other.m_in && m_out == other.m_out;
    }

    ObjectPtr Process::get_deposit(const Resource& in_resource, const Id& id, const Ogre::Vector3& position, float time) const
    {
        auto deposits = ObjectManager::getSingleton().get_in_range(position, in_resource.get_deposit_range());

        for (const auto& [deposit, distance] : deposits)
        {
            if (deposit->get_id() == id)
            {
                continue;
            }

            if (deposit->get_resources()->has_resource(in_resource.get_name(), in_resource.get_consumption() * time))
            {
                return deposit;
            }
        }

        return nullptr;
    }

    ObjectPtr Process::get_storage(const Resource& out_resource, const Id& id, const Ogre::Vector3& position, float time) const
    {
        auto storages = ObjectManager::getSingleton().get_in_range(position, out_resource.get_storage_range());

        for (const auto& [storage, distance] : storages)
        {
            if (storage->get_id() == id)
            {
                continue;
            }

            if (storage->get_resources()->has_resource(out_resource.get_name(), out_resource.get_production() * time))
            {
                return storage;
            }
        }

        return nullptr;
    }
}
