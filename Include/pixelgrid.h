#ifndef PIXELGRID_H
#define PIXELGRID_H

#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/event.h>
#include <array> // For std::array

class PixelGrid : public wxPanel {
public:
    PixelGrid(wxPanel* parent);
    ~PixelGrid();

    void SetCharacterData(const std::array<unsigned char, 8>& data);
    void GetCharacterData(std::array<unsigned char, 8>& data) const;

    wxSize DoGetBestSize() const override;

private:
    void OnPaint(wxPaintEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnSize(wxSizeEvent& event);

    wxDECLARE_EVENT_TABLE();

    static const int GRID_SIZE = 8;
    std::array<unsigned char, 8> m_characterData; // 8 bytes to store the character data
};

#endif