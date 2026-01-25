#pragma once

namespace Gecko
{
    class Statistics :
        public Ogre::Singleton<Statistics>
    {
    public:
        void add(const std::string& name, float value);

    public:
        auto begin()
        {
            return m_values.begin();
        }

        auto end()
        {
            return m_values.end();
        }

        auto cbegin() const
        {
            return m_values.cbegin();
        }

        auto cend() const
        {
            return m_values.cend();
        }

    private:
        std::map<std::string, float> m_values;
    };
}
