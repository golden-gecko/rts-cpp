#include "Gecko/Editor/Frame.hpp"

#include <filesystem>

#include <wx/aboutdlg.h>
#include <wx/artprov.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>

MyFrame::MyFrame() :
    wxFrame(nullptr, wxID_ANY, "Gecko Editor", wxDefaultPosition, wxSize(1200, 900))
{
    /*
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT);

    wxMenu* treeStyle = new wxMenu;
    treeStyle->AppendCheckItem(Id_MultiSelect, "&Multiple selections\tCtrl-M");
    treeStyle->AppendSeparator();
    treeStyle->AppendRadioItem(Id_NoCheckboxes, "&No checkboxes\tCtrl-1");
    treeStyle->AppendRadioItem(Id_Checkboxes2State, "&2-state checkboxes\tCtrl-2");
    treeStyle->AppendRadioItem(Id_Checkboxes3State, "&3-state checkboxes\tCtrl-3");
    treeStyle->AppendRadioItem(Id_CheckboxesUser3State, "&User-settable 3-state checkboxes\tCtrl-4");
    treeStyle->AppendSeparator();
    treeStyle->AppendCheckItem(Id_FlatList, "&Flat list");

    wxMenu* treeOper = new wxMenu;
    treeOper->Append(Id_DumpSelection, "&Dump selection\tCtrl-D");
    treeOper->AppendSeparator();
    treeOper->Append(Id_Check_HTMLDocs, "&Check Doc/HTML item\tCtrl-C");
    treeOper->Append(Id_Uncheck_HTMLDocs, "&Uncheck Doc/HTML item\tCtrl-U");
    treeOper->Append(Id_Indet_HTMLDocs, "Make Doc/HTML &indeterminate\tCtrl-I");
    treeOper->Append(Id_Select_HTMLDocs, "&Select Doc/HTML item\tCtrl-S");
    treeOper->Append(Id_DeleteAllItems, "DeleteAllItems");
    */

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar();
    // menuBar->Append(fileMenu, "&File");
    // menuBar->Append(treeStyle, "&Style");
    // menuBar->Append(treeOper, "&Operations");
    menuBar->Append(helpMenu, "&Help");

    // SetMenuBar(menuBar);

    CreateStatusBar(1);

    // Construct the image list with the standard images.
    InitImageList();

    // Create and layout child controls.
    m_treelist = CreateTreeListCtrl(wxTL_DEFAULT_STYLE);

    wxTextCtrl* textLog = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
    m_oldLogTarget = wxLog::SetActiveTarget(new wxLogTextCtrl(textLog));

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_treelist, wxSizerFlags(2).Expand());
    sizer->Add(textLog, wxSizerFlags(1).Expand());
    SetSizer(sizer);

    // Finally show everything.
    Show();
}

MyFrame::~MyFrame()
{
    delete m_imageList;
    delete wxLog::SetActiveTarget(m_oldLogTarget);
}

void MyFrame::InitImageList()
{
    wxSize iconSize = wxArtProvider::GetSizeHint(wxART_LIST, this);

    if (iconSize == wxDefaultSize)
    {
        iconSize = FromDIP(wxSize(16, 16));
    }

    m_imageList = new wxImageList(iconSize.x, iconSize.y);

    // The order should be the same as for the enum elements.
    static const wxString icons[] =
    {
        wxART_NORMAL_FILE,
        wxART_FOLDER,
        wxART_FOLDER_OPEN
    };

    for (unsigned n = 0; n < WXSIZEOF(icons); n++)
    {
        m_imageList->Add(wxArtProvider::GetIcon(icons[n], wxART_LIST, iconSize));
    }
}

wxTreeListItem add_item(wxTreeListCtrl* tree, wxTreeListItem parent, const std::string& name, const std::string& path)
{
    wxTreeListItem item = tree->AppendItem(parent, name);

    tree->SetItemText(item, Col_Files, path);

    return item;
}

void parse_directory(wxTreeListCtrl* tree, wxTreeListItem parent, const std::string& path)
{
    for (auto i = std::filesystem::directory_iterator(path); i != std::filesystem::directory_iterator(); i++)
    {
        std::string path = i->path().generic_string();
        std::string name = i->path().stem().generic_string();

        wxTreeListItem item = add_item(tree, parent, name, path);

        if (std::filesystem::is_directory(i->path()))
        {
            parse_directory(tree, item, path);
        }
    }
}

wxTreeListCtrl* MyFrame::CreateTreeListCtrl(long style)
{
    wxTreeListCtrl* tree = new wxTreeListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);

    tree->AppendColumn("Name", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
    tree->AppendColumn("Path", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);

    wxTreeListItem root = tree->GetRootItem();
    wxTreeListItem item = add_item(tree, root, "..", "..");

    parse_directory(tree, item, "..");

    return tree;
}

void MyFrame::RecreateTreeListCtrl(long style)
{
    wxTreeListCtrl* const treelist = CreateTreeListCtrl(style);
    GetSizer()->Replace(m_treelist, treelist);

    delete m_treelist;
    m_treelist = treelist;

    Layout();
}

void MyFrame::OnMultiSelect(wxCommandEvent& event)
{
    long style = m_treelist->GetWindowStyle();

    if ( event.IsChecked() )
        style |= wxTL_MULTIPLE;
    else
        style &= ~wxTL_MULTIPLE;

    RecreateTreeListCtrl(style);
}

void MyFrame::OnCheckboxes(wxCommandEvent& event)
{
    long style = m_treelist->GetWindowStyle();
    style &= ~(wxTL_CHECKBOX | wxTL_3STATE | wxTL_USER_3STATE);

    switch ( event.GetId() )
    {
        case Id_NoCheckboxes:
            break;

        case Id_Checkboxes2State:
            style |= wxTL_CHECKBOX;
            break;

        case Id_Checkboxes3State:
            style |= wxTL_3STATE;
            break;

        case Id_CheckboxesUser3State:
            style |= wxTL_USER_3STATE;
            break;

        default:
            wxFAIL_MSG( "Unknown checkbox style" );
            return;
    }

    RecreateTreeListCtrl(style);
}

void MyFrame::OnDumpSelection(wxCommandEvent& WXUNUSED(event))
{
    if ( m_treelist->HasFlag(wxTL_MULTIPLE) )
    {
        wxTreeListItems selections;
        const unsigned numSelected = m_treelist->GetSelections(selections);

        switch ( numSelected )
        {
            case 0:
                wxLogMessage("No items selected");
                break;

            case 1:
                wxLogMessage("Single item selected: %s",
                             DumpItem(selections[0]));
                break;

            default:
                wxLogMessage("%u items selected:", numSelected);
                for ( unsigned n = 0; n < numSelected; n++ )
                {
                    wxLogMessage("\t%s", DumpItem(selections[n]));
                }
        }
    }
    else // Single selection
    {
        wxLogMessage("Selection: %s", DumpItem(m_treelist->GetSelection()));
    }
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxAboutDialogInfo info;

    info.SetDescription("Gecko Editor");
    info.SetCopyright("(C) 2026 Golden Gecko <golden-gecko@holisz.pl>");

    wxAboutBox(info, this);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnDeleteAllItems(wxCommandEvent& event)
{
    m_treelist->DeleteAllItems();
}

wxString MyFrame::DumpItem(wxTreeListItem item) const
{
    return item.IsOk() ? m_treelist->GetItemText(item) : wxString("NONE");
}

const char* MyFrame::CheckedStateString(wxCheckBoxState state)
{
    switch ( state )
    {
        case wxCHK_UNCHECKED:
            return "unchecked";

        case wxCHK_UNDETERMINED:
            return "undetermined";

        case wxCHK_CHECKED:
            return "checked";
    }

    return "invalid";
}

void MyFrame::OnSelectionChanged(wxTreeListEvent& event)
{
    const char* msg;

    if ( m_treelist->HasFlag(wxTL_MULTIPLE) )
        msg = "Selection of the \"%s\" item changed.";
    else
        msg = "Selection changed, now is \"%s\".";

    wxLogMessage(msg, DumpItem(event.GetItem()));
}

void MyFrame::OnItemExpanding(wxTreeListEvent& event)
{
    wxLogMessage("Item \"%s\" is expanding", DumpItem(event.GetItem()));
}

void MyFrame::OnItemExpanded(wxTreeListEvent& event)
{
    wxLogMessage("Item \"%s\" expanded", DumpItem(event.GetItem()));
}

void MyFrame::OnItemChecked(wxTreeListEvent& event)
{
    wxTreeListItem item = event.GetItem();

    wxLogMessage("Item \"%s\" toggled, now %s (was %s)",
                 DumpItem(item),
                 CheckedStateString(m_treelist->GetCheckedState(item)),
                 CheckedStateString(event.GetOldCheckedState()));
}

void MyFrame::OnItemActivated(wxTreeListEvent& event)
{
    wxLogMessage("Item \"%s\" activated", DumpItem(event.GetItem()));
}

void MyFrame::OnItemContextMenu(wxTreeListEvent& event)
{
    enum
    {
        Id_Check_Item,
        Id_Uncheck_Item,
        Id_Indet_Item,
        Id_Check_Recursively,
        Id_Update_Parent
    };

    wxMenu menu;
    menu.Append(Id_Check_Item, "&Check item");
    menu.Append(Id_Uncheck_Item, "&Uncheck item");
    if ( m_treelist->HasFlag(wxTL_3STATE) )
        menu.Append(Id_Indet_Item, "Make item &indeterminate");
    menu.AppendSeparator();
    menu.Append(Id_Check_Recursively, "Check &recursively");
    menu.Append(Id_Update_Parent, "Update &parent");

    const wxTreeListItem item = event.GetItem();
    switch ( m_treelist->GetPopupMenuSelectionFromUser(menu) )
    {
        case Id_Check_Item:
            m_treelist->CheckItem(item);
            break;

        case Id_Uncheck_Item:
            m_treelist->UncheckItem(item);
            break;

        case Id_Indet_Item:
            m_treelist->CheckItem(item, wxCHK_UNDETERMINED);
            break;

        case Id_Check_Recursively:
            m_treelist->CheckItemRecursively(item);
            break;

        case Id_Update_Parent:
            m_treelist->UpdateItemParentStateRecursively(item);
            break;

        default:
            wxFAIL_MSG( "Unexpected menu selection" );
            wxFALLTHROUGH;

        case wxID_NONE:
            return;
    }
}
