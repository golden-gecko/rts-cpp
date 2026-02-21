#pragma once

#include "Gecko/Input/Strategies/OrderStrategy.hpp"

namespace Gecko
{
    class GuardOrderStrategy :
        public OrderStrategy
    {
    public:
        // From OrderStrategy.
        void execute(const OIS::MouseEvent& arg) override;
    };
}
