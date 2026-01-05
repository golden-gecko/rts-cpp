#pragma once

namespace Gecko
{
    enum QueryFlags : int
    {
        QF_Cursor = 1 << 0,
        QF_Layer  = 1 << 1,
        QF_Object = 1 << 2
    };
}
