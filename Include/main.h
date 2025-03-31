//main.h
#ifndef MAIN_H
#define MAIN_H

#include <wx/wx.h>

class TestFrame : public wxFrame {
public:
    TestFrame(const wxString& title);
};

class TestApp : public wxApp {
public:
    virtual bool OnInit();
};

#endif