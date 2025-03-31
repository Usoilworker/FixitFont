#include "wx/wx.h"
#include "gui.h" // Include the header for our GUI frame
#include "main.h" // Include the main application header

wxIMPLEMENT_APP(FontGeneratorApp);

bool FontGeneratorApp::OnInit() {
    MainFrame* frame = new MainFrame("Font Generator", wxPoint(50, 50), wxSize(800, 600));
    frame->Move(wxPoint(100, 100));
    frame->SetClientSize(800, 600); // Set size again
    frame->Show(true);
    return true;
}