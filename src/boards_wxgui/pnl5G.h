#pragma once

#include "lime/LimeSuite.h"
#include <map>
#include <wx/panel.h>
#include <wx/choice.h>
class wxStaticText;
class wxFlexGridSizer;
class wxCheckBox;

class pnlCoreSDR5G : public wxPanel
{
public:
    pnlCoreSDR5G(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, int style = 0, wxString name = wxEmptyString);
    void Initialize(lms_device_t *pControl);
    virtual ~pnlCoreSDR5G();
    virtual void UpdatePanel();

    void OnReadAll(wxCommandEvent &event);
    void OnWriteAll(wxCommandEvent &event);

protected:
    void OnLoopbackChange(wxCommandEvent &event);
    void OnLoopbackChange2(wxCommandEvent &event);
    wxChoice* cmbRx1Path;
    wxChoice* cmbTx1Path;
    wxCheckBox* tx11En;
    wxCheckBox* tx12En;
    wxCheckBox* tx2En;
    wxChoice* cmbTRx1Path;
    wxChoice* cmbTRx2Path;
    wxChoice* cmbRx2Path;
    wxChoice* cmbRxPathAB;
    wxChoice* cmbTx2Path;
    wxCheckBox* lnaByp;

    wxChoice* cmbRx1Path2;
    wxChoice* cmbTx1Path2;
    wxChoice* cmbTRx1Path2;
    wxChoice* cmbTRx2Path2;
    wxChoice* cmbRx2Path2;
    wxChoice* cmbTx2Path2;
    wxCheckBox* lnaByp2;
    wxCheckBox* tx11En2;
    wxCheckBox* tx12En2;
    wxCheckBox* tx2En2;
    lms_device_t *lmsControl;
    DECLARE_EVENT_TABLE()
};

