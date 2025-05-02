#ifndef PIXELGRID_H
#define PIXELGRID_H

#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/event.h>
#include <array>
#include <functional> // For std::function

class PixelGrid : public wxPanel {
public:
    PixelGrid(wxPanel* parent, std::function<void()> onPixelChanged);
    ~PixelGrid();

    void SetCharacterData(const std::array<unsigned char, 8>& data);
    void GetCharacterData(std::array<unsigned char, 8>& data) const;
    void SetRowData(int row, unsigned char data);// Added for testing
    wxSize DoGetBestSize() const override;

private:
    void OnPaint(wxPaintEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnSize(wxSizeEvent& event);

    wxDECLARE_EVENT_TABLE();

    static const int GRID_SIZE = 8;
    std::array<unsigned char, 8> m_characterData;
    std::function<void()> m_onPixelChanged; // Callback function
};

#endif