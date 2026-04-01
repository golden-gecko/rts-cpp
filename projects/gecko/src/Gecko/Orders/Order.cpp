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

    void Order::init()
    {
        base_type::init();

        m_sender_id = Id::Empty;
        m_receiver_id = Id::Empty;
        m_attempts_to_complete = 0;
    }

    ConfigurationPtr Order::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("type", order_type::to_string(m_type));
        configuration->set("sender_id", m_sender_id);
        configuration->set("receiver_id", m_receiver_id);
        configuration->set("attempts_to_complete", m_attempts_to_complete);

        return configuration;
    }

    void Order::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_sender_id = configuration->get_int("sender_id", Id::Empty.get());
        m_receiver_id = configuration->get_int("receiver_id", Id::Empty.get());
        m_attempts_to_complete = configuration->get_int<std::uint16_t>("m_attempts_to_complete", 0);
    }

    void Order::update(float time)
    {
    }

    Order::Order(order_type::Value type) :
        m_type(type)
    {
    }
}
