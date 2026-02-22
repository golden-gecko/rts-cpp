#pragma once

namespace Gecko
{
    class Statistics :
        public Ogre::Singleton<Statistics>,
        public OIS::KeyListener,
        public OIS::MouseListener
    {
    public:
        using Container = std::map<std::string, float>;

    public:
        // From OIS::KeyListener.
        bool keyPressed(const OIS::KeyEvent& arg) override;
        bool keyReleased(const OIS::KeyEvent& arg) override;

    public:
        // From OIS::MouseListener.
        bool mouseMoved(const OIS::MouseEvent& arg) override;
	    bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) override;
		bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) override;

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
