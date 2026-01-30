#pragma once

#include "Gecko/UI/Indicators/Indicator.hpp"

namespace Gecko
{
    class Point :
        public Indicator
    {
    private:
        using base_type = Indicator;

    public:
        explicit Point();

        ~Point() override;
    };
}
