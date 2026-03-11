#pragma once

#include <wx/frame.h>
#include <wx/log.h>
#include <wx/treelist.h>

enum
{
    Col_Component,
    Col_Files,
    Col_Size
};

enum
{
    Id_MultiSelect = 100,
    Id_FlatList,

    Id_Checkboxes_Start,
    Id_NoCheckboxes = Id_Checkboxes_Start,
    Id_Checkboxes2State,
    Id_Checkboxes3State,
    Id_CheckboxesUser3State,
    Id_Checkboxes_End,

    Id_DeleteAllItems,

    Id_DumpSelection,
    Id_Check_HTMLDocs,
    Id_Uncheck_HTMLDocs,
    Id_Indet_HTMLDocs,
    Id_Select_HTMLDocs
};

enum
{
    Icon_File,
    Icon_FolderClosed,
    Icon_FolderOpened
};

class MyFrame :
    public wxFrame
{
public:
    MyFrame();

    virtual ~MyFrame();

private:
    // Event handlers for the menu and wxTreeListCtrl events.
    void OnMultiSelect(wxCommandEvent& event);
    void OnFlatList(wxCommandEvent& event);
    void OnCheckboxes(wxCommandEvent& event);
    void OnDumpSelection(wxCommandEvent& event);
    void OnCheckHTMLDocs(wxCommandEvent& event);
    void OnSelectHTMLDocs(wxCommandEvent& event);

    void OnAbout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    void OnDeleteAllItems(wxCommandEvent& event);

    void OnSelectionChanged(wxTreeListEvent& event);
    void OnItemExpanding(wxTreeListEvent& event);
    void OnItemExpanded(wxTreeListEvent& event);
    void OnItemChecked(wxTreeListEvent& event);
    void OnItemActivated(wxTreeListEvent& event);
    void OnItemContextMenu(wxTreeListEvent& event);


    // Create the image list, called once only. Should add images to it in the
    // same order as they appear in the enum above.
    void InitImageList();

    // Create the control with the given styles.
    wxTreeListCtrl* CreateTreeListCtrl(long style);

    // Recreate an already existing control.
    void RecreateTreeListCtrl(long style);

    // Helper: return the text of the item or "NONE" if the item is invalid.
    wxString DumpItem(wxTreeListItem item) const;

    // Another helper: just translate wxCheckBoxState to user-readable text.
    static const char* CheckedStateString(wxCheckBoxState state);

    wxImageList* m_imageList;

    wxTreeListCtrl* m_treelist;

    wxLog* m_oldLogTarget;

    wxDECLARE_EVENT_TABLE();
};
