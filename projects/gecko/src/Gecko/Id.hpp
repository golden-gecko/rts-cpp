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
        explicit Id(int value = 0);

        bool is_valid() const;

        std::string to_string() const;

    public:
        int get() const;

    public:
        Id& operator=(int value);

        bool operator==(const Id& other) const;
        bool operator<(const Id& other) const;
        bool operator<=(const Id& other) const;
        bool operator>(const Id& other) const;
        bool operator>=(const Id& other) const;

        Id& operator++();
        Id& operator--();

        Id operator++(int);
        Id operator--(int);

        // TODO: Move outside Id.
        operator Json::Value() const;

    private:
        int m_value = 0;
    };

    // TODO: Move outside Id.
    inline std::ostream& operator<<(std::ostream& out, const Id& id)
    {
        return out << id.get();
    }
}
