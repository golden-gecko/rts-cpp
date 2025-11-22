#include "Id.hpp"

#include "Utils/Convert.hpp"

namespace Gecko
{
    const Id Id::Empty = 0;
    const Id Id::Min = 1;
    const Id Id::Max = std::numeric_limits<int>::max();

    Id::Id(int value) :
        m_value(value)
    {
    }

    bool Id::is_valid() const
    {
        return Min.m_value <= m_value && m_value <= Max.m_value;
    }

    std::string Id::to_string() const
    {
        return Utils::Convert::to_string(m_value);
    }
}
