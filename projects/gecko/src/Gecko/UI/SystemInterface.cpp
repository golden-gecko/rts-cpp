#include "Gecko/UI/SystemInterface.hpp"

#include "Gecko/Log.hpp"

namespace Gecko
{
    bool SystemInterface::LogMessage(Rml::Log::Type type, const Rml::String& message)
    {
        switch (type)
        {
            case Rml::Log::Type::LT_ALWAYS:
            case Rml::Log::Type::LT_ERROR:
                L_ERROR << message;
                break;

            case Rml::Log::Type::LT_ASSERT:
            case Rml::Log::Type::LT_WARNING:
                L_WARNING << message;
                break;

            case Rml::Log::Type::LT_INFO:
                L_INFO << message;
                break;

            case Rml::Log::Type::LT_DEBUG:
                L_DEBUG << message;
                break;
        }

        return Rml::SystemInterface::LogMessage(type, message);
    }
}
