#include "Gecko/Editor/App.hpp"
#include "Gecko/Editor/Frame.hpp"

/*

// #include <Gecko/Gecko.hpp>

#ifndef WX_PRECOMP

#include "wx/statusbr.h"
#endif
*/

wxIMPLEMENT_APP(MyApp);

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // EVT_MENU(Id_MultiSelect, MyFrame::OnMultiSelect)
    // EVT_MENU(Id_FlatList, MyFrame::OnFlatList)
    // EVT_MENU_RANGE(Id_Checkboxes_Start, Id_Checkboxes_End, MyFrame::OnCheckboxes)

    // EVT_MENU(Id_DumpSelection, MyFrame::OnDumpSelection)
    // EVT_MENU_RANGE(Id_Check_HTMLDocs, Id_Indet_HTMLDocs, MyFrame::OnCheckHTMLDocs)
    // EVT_MENU(Id_Select_HTMLDocs, MyFrame::OnSelectHTMLDocs)

    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)

    // EVT_MENU(Id_DeleteAllItems, MyFrame::OnDeleteAllItems)

    EVT_TREELIST_SELECTION_CHANGED(wxID_ANY, MyFrame::OnSelectionChanged)
    EVT_TREELIST_ITEM_EXPANDING(wxID_ANY, MyFrame::OnItemExpanding)
    EVT_TREELIST_ITEM_EXPANDED(wxID_ANY, MyFrame::OnItemExpanded)
    EVT_TREELIST_ITEM_CHECKED(wxID_ANY, MyFrame::OnItemChecked)
    EVT_TREELIST_ITEM_ACTIVATED(wxID_ANY, MyFrame::OnItemActivated)
    EVT_TREELIST_ITEM_CONTEXT_MENU(wxID_ANY, MyFrame::OnItemContextMenu)
wxEND_EVENT_TABLE()
