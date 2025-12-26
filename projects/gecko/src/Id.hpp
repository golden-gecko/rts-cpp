#pragma once

namespace Gecko
{
    class Id final
    {
    public:
        static const Id Empty;
        static const Id Min;
        static const Id Max;

    public:
        Id(int value = 0);

        bool is_valid() const;

        std::string to_string() const;

    public:
        auto get() const
        {
            return m_value;
        }

    public:
        Id& operator=(int value)
        {
            m_value = value;

            return *this;
        }

        bool operator==(const Id& other) const
        {
            return m_value == other.m_value;
        }

        bool operator<(const Id& other) const
        {
            return m_value < other.m_value;
        }

        bool operator<=(const Id& other) const
        {
            return m_value <= other.m_value;
        }

        bool operator>(const Id& other) const
        {
            return m_value > other.m_value;
        }

        bool operator>=(const Id& other) const
        {
            return m_value >= other.m_value;
        }

        Id& operator++()
        {
            m_value++;

            return *this;
        }

        Id operator++(int)
        {
            Id previous(*this);

            ++(*this);

            return previous;
        }

        Id& operator--()
        {
            m_value--;

            return *this;
        }

        Id operator--(int)
        {
            Id previous(*this);

            --(*this);

            return previous;
        }

        operator Json::Value() const
        {
            return m_value;
        }

    private:
        int m_value = 0;
    };

    inline std::ostream& operator<<(std::ostream& out, const Id& id)
    {
        return out << id.get();
    }
}
