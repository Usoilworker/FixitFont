// pixelgrid.cpp
#include "pixelgrid.h"

wxBEGIN_EVENT_TABLE(PixelGrid, wxPanel)
EVT_PAINT(PixelGrid::OnPaint)
EVT_LEFT_DOWN(PixelGrid::OnMouseLeftDown)
wxEND_EVENT_TABLE()

PixelGrid::PixelGrid(wxPanel* parent)
    : wxPanel(parent, wxID_ANY) {
    // Initialize pixel data to false (off)
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            m_pixels[row][col] = false;
        }
    }
}

PixelGrid::~PixelGrid() {
}

wxSize PixelGrid::DoGetBestSize() const {
    return wxSize(GRID_SIZE * 10, GRID_SIZE * 10); // Each pixel is 20x20
}

void PixelGrid::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

    // Get the client size of the panel
    wxSize clientSize = GetClientSize();
    int pixelSizeX = clientSize.GetWidth() / GRID_SIZE;
    int pixelSizeY = clientSize.GetHeight() / GRID_SIZE;
    int pixelSize = wxMin(pixelSizeX, pixelSizeY); // Use the smaller dimension to prevent distortion

    // Draw the grid
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            wxColour color = m_pixels[row][col] ? wxColour(0, 0, 0) : wxColour(200, 200, 200); // Black or light gray
            wxBrush brush(color);
            dc.SetBrush(brush);
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(col * pixelSize, row * pixelSize, pixelSize, pixelSize);
        }
    }
}

void PixelGrid::OnMouseLeftDown(wxMouseEvent& event) {
    // Get the mouse coordinates
    wxPoint mousePos = event.GetPosition();

    // Calculate the row and column of the clicked pixel
    wxSize clientSize = GetClientSize();
    int pixelSizeX = clientSize.GetWidth() / GRID_SIZE;
    int pixelSizeY = clientSize.GetHeight() / GRID_SIZE;
    int pixelSize = wxMin(pixelSizeX, pixelSizeY);

    int col = mousePos.x / pixelSize;
    int row = mousePos.y / pixelSize;

    // Toggle the pixel state
    if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
        m_pixels[row][col] = !m_pixels[row][col];
        Refresh(); // Trigger a repaint to update the display
    }
}