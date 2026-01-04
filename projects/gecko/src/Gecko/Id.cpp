#include "Gecko/Id.hpp"

#include "Gecko/Utils/Convert.hpp"

namespace Gecko
{
    const Id Id::Empty = Id(0);
    const Id Id::Min   = Id(1);
    const Id Id::Max   = Id(std::numeric_limits<int>::max());

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

    int Id::get() const
    {
        return m_value;
    }

    Id& Id::operator=(int value)
    {
        m_value = value;

        return *this;
    }

    bool Id::operator==(const Id& other) const
    {
        return m_value == other.m_value;
    }

    bool Id::operator<(const Id& other) const
    {
        return m_value < other.m_value;
    }

    bool Id::operator<=(const Id& other) const
    {
        return m_value <= other.m_value;
    }

    bool Id::operator>(const Id& other) const
    {
        return m_value > other.m_value;
    }

    bool Id::operator>=(const Id& other) const
    {
        return m_value >= other.m_value;
    }

    Id& Id::operator++()
    {
        m_value++;

        return *this;
    }

    Id& Id::operator--()
    {
        m_value--;

        return *this;
    }

    Id Id::operator++(int)
    {
        Id previous(*this);

        ++(*this);

        return previous;
    }

    Id Id::operator--(int)
    {
        Id previous(*this);

        --(*this);

        return previous;
    }

    Id::operator Json::Value() const
    {
        return m_value;
    }
}
