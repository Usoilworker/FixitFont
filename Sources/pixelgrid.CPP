#include "pixelgrid.h"
#include <wx/dcclient.h>

wxBEGIN_EVENT_TABLE(PixelGrid, wxPanel)
EVT_PAINT(PixelGrid::OnPaint)
EVT_LEFT_DOWN(PixelGrid::OnMouseLeftDown)
EVT_SIZE(PixelGrid::OnSize)
wxEND_EVENT_TABLE()

PixelGrid::PixelGrid(wxPanel* parent)
    : wxPanel(parent) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            m_pixels[i][j] = false; // Initialize all pixels to off
        }
    }
}

PixelGrid::~PixelGrid() {
}

wxSize PixelGrid::DoGetBestSize() const {
    return wxSize(8 * 10, 8 * 10); // Fixed preferred size of 80x80
}

void PixelGrid::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxSize clientSize = GetClientSize();
    int pixelWidth = clientSize.GetWidth() / GRID_SIZE;
    int pixelHeight = clientSize.GetHeight() / GRID_SIZE;

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            wxColor color = m_pixels[i][j] ? wxColor(0, 0, 0) : wxColor(200, 200, 200);
            dc.SetBrush(wxBrush(color));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(j * pixelWidth, i * pixelHeight, pixelWidth, pixelHeight);
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
        m_pixels[row][col] = !m_pixels[row][col];
        Refresh(); // Trigger a repaint
    }
}

void PixelGrid::OnSize(wxSizeEvent& event) {
    Refresh(); // Trigger a repaint when the panel is resized
    event.Skip(); // Allow default size event handling
}