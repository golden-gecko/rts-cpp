#pragma once

#include <wx/app.h>

class MyApp :
    public wxApp
{
public:
    // From wxApp.
    bool OnInit() override;
};
