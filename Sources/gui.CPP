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
#include <wx/tokenzr.h>

// Event Table Definition
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_SPINCTRL(ID_CHAR_SELECTOR, MainFrame::OnCharSelected)      // Use specific ID
EVT_BUTTON(ID_CLEAR_BUTTON, MainFrame::OnClearButtonClicked)   // Use specific ID
EVT_BUTTON(ID_SAVE_BUTTON, MainFrame::OnSaveButtonClicked)     // Add event for Save button
EVT_BUTTON(ID_LOAD_BUTTON, MainFrame::OnLoadButtonClicked)   // For future loading functionality
EVT_BUTTON(ID_UNDO_BUTTON, MainFrame::OnUndoButtonClicked) // Handler for undo button
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
    // Create the undo button
	m_undoButton = new wxButton(mainPanel, ID_UNDO_BUTTON, "Undo"); 
    // Create the clear button
    m_clearButton = new wxButton(mainPanel, ID_CLEAR_BUTTON, "Clear"); 

    // Create the save button
    m_saveButton = new wxButton(mainPanel, ID_SAVE_BUTTON, "Save"); 

    // Create the load button (declared but not added to sizer yet for future implementation)
    m_loadButton = new wxButton(mainPanel, ID_LOAD_BUTTON, "Load"); 

    // Create a horizontal sizer for the character selector and controls
    wxBoxSizer* controlsSizer = new wxBoxSizer(wxHORIZONTAL);
    controlsSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Character:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); // Label
    controlsSizer->Add(m_charSelector, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	controlsSizer->Add(m_undoButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); 
    controlsSizer->Add(m_clearButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlsSizer->Add(m_saveButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); 
    controlsSizer->Add(m_loadButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5); 
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

	m_LoadedOriginalFontData = m_fontData; // Store the original font data for undo functionality

    LogMessage("Font Generator Started.");
}

MainFrame::~MainFrame() {
    // Destructor implementation (empty is fine if no resources need explicit freeing)
}

void MainFrame::OnUndoButtonClicked(wxCommandEvent& event) {
	// This function will be used to undo changes to the current character data
	// For now, it just logs a message
	LogMessage(wxString::Format("Undo button clicked for character %d.", m_currentCharacter));
	// Implement undo logic here if needed in the future
	if (m_pixelGrid) {
		
        m_fontData.characterPatterns[m_currentCharacter] = m_LoadedOriginalFontData.characterPatterns[m_currentCharacter];

		LoadCharacterDataToGrid();
		LogMessage(wxString::Format("Changes undone for character %d.", m_currentCharacter));
	}
	else {
		LogMessage("Pixel Grid not initialized, cannot undo changes.");
	}
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
	m_LoadedOriginalFontData = m_fontData; // Update the original font data after saving
}

void MainFrame::LoadFromFile(const wxString& filePath) {
    LogMessage(wxString::Format("Attempting to load font from: %s", filePath));

    wxFileInputStream input(filePath);
    if (!input.IsOk()) {
        LogMessage(wxString::Format("Error: Could not open file for reading: %s", filePath));
        return;
    }

    wxTextInputStream textInput(input);
    bool errorOccurred = false;
    int charIdx = 0; // <--- DECLARE charIdx HERE, initialize to 0

    // Loop to read character data
    for (/* charIdx is already initialized */; charIdx < FontData::NUM_CHARACTERS; ++charIdx) {
        // MODIFIED Eof check:
        if (textInput.GetInputStream().Eof()) {
            LogMessage(wxString::Format("Info: End of file reached. Attempted to read up to character %d.", charIdx));
            break;
        }

        wxString headerLine = textInput.ReadLine();
        // MODIFIED Eof check (if you also want to check after ReadLine, which is good practice):
        if (textInput.GetInputStream().Eof() && headerLine.IsEmpty()) {
            LogMessage(wxString::Format("Info: End of file reached after reading data for %d characters.", charIdx));
            break;
        }

        wxString expectedHeaderStart = wxString::Format("char %d:", charIdx);
        if (!headerLine.StartsWith(expectedHeaderStart)) {
            LogMessage(wxString::Format("Error: Malformed file. Expected '%s' but got '%s' for character %d.",
                expectedHeaderStart, headerLine, charIdx));
            errorOccurred = true;
            break;
        }

        for (int row = 0; row < FontData::PATTERN_ROWS; ++row) {
            // MODIFIED Eof check:
            if (textInput.GetInputStream().Eof()) {
                LogMessage(wxString::Format("Error: Unexpected end of file while reading row %d for character %d.", row, charIdx));
                errorOccurred = true;
                break;
            }
            wxString bitString = textInput.ReadLine();
            if (bitString.length() != FontData::PATTERN_BITS_PER_ROW) {
                LogMessage(wxString::Format("Error: Malformed row data for char %d, row %d. Expected %d bits, got %d.",
                    charIdx, row, FontData::PATTERN_BITS_PER_ROW, (int)bitString.length()));
                errorOccurred = true;
                break;
            }

            for (wxChar c : bitString) {
                if (c != '0' && c != '1') {
                    LogMessage(wxString::Format("Error: Invalid character '%c' in bit string for char %d, row %d.", c, charIdx, row));
                    errorOccurred = true;
                    break;
                }
            }
            if (errorOccurred) break;

            try {
                m_fontData.characterPatterns[charIdx][row] = std::bitset<FontData::PATTERN_BITS_PER_ROW>(bitString.ToStdString());
            }
            catch (const std::invalid_argument& e) {
                LogMessage(wxString::Format("Error: Could not parse bit string '%s' for char %d, row %d. Exception: %s", bitString, charIdx, row, e.what()));
                errorOccurred = true;
                break;
            }
        } // End of row loop
        if (errorOccurred) break; // Exit char loop if error in row loop
    } // End of character loop

    // Logging based on outcome
    if (errorOccurred) {
        LogMessage(wxString::Format("Font loading failed due to errors while processing character %d. Font data might be incomplete or corrupted.", charIdx));
        // You might want to clear m_fontData or reload a default state here
        // m_fontData = FontData(); // Example: revert to a blank font
    }
    else if (charIdx < FontData::NUM_CHARACTERS) {
        // This means the loop exited due to EOF and not all characters were present in the file.
        LogMessage(wxString::Format("Font loaded. File contained data for %d characters (processed up to character index %d). Remaining characters are unchanged or default.", charIdx, charIdx > 0 ? charIdx - 1 : 0));
        // Optionally, clear the remainder of m_fontData if partial files should mean the rest is blank:
        // for (int i = charIdx; i < FontData::NUM_CHARACTERS; ++i) {
        //    for (int j = 0; j < FontData::PATTERN_ROWS; ++j) {
        //        m_fontData.characterPatterns[i][j].reset();
        //    }
        // }
    }
    else {
        LogMessage(wxString::Format("Font successfully loaded from: %s (%d characters processed).", filePath, charIdx));
		m_LoadedOriginalFontData = m_fontData; // Update the original font data after loading
    }

    // Refresh the PixelGrid with the data of the currently selected character
    LoadCharacterDataToGrid();
}


void MainFrame::OnLoadButtonClicked(wxCommandEvent& event) {
    LogMessage("Load button clicked. Opening file dialog...");

    wxFileDialog openFileDialog(this, _("Open Font File"), "", "",
        _("Font Files (*.fnt)|*.fnt|All files (*.*)|*.*"),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString path = openFileDialog.GetPath();
        LoadFromFile(path);
    }
    else {
        LogMessage("Load operation cancelled by user.");
    }
}