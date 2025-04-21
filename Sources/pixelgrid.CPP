#include "pixelgrid.h"
#include <wx/dcclient.h>

wxBEGIN_EVENT_TABLE(PixelGrid, wxPanel)
EVT_PAINT(PixelGrid::OnPaint)
EVT_LEFT_DOWN(PixelGrid::OnMouseLeftDown)
EVT_SIZE(PixelGrid::OnSize)
wxEND_EVENT_TABLE()

PixelGrid::PixelGrid(wxPanel* parent)
    : wxPanel(parent) {
    // Initialize character data to 0 (all pixels off)
    for (int i = 0; i < 8; ++i) {
        m_characterData[i] = 0;
    }
}

PixelGrid::~PixelGrid() {
}

wxSize PixelGrid::DoGetBestSize() const {
    return wxSize(8 * 10, 8 * 10); // Fixed preferred size of 80x80
}

void PixelGrid::SetCharacterData(const std::array<unsigned char, 8>& data) {
    m_characterData = data;
    Refresh(); // Trigger a repaint
}

void PixelGrid::GetCharacterData(std::array<unsigned char, 8>& data) const {
    data = m_characterData;
}

void PixelGrid::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxSize clientSize = GetClientSize();
    int pixelWidth = clientSize.GetWidth() / GRID_SIZE;
    int pixelHeight = clientSize.GetHeight() / GRID_SIZE;

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            if ((m_characterData[row] >> (7 - col)) & 0x01) { // Check if the bit is set
                dc.SetBrush(wxBrush(wxColor(0, 0, 0))); // Black
            }
            else {
                dc.SetBrush(wxBrush(wxColor(200, 200, 200))); // Light gray
            }
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(col * pixelWidth, row * pixelHeight, pixelWidth, pixelHeight);
        }
    }
}

void PixelGrid::OnMouseLeftDown(wxMouseEvent& event) {
    wxPoint mousePos = event.GetPosition();
    wxSize clientSize = GetClientSize();
    int pixelWidth = clientSize.GetWidth() / GRID_SIZE;
    int pixelHeight = clientSize.GetHeight() / GRID_SIZE;

    int col = mousePos.x / pixelWidth;
    int row = mousePos.y / pixelHeight;

    if (col >= 0 && col < GRID_SIZE && row >= 0 && row < GRID_SIZE) {
        m_characterData[row] ^= (1 << (7 - col)); // Toggle the bit
        Refresh();
    }
}

void PixelGrid::OnSize(wxSizeEvent& event) {
    Refresh();
    event.Skip();
}