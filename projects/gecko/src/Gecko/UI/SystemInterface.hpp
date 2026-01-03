#pragma once

namespace Gecko
{
    class SystemInterface :
        public Rml::SystemInterface
    {
    public:
        // From Rml::SystemInterface.
        bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;
    };
}
