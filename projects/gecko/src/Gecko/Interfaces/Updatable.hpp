#pragma once

namespace Gecko
{
    class Updatable
    {
    public:
        virtual ~Updatable() = default;
        
    public:
        virtual void update(float time) = 0;
    };
}
