// GUI.cpp
#include "wx/wx.h"
#include "gui.h"
#include <wx/spinctrl.h>
#include <array>
#include <bitset>
#include <wx/button.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

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

    m_mainPanel = mainPanel; // Store the main panel

    // Initialize some font data for testing
    // Note: charA_pattern should also be a std::array<std::bitset<8>, 8>
    std::array<std::bitset<FontData::PATTERN_BITS_PER_ROW>, FontData::PATTERN_ROWS> charA_pattern = {
        std::bitset<8>("00011000"), std::bitset<8>("00100100"),
        std::bitset<8>("01000010"), std::bitset<8>("01111110"),
        std::bitset<8>("10000001"), std::bitset<8>("10000001"),
        std::bitset<8>("10000001"), std::bitset<8>("00000000")
    };
    for (int i = 0; i < FontData::PATTERN_ROWS; ++i) { // Loop 8 times for the 8 rows
        // Assign the entire std::bitset for each row
        m_fontData.characterPatterns[65][i] = charA_pattern[i]; // Store 'A' at index 65 (ASCII)
    }

    LoadCharacterDataToGrid(); // Load the initially selected character's data

    LogMessage("Font Generator Started.");
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
    // Iterate through the 8 rows of the current character
    for (int i = 0; i < FontData::PATTERN_ROWS; ++i) { // Loop through the 8 rows
        // Access the std::bitset for the current character's current row
        // Indexing is now characterPatterns[character_index][row_index]
        std::bitset<FontData::PATTERN_BITS_PER_ROW> rowBits = m_fontData.characterPatterns[m_currentCharacter][i];

        unsigned char byte = 0;
        // Convert the bitset row to an unsigned char
        for (int bit = 0; bit < FontData::PATTERN_BITS_PER_ROW; ++bit) {
            // Check if the bit is set (from left to right in bitset, corresponds to MSB to LSB in byte)
            // bitset<N> index is 0 for LSB, N-1 for MSB
            // For drawing, we usually want bit 0 to be the leftmost pixel, bit 7 to be the rightmost.
            // If bitset<8>[0] is the rightmost, and bitset<8>[7] is the leftmost:
            if (rowBits[FontData::PATTERN_BITS_PER_ROW - 1 - bit]) { // Read bit from MSB to LSB
                byte |= (1 << (FontData::PATTERN_BITS_PER_ROW - 1 - bit)); // Set corresponding bit in byte
            }
        }
        charData[i] = byte;
    }

    wxString logMessage = wxString::Format("Loaded character %d: ", m_currentCharacter);
    for (unsigned char val : charData) {
        logMessage += wxString::Format("%02X ", val);
    }
    LogMessage(logMessage);

    m_pixelGrid->SetCharacterData(charData);
    LogMessage(wxString::Format("Loaded character %d into grid.", m_currentCharacter));
}

void MainFrame::SaveCurrentCharacterData() {
    if (m_pixelGrid) {
        std::array<unsigned char, 8> charData;
        m_pixelGrid->GetCharacterData(charData); // Get data from the pixel grid

        wxString logMessage = wxString::Format("Saving character %d: ", m_currentCharacter);
        for (unsigned char val : charData) {
            logMessage += wxString::Format("%02X ", val);
        }
        LogMessage(logMessage);

        // Iterate through the 8 rows of the current character
        for (int i = 0; i < FontData::PATTERN_ROWS; ++i) {
            // Convert the unsigned char row to an std::bitset
            std::bitset<FontData::PATTERN_BITS_PER_ROW> rowBits(charData[i]); // Initialize bitset from unsigned char
            m_fontData.characterPatterns[m_currentCharacter][i] = rowBits; // Assign the bitset to the correct location
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

void MainFrame::OnSaveButtonClicked(wxCommandEvent& event) {
    LogMessage("Save button clicked. Opening file dialog...");

    wxFileDialog saveFileDialog(this, _("Save Font File"), "", "myfont.fnt", // Default filename
        _("Font Files (*.fnt)|*.fnt|All files (*.*)|*.*"),
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString path = saveFileDialog.GetPath();
        SaveToFile(path);
    }
    else {
        LogMessage("Save operation cancelled by user.");
    }
}

void MainFrame::SaveToFile(const wxString& filePath) {
    LogMessage(wxString::Format("Attempting to save font to: %s", filePath));

    wxFileOutputStream output(filePath); // Open the file for writing
    if (!output.IsOk()) {
        LogMessage(wxString::Format("Error: Could not open file for writing: %s", filePath));
        return; // Exit if file could not be opened
    }

    // Declare and initialize wxTextOutputStream *only if* output is valid
    wxTextOutputStream textOutput(output);

    // ... rest of the function, which is now guaranteed to run only if 'output' is valid ...
    for (int charIdx = 0; charIdx < FontData::NUM_CHARACTERS; ++charIdx) {
        textOutput << "char " << charIdx << ":\n"; // Write character header

        for (int row = 0; row < FontData::PATTERN_ROWS; ++row) {
            std::bitset<FontData::PATTERN_BITS_PER_ROW> rowBits = m_fontData.characterPatterns[charIdx][row];
            textOutput << rowBits.to_string() << "\n"; // Write bitset as "01011000" string
        }
    }
    LogMessage(wxString::Format("Font successfully saved to: %s", filePath));
}