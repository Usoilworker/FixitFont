#ifndef PIXELGRID_H
#define PIXELGRID_H

#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/event.h>

class PixelGrid : public wxPanel {
public:
    PixelGrid(wxPanel* parent);
    ~PixelGrid();

    wxSize DoGetBestSize() const override;

private:
    void OnPaint(wxPaintEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);

    wxDECLARE_EVENT_TABLE();

    static const int GRID_SIZE = 8;
    bool m_pixels[GRID_SIZE][GRID_SIZE];
};

#endif