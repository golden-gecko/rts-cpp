#include "Gecko/Editor/App.hpp"

#include "Gecko/Editor/Frame.hpp"

bool MyApp::OnInit()
{
    if (wxApp::OnInit() == false)
    {
        return false;
    }

    new MyFrame();

    return true;
}
