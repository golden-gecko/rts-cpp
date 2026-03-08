#pragma once

#include "Gecko/Id.hpp"

namespace Gecko
{
    struct Request
    {
        Id requester_id;
        std::string resource_name;
        float resource_value;

        explicit Request(const Id& requester_id, const std::string& resource_name, float resource_value);
    };

    class JobManager :
        public Ogre::Singleton<JobManager>
    {
    public:
        void add_in(const Id& requester, const std::string& resource_name, float resource_value);
        void add_out(const Id& requester, const std::string& resource_name, float resource_value);

        std::vector<OrderPtr> get_job(const Id& id, const std::shared_ptr<Components>& components, const ResourcesPtr& resources);

        void remove_in(const Id& requester, const std::string& resource_name);
        void remove_out(const Id& requester, const std::string& resource_name);

    private:
        std::deque<Request> m_in_queue;
        std::deque<Request> m_out_queue;

        std::vector<OrderPtr> get_attack_job(const Id& id, const ResourcesPtr& resources);
        std::vector<OrderPtr> get_transport_job(const Id& id, const ResourcesPtr& resources);
        std::vector<OrderPtr> get_unload_job(const Id& id, const ResourcesPtr& resources);
    };
}
