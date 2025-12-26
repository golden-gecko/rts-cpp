#pragma once

namespace Gecko
{
    class Updatable
    {
    public:
        virtual ~Updatable() = default;

        virtual void update(float time) = 0;
    };
}
