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

        std::vector<Order*> get_job(const Id& id, const std::shared_ptr<Components>& components, const std::shared_ptr<Resources>& resources);

        void remove_in(const Id& requester, const std::string& resource_name);
        void remove_out(const Id& requester, const std::string& resource_name);

    private:
        std::deque<Request> m_in_queue;
        std::deque<Request> m_out_queue;

        std::vector<Order*> get_attack_job(const Id& id, const std::shared_ptr<Resources>& resources);
        std::vector<Order*> get_transport_job(const Id& id, const std::shared_ptr<Resources>& resources);
        std::vector<Order*> get_unload_job(const Id& id, const std::shared_ptr<Resources>& resources);
    };
}
