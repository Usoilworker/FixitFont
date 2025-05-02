// GUI.cpp
#include "wx/wx.h"
#include "gui.h"
#include <wx/spinctrl.h>
#include <array>
#include <bitset> // Make sure this is included

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_SPINCTRL(wxID_ANY, MainFrame::OnCharSelected) // Handle character selection
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size), m_currentCharacter(0) {
    // Create a main panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);

    // Create the PixelGrid with a callback
    m_pixelGrid = new PixelGrid(mainPanel, [this]() { SaveCurrentCharacterData(); });

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

    m_mainPanel = mainPanel;

    // Initialize PixelGrid with data for the first character
    LoadCharacterDataToGrid();
}

MainFrame::~MainFrame() {
    // Destructor implementation (even if empty)
}

void MainFrame::LoadCharacterDataToGrid() {
    for (int i = 0; i < 8; ++i) {
        std::bitset<8> rowBits = m_fontData.characterPatterns[m_currentCharacter * 8 + i];
        unsigned char byte = 0;
        for (int bit = 0; bit < 8; ++bit) {
            if (rowBits[bit]) {
                byte |= (1 << (7 - bit));
            }
        }
        m_pixelGrid->SetRowData(i, byte); // Use the new setter
    }
}

void MainFrame::SaveCurrentCharacterData() {
    if (m_pixelGrid) {
        std::array<unsigned char, 8> charData;
        m_pixelGrid->GetCharacterData(charData);
        wxString logMessage = wxString::Format("Saving char %d: ", m_currentCharacter);
        for (unsigned char val : charData) {
            logMessage += wxString::Format("%02X ", val);
        }
        wxLogMessage(logMessage); // Output to the debug log

        for (int i = 0; i < 8; ++i) {
            std::bitset<8> rowBits(charData[i]);
            m_fontData.characterPatterns[m_currentCharacter * 8 + i] = rowBits;
        }
    }
}


void MainFrame::OnCharSelected(wxSpinEvent& event) {
    SaveCurrentCharacterData(); // Save the data of the previously selected character
    m_currentCharacter = event.GetInt();
    LoadCharacterDataToGrid(); // Load the data for the newly selected character
}