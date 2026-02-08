#pragma once

namespace Gecko
{
    class Serializable
    {
    public:
        virtual ~Serializable() = default;
        
    public:
        virtual ConfigurationPtr serialize() const = 0;
        virtual void deserialize(const ConfigurationPtr& configuration) = 0;
    };
}
