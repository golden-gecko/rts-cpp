#pragma once

#include "Gecko/Objects/Object.hpp"

namespace City
{
    class Citizen : public Gecko::Object
    {
    private:
        using base_type = Gecko::Object;

    public:
        explicit Citizen() = default;
        explicit Citizen(const Citizen& other);

        void update(float time) override;

    public:
        bool is_working() const
        {
            return false; // get_state() == Gecko::State::Value::Working;
        }

    public:
        Gecko::Id get_home_id() const
        {
            return home_id;
        }

        Gecko::Id get_workplace_id() const
        {
            return workplace_id;
        }

        Gecko::Id get_vehicle_id() const
        {
            return vehicle_id;
        }

    public:
        void set_home_id(Gecko::Id _home_id)
        {
            home_id = _home_id;
        }

        void set_position(const Ogre::Vector3& position, bool validate = true) override;

        void set_workplace_id(Gecko::Id _workplace_id)
        {
            workplace_id = _workplace_id;
        }

        void set_vehicle_id(Gecko::Id _vehicle_id)
        {
            vehicle_id = _vehicle_id;
        }

    protected:
        void update_orders(float time) override;
        void update_resources(float time) override;

        Gecko::order_status on_follow(Gecko::Order* order, float time) override;
        Gecko::order_status on_guard(Gecko::Order* order, float time) override;
        Gecko::order_status on_load(Gecko::Order* order, float time) override;
        Gecko::order_status on_move(Gecko::Order* order, float time) override;
        Gecko::order_status on_patrol(Gecko::Order* order, float time) override;
        Gecko::order_status on_unload(Gecko::Order* order, float time) override;

    private:
        float distance = 0.0f;
        float speed = 0.0f;

        Gecko::Id home_id;
        Gecko::Id workplace_id;

        Gecko::Timer home_time;
        Gecko::Timer workplace_time;

        Gecko::Id vehicle_id;

        // AI.
        Gecko::Timer update_timer = Gecko::Timer(3.0f);
    };
}
