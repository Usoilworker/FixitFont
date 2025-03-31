#include "gui.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    m_mainPanel = new wxPanel(this, wxID_ANY); // Create the main panel

    // Create a sizer to manage the layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    m_mainPanel->SetSizer(mainSizer);

    // Set the sizer for the frame and fit the frame to the sizer
    SetSizerAndFit(mainSizer);
}

MainFrame::~MainFrame() {
    // Destructor implementation (even if empty)
}