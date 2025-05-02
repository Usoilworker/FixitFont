// FixitFont GUI.h Header
#ifndef GUI_H
#define GUI_H

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include "pixelgrid.h"
#include "fontdata.h"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MainFrame();

private:
    wxPanel* m_mainPanel;
    PixelGrid* m_pixelGrid;
    FontData m_fontData;
    int m_currentCharacter;
    wxTextCtrl* m_logWindow; // New text control for the console
    wxSpinCtrl* m_charSelector;
    wxButton* m_clearButton; // New button to clear the grid

    void SaveCurrentCharacterData();
    void LoadCharacterDataToGrid();
    void OnCharSelected(wxSpinEvent& event);
    void LogMessage(const wxString& message); // Helper function for logging
    void OnClearButtonClicked(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif