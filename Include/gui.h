// FixitFont GUI.h Header
#ifndef GUI_H
#define GUI_H

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>    // For wxTextCtrl (m_logWindow)
#include <wx/stattext.h>    // For wxStaticText (Character: label)
#include <wx/spinctrl.h>    // For wxSpinCtrl (m_charSelector)
#include <wx/button.h>      // For wxButton (m_clearButton, m_saveButton, m_loadButton)

#include "pixelgrid.h"
#include "fontdata.h"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MainFrame();

private:
    // Member controls (pointers are good practice for controls owned by the frame)
    wxPanel* m_mainPanel;
    PixelGrid* m_pixelGrid;
    wxTextCtrl* m_logWindow;
    wxSpinCtrl* m_charSelector;
    wxButton* m_clearButton;
    wxButton* m_saveButton;
    wxButton* m_loadButton;

    // Data members
    FontData m_fontData;
    int m_currentCharacter;

    // Helper functions (private member functions)
    void SaveCurrentCharacterData();
    void LoadCharacterDataToGrid();
    void LogMessage(const wxString& message);
    void SaveToFile(const wxString& filePath);

    // Event Handlers
    void OnCharSelected(wxSpinEvent& event);
    void OnClearButtonClicked(wxCommandEvent& event);
    void OnSaveButtonClicked(wxCommandEvent& event);
    void OnLoadButtonClicked(wxCommandEvent& event); // For future loading functionality

    wxDECLARE_EVENT_TABLE();

    // Custom IDs for controls (inside the class, or globally if outside)
    enum {
        ID_CHAR_SELECTOR = wxID_HIGHEST + 1, // Specific ID for char selector
        ID_CLEAR_BUTTON = wxID_HIGHEST + 2,
        ID_SAVE_BUTTON = wxID_HIGHEST + 3,
        ID_LOAD_BUTTON = wxID_HIGHEST + 4
    };
};

#endif