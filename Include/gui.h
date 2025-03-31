// FixitFont GUI.h Header
#ifndef GUI_H
#define GUI_H

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MainFrame();

private:
    wxPanel* m_mainPanel; // Main panel to hold our UI elements

    wxDECLARE_EVENT_TABLE();
};

#endif