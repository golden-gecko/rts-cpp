#pragma once

#include <Gecko/Objects/Object.hpp>
#include <Gecko/Rectangle.hpp>

namespace City
{
    class CityBuilding : public Gecko::Object
    {
    private:
        using base_type = Gecko::Object;

    public:
        CityBuilding();
        CityBuilding(const CityBuilding& other);

        void assign_resident(Gecko::Id citizen_id)
        {
            if (can_assign_resident() == true)
            {
                residents.emplace_back(citizen_id);
            }
        }

        void assign_worker(Gecko::Id worker_id)
        {
            if (can_assign_worker() == true)
            {
                workers.emplace_back(worker_id);
            }
        }

        bool can_assign_resident() const
        {
            return is_residential() && residents.size() < max_residents;
        }

        bool can_assign_worker() const
        {
            return is_industrial() && workers.size() < max_workers;
        }

        Gecko::Entrance get_entrance() const override;

        std::size_t get_present_residents() const;

        std::size_t get_present_workers() const;

        std::size_t get_max_residents() const
        {
            return max_residents;
        }

        std::size_t get_max_workers() const
        {
            return max_workers;
        }

        float get_inhabitancy() const;

        float get_work_efficiency() const;

        bool is_residential() const
        {
            return residential;
        }

        bool is_industrial() const
        {
            return industrial;
        }

        // Gecko::Rectangle get_occupied_space() const override;

        void set_position(const Ogre::Vector3& position, bool validate = true) override;

    protected:
        void update_resources(float time) override;

    private:
        bool residential = false;
        bool industrial = false;

        std::size_t max_residents = 0;
        std::size_t max_workers = 0;

        std::vector<Gecko::Id> residents;
        std::vector<Gecko::Id> workers;
    };
}
