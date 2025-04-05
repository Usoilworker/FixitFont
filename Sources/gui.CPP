#include "gui.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    // Create a main panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);

    // Create the PixelGrid as a child of the main panel
    m_pixelGrid = new PixelGrid(mainPanel);

    // Create a sizer for the main panel
    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(m_pixelGrid, 0, wxALL, 10); // No expand for PixelGrid
    mainPanel->SetSizer(panelSizer);
    panelSizer->Fit(mainPanel); // Fit panel to PixelGrid's best size

    // Create a sizer for the frame and add the main panel
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(mainPanel, 0, wxALL | wxALIGN_TOP | wxALIGN_LEFT, 10); // No expand for panel, align top-left
    SetSizer(frameSizer);
    Fit();
    SetMinSize(GetSize());

    m_mainPanel = mainPanel; // Store the main panel
}

MainFrame::~MainFrame() {
}