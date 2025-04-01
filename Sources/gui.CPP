#include "gui.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    m_mainPanel = new wxPanel(this, wxID_ANY); // Create the main panel

    // Create the PixelGrid
    m_pixelGrid = new PixelGrid(m_mainPanel);

    // Create a sizer to manage the layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_pixelGrid, 1, wxALL | wxEXPAND, 10); // Add PixelGrid to the sizer
    m_mainPanel->SetSizer(mainSizer);

    // Set the sizer for the frame and fit the frame to the sizer
    SetSizer(mainSizer); // Set the sizer for the Frame
    Fit(); // Tell the Frame to use the sizer and fit
}

MainFrame::~MainFrame() {
}