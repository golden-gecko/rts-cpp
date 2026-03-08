#include "Gecko/Process.hpp"

#include "Gecko/Components/Storage.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Components.hpp"
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
        m_in = std::make_shared<Resources>();
        m_out = std::make_shared<Resources>();
    }

    ConfigurationPtr Process::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("name", m_name);
        configuration->set("in", m_in->serialize());
        configuration->set("out", m_out->serialize());
        configuration->set("time", m_time.get_max());

        return configuration;
    }

    void Process::deserialize(const ConfigurationPtr& configuration)
    {
        if (configuration->has_member("in"))
        {
            m_in->deserialize(configuration->get_child("in"));
        }

        if (configuration->has_member("out"))
        {
            m_out->deserialize(configuration->get_child("out"));
        }

        m_time.set_max(configuration->get_float("time", 0.0f));
    }

    void Process::update(float time, const Id& owner_id, const Ogre::Vector3& owner_position, const ResourcesPtr& owner_resources)
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
                    if (get_deposit(in_resource, owner_position, { owner_id }) == nullptr)
                    {
                        in_resources_available = false;

                        break;
                    }
                }
                else if (owner_resources->has_resource(in_resource_name, in_resource.get_consumption()) == false)
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
                    if (get_storage(out_resource, owner_position, { owner_id }) == nullptr)
                    {
                        in_resources_available = false;

                        break;
                    }
                }
                else if (owner_resources->has_storage(out_resource_name, out_resource.get_production()) == false)
                {
                    out_resources_available = false;

                    break;
                }
            }
        }

        if (in_resources_available && out_resources_available)
        {
            if (m_time.update(time))
            {
                m_time.reset();

                // Remove resources.
                for (const auto& [in_resource_name, in_resource] : *(m_in))
                {
                    if (in_resource.get_need_deposit())
                    {
                        ObjectPtr deposit = get_deposit(in_resource, owner_position, { owner_id });

                        if (deposit)
                        {
                            deposit->get_resources()->remove(in_resource_name, in_resource.get_consumption());
                        }
                        else
                        {
                            L_ERROR << "Deposit for resource " << in_resource_name << " not found.";
                        }
                    }
                    else
                    {
                        owner_resources->remove(in_resource_name, in_resource.get_consumption());
                    }
                }

                // Add resources.
                for (const auto& [out_resource_name, out_resource] : *(m_out))
                {
                    if (out_resource.get_need_storage())
                    {
                        ObjectPtr storage = get_storage(out_resource, owner_position, { owner_id });

                        if (storage)
                        {
                            storage->get_resources()->remove(out_resource_name, out_resource.get_production());
                        }
                        else
                        {
                            L_ERROR << "Storage for resource " << out_resource_name << " not found.";
                        }
                    }
                    else if (owner_resources->has_storage(out_resource_name, out_resource.get_production()))
                    {
                        owner_resources->add(out_resource_name, out_resource.get_production());
                    }
                }
            }
        }

        // Notify request manager about available resources.
        auto request_manager = JobManager::getSingletonPtr();

        for (const auto& [in_resource_name, _] : *(m_in))
        {
            std::uint64_t storage = owner_resources->get_storage(in_resource_name);

            if (storage > 0)
            {
                request_manager->add_in(owner_id, in_resource_name, storage);
            }
            else
            {
                request_manager->remove_in(owner_id, in_resource_name);
            }
        }

        for (const auto& [out_resource_name, _] : *(m_out))
        {
            std::uint64_t current = owner_resources->get_current(out_resource_name);

            if (current > 0)
            {
                request_manager->add_out(owner_id, out_resource_name, current);
            }
            else
            {
                request_manager->remove_out(owner_id, out_resource_name);
            }
        }
    }

    bool Process::operator==(const Process& other) const
    {
        return m_name == other.m_name && m_in == other.m_in && m_out == other.m_out;
    }

    ObjectPtr Process::get_deposit(const Resource& in_resource, const Ogre::Vector3& position, const Id& exclude) const
    {
        auto deposits = ObjectManager::getSingleton().get_in_range(position, in_resource.get_deposit_range(), { exclude });

        for (const auto& [deposit, _] : deposits)
        {
            if (deposit->get_resources()->has_resource(in_resource.get_name(), in_resource.get_consumption()))
            {
                return deposit;
            }
        }

        return nullptr;
    }

    ObjectPtr Process::get_storage(const Resource& out_resource, const Ogre::Vector3& position, const Id& exclude) const
    {
        auto storages = ObjectManager::getSingleton().get_in_range(position, out_resource.get_storage_range(), { exclude });

        for (const auto& [storage, _] : storages)
        {
            if (storage->get_resources()->has_resource(out_resource.get_name(), out_resource.get_production()))
            {
                return storage;
            }
        }

        return nullptr;
    }
}
