#include "wx/wx.h"
#include "gui.h" // Must be first after wx/wx.h
#include <wx/spinctrl.h>
#include <array>
#include <bitset>
#include <wx/button.h>
#include <wx/filedlg.h>   // For wxFileDialog
#include <wx/wfstream.h>  // For wxFileOutputStream
#include <wx/txtstrm.h>   // For wxTextOutputStream
#include <wx/textfile.h>  // For wxTextFile, if needed for loading (not used in this save-only version)
#include <wx/stattext.h>  // For wxStaticText

// Event Table Definition
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_SPINCTRL(ID_CHAR_SELECTOR, MainFrame::OnCharSelected)      // Use specific ID
EVT_BUTTON(ID_CLEAR_BUTTON, MainFrame::OnClearButtonClicked)   // Use specific ID
EVT_BUTTON(ID_SAVE_BUTTON, MainFrame::OnSaveButtonClicked)     // Add event for Save button
// EVT_BUTTON(ID_LOAD_BUTTON, MainFrame::OnLoadButtonClicked)   // For future loading functionality
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size),
    m_currentCharacter(0),
    // Initialize member pointers to nullptr to prevent static analysis warnings and runtime issues
    m_mainPanel(nullptr),
    m_pixelGrid(nullptr),
    m_logWindow(nullptr),
    m_charSelector(nullptr),
    m_clearButton(nullptr),
    m_saveButton(nullptr),
    m_loadButton(nullptr)
{
    // Create a main panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);
    m_mainPanel = mainPanel; // Store the main panel pointer

    // Create the PixelGrid with a callback
    m_pixelGrid = new PixelGrid(mainPanel, [this]() { LogMessage("Pixel Grid Changed"); SaveCurrentCharacterData(); });

    // Create character selection control
    m_charSelector = new wxSpinCtrl(mainPanel, ID_CHAR_SELECTOR, wxString::Format("%d", m_currentCharacter), // Use specific ID
        wxDefaultPosition, wxDefaultSize, 0, 0, FontData::NUM_CHARACTERS - 1);

    // Create the clear button
    m_clearButton = new wxButton(mainPanel, ID_CLEAR_BUTTON, "Clear"); // Use specific ID

    // Create the save button
    m_saveButton = new wxButton(mainPanel, ID_SAVE_BUTTON, "Save"); // Use specific ID

    // Create the load button (declared but not added to sizer yet for future implementation)
    m_loadButton = new wxButton(mainPanel, ID_LOAD_BUTTON, "Load"); // Use specific ID

    // Create a horizontal sizer for the character selector and controls
    wxBoxSizer* controlsSizer = new wxBoxSizer(wxHORIZONTAL);
    controlsSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Character:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); // Label
    controlsSizer->Add(m_charSelector, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlsSizer->Add(m_clearButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlsSizer->Add(m_saveButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); // Now visible and clickable
    // controlsSizer->Add(m_loadButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); // Uncomment when implementing Load
    controlsSizer->AddStretchSpacer(1); // Push controls to the left

    // Create the log window (console)
    m_logWindow = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 100), wxTE_MULTILINE | wxTE_READONLY); // Correctly assign to member

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

    // Initialize some font data for testing
    std::array<std::bitset<FontData::PATTERN_BITS_PER_ROW>, FontData::PATTERN_ROWS> charA_pattern = {
        std::bitset<FontData::PATTERN_BITS_PER_ROW>("00011000"), std::bitset<FontData::PATTERN_BITS_PER_ROW>("00100100"),
        std::bitset<FontData::PATTERN_BITS_PER_ROW>("01000010"), std::bitset<FontData::PATTERN_BITS_PER_ROW>("01111110"),
        std::bitset<FontData::PATTERN_BITS_PER_ROW>("10000001"), std::bitset<FontData::PATTERN_BITS_PER_ROW>("10000001"),
        std::bitset<FontData::PATTERN_BITS_PER_ROW>("10000001"), std::bitset<FontData::PATTERN_BITS_PER_ROW>("00000000")
    };
    for (int i = 0; i < FontData::PATTERN_ROWS; ++i) {
        m_fontData.characterPatterns[65][i] = charA_pattern[i]; // Store 'A' at index 65 (ASCII)
    }

    LoadCharacterDataToGrid(); // Load the initially selected character's data

    LogMessage("Font Generator Started.");
}

MainFrame::~MainFrame() {
    // Destructor implementation (empty is fine if no resources need explicit freeing)
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
    for (int i = 0; i < FontData::PATTERN_ROWS; ++i) {
        std::bitset<FontData::PATTERN_BITS_PER_ROW> rowBits = m_fontData.characterPatterns[m_currentCharacter][i];

        unsigned char byte = 0;
        for (int bit = 0; bit < FontData::PATTERN_BITS_PER_ROW; ++bit) {
            if (rowBits[FontData::PATTERN_BITS_PER_ROW - 1 - bit]) {
                byte |= (1 << (FontData::PATTERN_BITS_PER_ROW - 1 - bit));
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
        m_pixelGrid->GetCharacterData(charData);

        wxString logMessage = wxString::Format("Saving character %d: ", m_currentCharacter);
        for (unsigned char val : charData) {
            logMessage += wxString::Format("%02X ", val);
        }
        LogMessage(logMessage);

        for (int i = 0; i < FontData::PATTERN_ROWS; ++i) {
            std::bitset<FontData::PATTERN_BITS_PER_ROW> rowBits(charData[i]);
            m_fontData.characterPatterns[m_currentCharacter][i] = rowBits;
        }
    }
}

void MainFrame::OnCharSelected(wxSpinEvent& event) {
    SaveCurrentCharacterData();
    m_currentCharacter = event.GetInt();
    LoadCharacterDataToGrid();
}

void MainFrame::LogMessage(const wxString& message) {
    if (m_logWindow) {
        m_logWindow->AppendText(message + "\n");
        m_logWindow->ShowPosition(m_logWindow->GetLastPosition());
    }
}

void MainFrame::OnSaveButtonClicked(wxCommandEvent& event) {
    LogMessage("Save button clicked. Opening file dialog...");

    wxFileDialog saveFileDialog(this, _("Save Font File"), "", "myfont.fnt",
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

    wxFileOutputStream output(filePath);
    if (!output.IsOk()) {
        LogMessage(wxString::Format("Error: Could not open file for writing: %s", filePath));
        return;
    }

    wxTextOutputStream textOutput(output);
    for (int charIdx = 0; charIdx < FontData::NUM_CHARACTERS; ++charIdx) {
        textOutput << "char " << charIdx << ":\n";
        for (int row = 0; row < FontData::PATTERN_ROWS; ++row) {
            std::bitset<FontData::PATTERN_BITS_PER_ROW> rowBits = m_fontData.characterPatterns[charIdx][row];
            textOutput << rowBits.to_string() << "\n";
        }
    }
    LogMessage(wxString::Format("Font successfully saved to: %s", filePath));
}