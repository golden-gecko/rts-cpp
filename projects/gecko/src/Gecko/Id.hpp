#pragma once

namespace Gecko
{
    class Id
    {
    public:
        static const Id Empty;
        static const Id Min;
        static const Id Max;

    public:
        static Id from_string(const std::string& id);

    public:
        explicit Id(int value = 0);
        Id(const Id& other);

    public:
        Id& operator=(const Id&);
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

        operator Json::Value() const;

    public:
        std::string to_string() const;

    public:
        int get() const;

    public:
        bool is_valid() const;

    private:
        int m_value = 0; // TODO: Change to std::uint64_t.
    };

    std::ostream& operator<<(std::ostream& out, const Id& id);
}
