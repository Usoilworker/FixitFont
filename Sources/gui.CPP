// GUI.cpp
#include "wx/wx.h"
#include "gui.h"
#include <wx/spinctrl.h> // For character selection control

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_SPINCTRL(wxID_ANY, MainFrame::OnCharSelected) // Handle character selection
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size), m_currentCharacter(0) {
    // Create a main panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);

    // Create the PixelGrid as a child of the main panel
    m_pixelGrid = new PixelGrid(mainPanel);

    // Create character selection control
    wxSpinCtrl* charCtrl = new wxSpinCtrl(mainPanel, wxID_ANY, wxString::Format("%d", m_currentCharacter),
        wxDefaultPosition, wxDefaultSize, 0, 0, FontData::NUM_CHARACTERS - 1);

    // Create a sizer for the main panel
    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(m_pixelGrid, 0, wxALL, 10);
    panelSizer->Add(charCtrl, 0, wxALL, 10); // Add character selection control
    mainPanel->SetSizer(panelSizer);
    panelSizer->Fit(mainPanel);

    // Create a sizer for the frame and add the main panel
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(mainPanel, 1, wxALL | wxEXPAND, 10); // Expand the panel
    SetSizer(frameSizer);
    Fit();

    m_mainPanel = mainPanel; // Store the main panel

    // Initialize PixelGrid with data for the first character
    std::array<unsigned char, 8> initialCharData;
    for (int i = 0; i < 8; ++i) {
        unsigned char byte = 0;
        for (int bit = 0; bit < 8; ++bit) {
            if (m_fontData.characterPatterns[m_currentCharacter][bit]) {
                byte |= (1 << (7 - bit));
            }
        }
        initialCharData[i] = byte;
    }
    m_pixelGrid->SetCharacterData(initialCharData);
}

MainFrame::~MainFrame() {
    // Destructor implementation (even if empty)
}

void MainFrame::OnCharSelected(wxSpinEvent& event) {
    m_currentCharacter = event.GetInt();
    std::array<unsigned char, 8> charData;
    for (int i = 0; i < 8; ++i) {
        unsigned char byte = 0;
        for (int bit = 0; bit < 8; ++bit) {
            if (m_fontData.characterPatterns[m_currentCharacter][bit]) {
                byte |= (1 << (7 - bit)); // Set the bit in the unsigned char
            }
        }
        charData[i] = byte;
    }
    m_pixelGrid->SetCharacterData(charData);
}