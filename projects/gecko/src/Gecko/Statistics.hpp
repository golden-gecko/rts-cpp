#pragma once

namespace Gecko
{
    class Statistics :
        public Ogre::Singleton<Statistics>
    {
    public:
        using Container = std::map<std::string, float>;

    public:
        void add(const std::string& name, float value);

    public:
        Container::iterator begin()
        {
            return m_values.begin();
        }

        Container::iterator end()
        {
            return m_values.end();
        }

        Container::const_iterator cbegin() const
        {
            return m_values.cbegin();
        }

        Container::const_iterator cend() const
        {
            return m_values.cend();
        }

    private:
        Container m_values;
    };
}
