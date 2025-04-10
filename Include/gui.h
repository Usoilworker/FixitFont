// FixitFont GUI.h Header
#ifndef GUI_H
#define GUI_H

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include "pixelgrid.h" // Include the PixelGrid header

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MainFrame();

private:
    wxPanel* m_mainPanel; // Main panel to hold our UI elements
    PixelGrid* m_pixelGrid; // Pointer to the PixelGrid

    wxDECLARE_EVENT_TABLE();
};

#endif