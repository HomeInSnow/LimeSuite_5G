#pragma once

#include "lime/LimeSuite.h"
#include <map>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
class wxStaticText;
class wxFlexGridSizer;
class wxCheckBox;

class pnl5GRadio : public wxPanel
{
public:
    pnl5GRadio(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, int style = 0, wxString name = wxEmptyString);
    void Initialize(lms_device_t *pControl);
    virtual ~pnl5GRadio();
    virtual void UpdatePanel();

    void OnReadAll(wxCommandEvent &event);
    void OnWriteAll(wxCommandEvent &event);

protected:
    void OnInputChange(wxCommandEvent &event);
    void OnDacChange(wxCommandEvent &event);

    // LMS1 Ch0
    wxChoice* cmbLms1Rx1Path;
    wxChoice* cmbLms1Tx1Path;
    wxCheckBox* Lms1tx1En;
    wxSpinCtrl* spinDac1;

    // LMS1 Ch1
    wxChoice* cmbLms1Rx2Path;
    wxChoice* cmbLms1Tx2Path;
    wxCheckBox* Lms1tx2En;
    wxSpinCtrl* spinDac2;

    // LMS2 Ch0
    wxCheckBox* Lms2tx1En;
    wxCheckBox* Lms2Lna1En;
    wxChoice* cmbLms2Trx1TPath;
    wxChoice* cmbLms2Trx1Path;
    wxChoice* cmbLms2Rx1CPath;
    wxChoice* cmbLms2Rx1InPath;

    // LMS2 Ch1
    wxCheckBox* Lms2tx2En;
    wxCheckBox* Lms2Lna2En;
    wxChoice* cmbLms2Trx2TPath;
    wxChoice* cmbLms2Trx2Path;
    wxChoice* cmbLms2Rx2CPath;
    wxChoice* cmbLms2Rx2InPath;

    //LMS3
    wxChoice* cmbLms3Rx1Path;
    wxChoice* cmbLms3Rx2Path;

    lms_device_t *lmsControl;
    DECLARE_EVENT_TABLE()
};

