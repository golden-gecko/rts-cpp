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

class MyComparator :
    public wxTreeListItemComparator
{
public:
    virtual int Compare(wxTreeListCtrl* treelist,unsigned column, wxTreeListItem item1, wxTreeListItem item2) override
    {
        wxString text1 = treelist->GetItemText(item1, column),
                 text2 = treelist->GetItemText(item2, column);

        switch ( column )
        {
            case Col_Component:
                // Simple alphabetical comparison is fine for those.
                return text1.CmpNoCase(text2);

            case Col_Files:
                // Compare strings as numbers.
                return GetNumFilesFromText(text1) - GetNumFilesFromText(text2);

            case Col_Size:
                // Compare strings as numbers but also take care of "KiB" and
                // "MiB" suffixes.
                return GetSizeFromText(text1) - GetSizeFromText(text2);
        }

        wxFAIL_MSG( "Sorting on unknown column?" );

        return 0;
    }

private:
    // Return the number of files handling special value "many". Notice that
    // the returned value is signed to allow using it in subtraction above.
    int GetNumFilesFromText(const wxString& text) const
    {
        unsigned long n;
        if ( !text.ToULong(&n) )
        {
            if ( text == "many" )
                n = 9999;
            else
                n = 0;
        }

        return n;
    }

    // Return the size in KiB from a string with either KiB or MiB suffix.
    int GetSizeFromText(const wxString& text) const
    {
        wxString size;
        unsigned factor = 1;
        if ( text.EndsWith(" MiB", &size) )
            factor = 1024;
        else if ( !text.EndsWith(" KiB", &size) )
            return 0;

        unsigned long n = 0;
        size.ToULong(&n);

        return n*factor;
    }
};

// ----------------------------------------------------------------------------
// Main window class
// ----------------------------------------------------------------------------

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


    enum
    {
        Icon_File,
        Icon_FolderClosed,
        Icon_FolderOpened
    };

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

    MyComparator m_comparator;

    wxTreeListItem m_itemHTMLDocs;

    wxLog* m_oldLogTarget;

    bool m_isFlat;

    wxDECLARE_EVENT_TABLE();
};
