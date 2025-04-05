#include "gui.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    // Create the main panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);

    // Create the PixelGrid as a child of the main panel
    m_pixelGrid = new PixelGrid(mainPanel);

    // Create a sizer to manage the layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_pixelGrid, 1, wxALL | wxEXPAND, 10); // Add PixelGrid to the sizer

    // Set the sizer for the main panel
    mainPanel->SetSizer(mainSizer);
    mainPanel->Fit(); // Fit the panel to its content

    // Set the sizer for the frame
    SetSizer(new wxBoxSizer(wxVERTICAL)); // Create a new sizer for the frame
    GetSizer()->Add(mainPanel, 1, wxALL | wxEXPAND, 0); // Add the main panel to the frame's sizer
    Fit(); // Tell the Frame to use the sizer and fit
    SetMinSize(GetSize()); // Optional: Set the initial size as the minimum size

    m_mainPanel = mainPanel; // Store the main panel (optional, but good practice if you need to refer to it later)
}

MainFrame::~MainFrame() {
}