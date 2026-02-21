#pragma once

#include "Gecko/Input/Strategies/OrderStrategy.hpp"

namespace Gecko
{
    class AttackOrderStrategy :
        public OrderStrategy
    {
    public:
        // From OrderStrategy.
        void execute(const OIS::MouseEvent& arg) override;
    };
}
