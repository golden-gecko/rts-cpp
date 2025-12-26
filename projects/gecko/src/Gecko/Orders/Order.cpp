#include "Gecko/Orders/Order.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    namespace order_type
    {
        static const boost::bimap<Value, std::string> type_to_string = boost::assign::list_of<boost::bimap<Value, std::string>::relation>
            TYPE_TO_STRING(Value, Attack)
            TYPE_TO_STRING(Value, Create)
            TYPE_TO_STRING(Value, Destroy)
            TYPE_TO_STRING(Value, Follow)
            TYPE_TO_STRING(Value, Guard)
            TYPE_TO_STRING(Value, Load)
            TYPE_TO_STRING(Value, Move)
            TYPE_TO_STRING(Value, None)
            TYPE_TO_STRING(Value, Patrol)
            TYPE_TO_STRING(Value, Rally)
            TYPE_TO_STRING(Value, Stop)
            TYPE_TO_STRING(Value, Unload)
            TYPE_TO_STRING(Value, Wait)
        ;

        std::string to_string(Value value)
        {
            auto i = type_to_string.left.find(value);

            if (i == type_to_string.left.end())
            {
                throw Exception("Value not found.");
            }

            return i->second;
        }

        Value from_string(const std::string& value)
        {
            auto i = type_to_string.right.find(value);

            if (i == type_to_string.right.end())
            {
                throw Exception("Value " + value + " not found.");
            }

            return i->second;
        }
    }

    Order::Order(order_type::Value type) :
        type(type)
    {
    }

    Order::Order(const Order& other) :
        base_type(other)
    {
        type = other.type;
    }

    void Order::init()
    {
        base_type::init();

        sender_id = Id::Empty;
        receiver_id = Id::Empty;
        attempts_to_complete = 0;
    }

    std::shared_ptr<Configuration> Order::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("type", order_type::to_string(type));
        configuration->set("sender_id", sender_id);
        configuration->set("receiver_id", receiver_id);
        configuration->set("attempts_to_complete", attempts_to_complete);

        return configuration;
    }

    void Order::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        sender_id = configuration->get_int("sender_id", Id::Empty.get());
        receiver_id = configuration->get_int("receiver_id", Id::Empty.get());
        attempts_to_complete = configuration->get_int<std::uint16_t>("attempts_to_complete", 0);
    }

    void Order::update(float time)
    {
    }
}
