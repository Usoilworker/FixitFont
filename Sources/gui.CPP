// GUI.cpp
#include "wx/wx.h"
#include "gui.h"
#include <wx/spinctrl.h>
#include <array>
#include <bitset>
#include <wx/button.h>

// Event Table Defenition
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_SPINCTRL(wxID_ANY, MainFrame::OnCharSelected) // Handle character selection
EVT_BUTTON(wxID_ANY, MainFrame::OnClearButtonClicked)
wxEND_EVENT_TABLE()

// As stated above end of event table

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size), m_currentCharacter(0) {
    // Create a main panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);

    // Create the PixelGrid with a callback
    m_pixelGrid = new PixelGrid(mainPanel, [this]() { LogMessage("Pixel Grid Changed"); SaveCurrentCharacterData(); });

    // Create character selection control
    wxSpinCtrl* charCtrl = new wxSpinCtrl(mainPanel, wxID_ANY, wxString::Format("%d", m_currentCharacter),
        wxDefaultPosition, wxDefaultSize, 0, 0, FontData::NUM_CHARACTERS - 1);
    m_charSelector = charCtrl; // Store the pointer

    // Create the clear button
    wxButton* clearButton = new wxButton(mainPanel, wxID_ANY, "Clear");
    m_clearButton = clearButton; // Store the button pointer

    // Create a horizontal sizer for the character selector and clear button
    wxBoxSizer* controlsSizer = new wxBoxSizer(wxHORIZONTAL);
    controlsSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Character:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); // Label
    controlsSizer->Add(m_charSelector, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlsSizer->Add(m_clearButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlsSizer->AddStretchSpacer(1); // Push controls to the left

    // Create the log window (console)
    wxTextCtrl* logWindow = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 100), wxTE_MULTILINE | wxTE_READONLY);
    m_logWindow = logWindow; // Actually create and store the log window

    // Create a vertical sizer for the main panel
    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(m_pixelGrid, 0, wxALL, 10); // Pixel Grid at the top
    panelSizer->Add(controlsSizer, 0, wxALL | wxEXPAND, 5); // Controls below, expand horizontally
    panelSizer->Add(m_logWindow, 1, wxALL | wxEXPAND, 10); // Log window at the bottom
    mainPanel->SetSizer(panelSizer);
    panelSizer->Fit(mainPanel);

    // Create a sizer for the frame and add the main panel
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(mainPanel, 1, wxALL | wxEXPAND, 10);
    SetSizer(frameSizer);
    Fit();
    SetMinSize(GetSize());

    m_mainPanel = mainPanel;

    // Initialize PixelGrid with data for the first character
    LoadCharacterDataToGrid();

    LogMessage("Font Generator Started."); // Initial log message
}

MainFrame::~MainFrame() {
    // Destructor implementation (even if empty)
}


void MainFrame::OnClearButtonClicked(wxCommandEvent& event) {
    if (m_pixelGrid) {
        std::array<unsigned char, 8> clearData;
        for (int i = 0; i < 8; ++i) {
            clearData[i] = 0; // Fill with all off pixels
        }
        m_pixelGrid->SetCharacterData(clearData);
        LogMessage(wxString("Pixel Grid Cleared."));
        SaveCurrentCharacterData(); // Save the cleared state
        LoadCharacterDataToGrid(); // Refresh the grid display
    }
}
void MainFrame::LoadCharacterDataToGrid() {
    std::array<unsigned char, 8> charData;
    for (int i = 0; i < 8; ++i) {
        unsigned char byte = 0;
        std::bitset<8> rowBits = m_fontData.characterPatterns[m_currentCharacter * 8 + i];
        for (int bit = 0; bit < 8; ++bit) {
            if (rowBits[bit]) {
                byte |= (1 << bit); // Ensure this is the correct bit order
            }
        }
        charData[i] = byte;
    }

    wxString logMessage = wxString::Format("Loaded character %d: ", m_currentCharacter);
    for (unsigned char val : charData) {
        logMessage += wxString::Format("%02X ", val);
    }
    LogMessage(logMessage); // Output the loaded data to the console

    m_pixelGrid->SetCharacterData(charData);
    LogMessage(wxString::Format("Loaded character %d into grid.", m_currentCharacter));
}
void MainFrame::SaveCurrentCharacterData() {
    if (m_pixelGrid) {
        std::array<unsigned char, 8> charData;
        m_pixelGrid->GetCharacterData(charData);
        wxString logMessage = wxString::Format("Saving character %d: ", m_currentCharacter);
        for (unsigned char val : charData) {
            logMessage += wxString::Format("%02X ", val);
        }
        LogMessage(logMessage);

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

void MainFrame::LogMessage(const wxString& message) {
    if (m_logWindow) {
        m_logWindow->AppendText(message + "\n");
    }
}