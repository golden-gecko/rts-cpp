#pragma once

namespace Gecko
{
    enum QueryFlags : int
    {
        QF_CURSOR = 1 << 0, // TODO: Make lower case.
        QF_LAYER  = 1 << 1,
        QF_OBJECT = 1 << 2
    };
}
