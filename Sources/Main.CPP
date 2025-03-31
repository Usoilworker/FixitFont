
#include "main.h"

TestFrame::TestFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)) {
    // Add a simple text control
    wxTextCtrl* textCtrl = new wxTextCtrl(this, wxID_ANY, "Hello, wxWidgets!");
    // Center the text control within the frame
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(textCtrl, 1, wxEXPAND | wxALL, 10);
    SetSizer(sizer);
    Centre();
}

bool TestApp::OnInit() {
    TestFrame* frame = new TestFrame("Test wxWidgets Window");
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(TestApp);