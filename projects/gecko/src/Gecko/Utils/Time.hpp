#pragma once

#define L_TIME(name) Gecko::Utils::Time::Scope scope(name)

namespace Gecko::Utils::Time
{
    typedef std::chrono::steady_clock::time_point Value;

    Value get_current();

    float get_duration(const Value& start, const Value& end);

    class Scope
    {
    public:
        explicit Scope(const std::string& name);

        ~Scope();

    private:
        std::string m_name;

        Value m_start;
    };
}
