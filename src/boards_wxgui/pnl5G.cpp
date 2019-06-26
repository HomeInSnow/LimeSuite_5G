#include "pnl5G.h"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/msgdlg.h>
#include "lms7suiteEvents.h"

#include <ciso646>


using namespace std;

BEGIN_EVENT_TABLE(pnlCoreSDR5G, wxPanel)
END_EVENT_TABLE()

pnlCoreSDR5G::pnlCoreSDR5G(wxWindow* parent,wxWindowID id, const wxPoint& pos,const wxSize& size, int style, wxString name)
{
    lmsControl = nullptr;

    Create(parent, id, pos, size, style, name);
#ifdef WIN32
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
#endif
    wxFlexGridSizer* mainSizer = new wxFlexGridSizer(0, 2, 5, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW1_TX1 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbTx1Path = new wxChoice(this, wxNewId());
    cmbTx1Path->Append("TX1_1 -> TX_OUT");
    cmbTx1Path->Append("TX1_2 -> TX_OUT");
    cmbTx1Path->SetSelection(0);
    Connect(cmbTx1Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(cmbTx1Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("TX1_1_EN  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    tx11En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(tx11En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(tx11En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("TX1_2_EN  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    tx12En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(tx12En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(tx12En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW1_TRX1T ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbTRx1Path = new wxChoice(this, wxNewId());
    cmbTRx1Path->Append("TX_OUT -> J1 (TRX)");
    cmbTRx1Path->Append("TRX1_IN <- J1 (TRX)");
    cmbTRx1Path->SetSelection(0);
    Connect(cmbTRx1Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(cmbTRx1Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("LNA1_EN_M ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbRx1Path = new wxChoice(this, wxNewId());
    cmbRx1Path->Append("RX1_H <- RX_IN");
    cmbRx1Path->Append("RX1_W <- RX_IN");
    cmbRx1Path->SetSelection(0);
    Connect(cmbRx1Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(cmbRx1Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW1_TRX1R ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbTRx2Path = new wxChoice(this, wxNewId());
    cmbTRx2Path->Append("RX_IN <- TRX1_IN");
    cmbTRx2Path->Append("RX_IN <- J2 (RX)");
    cmbTRx2Path->SetSelection(0);
    Connect(cmbTRx2Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(cmbTRx2Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("LNA1_BP_M ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    lnaByp = new wxCheckBox(this, wxNewId(), _("Bypass"));
    Connect(lnaByp->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(lnaByp, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW1_TRX2T  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbTx2Path = new wxChoice(this, wxNewId());
    cmbTx2Path->Append("TX2_1 -> J3 (TRX)");
    cmbTx2Path->Append("TRX2_IN <- J3 (TRX)");
    cmbTx2Path->SetSelection(0);
    Connect(cmbTx2Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(cmbTx2Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("TX2_1_EN   ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    tx2En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(tx2En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(tx2En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW1_TRX2R ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbRx2Path = new wxChoice(this, wxNewId());
    cmbRx2Path->Append("RX2_H <- TRX2_IN");
    cmbRx2Path->Append("RX2_H <- RX2_IN");
    cmbRx2Path->SetSelection(0);
    Connect(cmbRx2Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(cmbRx2Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW1_RX2 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbRxPathAB = new wxChoice(this, wxNewId());
    cmbRxPathAB->Append("RX2_IN <- J4 (RX2A)");
    cmbRxPathAB->Append("RX2_IN <- J5 (RX2B)");
    cmbRxPathAB->SetSelection(0);
    Connect(cmbRxPathAB->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), NULL, this);
    mainSizer->Add(cmbRxPathAB, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    auto lms1Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("LMS1") ), wxVERTICAL );
    lms1Sizer->Add(mainSizer, 0, 0, 5);

    wxFlexGridSizer* mainSizer2 = new wxFlexGridSizer(0, 2, 5, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW2_TX1 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbTx1Path2 = new wxChoice(this, wxNewId());
    cmbTx1Path2->Append("TX1_2 -> TX_OUT");
    cmbTx1Path2->Append("TX1_1 -> TX_OUT");
    cmbTx1Path2->SetSelection(0);
    Connect(cmbTx1Path2->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(cmbTx1Path2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("TX1_1_EN  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    tx11En2 = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(tx11En2->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(tx11En2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("TX1_2_EN  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    tx12En2 = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(tx12En2->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(tx12En2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW2_TRX1T ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbTRx1Path2 = new wxChoice(this, wxNewId());
    cmbTRx1Path2->Append("TX_OUT -> J6 (TRX)");
    cmbTRx1Path2->Append("TRX1_IN <- J6 (TRX)");
    cmbTRx1Path2->SetSelection(0);
    Connect(cmbTRx1Path2->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(cmbTRx1Path2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("LNA2_EN_M ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbRx1Path2 = new wxChoice(this, wxNewId());
    cmbRx1Path2->Append("RX1_H <- RX_IN");
    cmbRx1Path2->Append("RX1_W <- RX_IN");
    cmbRx1Path2->SetSelection(0);
    Connect(cmbRx1Path2->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(cmbRx1Path2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW2_TRX1R ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbTRx2Path2 = new wxChoice(this, wxNewId());
    cmbTRx2Path2->Append("RX_IN <- TRX1_IN");
    cmbTRx2Path2->Append("RX_IN <- J7 (RX)");
    cmbTRx2Path2->SetSelection(0);
    Connect(cmbTRx2Path2->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(cmbTRx2Path2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("LNA2_BP_M ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    lnaByp2 = new wxCheckBox(this, wxNewId(), _("Bypass"));
    Connect(lnaByp2->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(lnaByp2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW2_TRX2T ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbTx2Path2 = new wxChoice(this, wxNewId());
    cmbTx2Path2->Append("TX2_1 -> J8 (TRX)");
    cmbTx2Path2->Append("TRX2_IN <- J8 (TRX)");
    cmbTx2Path2->SetSelection(0);
    Connect(cmbTx2Path2->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(cmbTx2Path2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("TX2_1_EN   ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    tx2En2 = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(tx2En2->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(tx2En2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW2_TRX2R ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbRx2Path2 = new wxChoice(this, wxNewId());
    cmbRx2Path2->Append("RX2_H <- TRX2_IN");
    cmbRx2Path2->Append("RX2_H <- RX (J9)");
    cmbRx2Path2->SetSelection(0);
    Connect(cmbRx2Path2->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), NULL, this);
    mainSizer2->Add(cmbRx2Path2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);
    auto lms2Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("LMS2") ), wxVERTICAL );
    lms2Sizer->Add(mainSizer2, 0, 0, 5);

    wxStaticBoxSizer* mainBoxSizer;
    mainBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("RF switch controls") ), wxHORIZONTAL );
    mainBoxSizer->Add(lms1Sizer, 0, 0, 5);
    mainBoxSizer->Add(lms2Sizer, 0, 0, 5);
    mainBoxSizer->Fit(this);
    mainBoxSizer->SetSizeHints(this);
    SetSizer(mainBoxSizer);
    Layout();

    Bind(READ_ALL_VALUES, &pnlCoreSDR5G::OnReadAll, this, this->GetId());
    Bind(WRITE_ALL_VALUES, &pnlCoreSDR5G::OnWriteAll, this, this->GetId());
}

void pnlCoreSDR5G::Initialize(lms_device_t* pControl)
{
    lmsControl = pControl;
}

pnlCoreSDR5G::~pnlCoreSDR5G()
{
    cmbRx1Path->Disconnect(wxEVT_CHOICE, cmbRx1Path->GetId(), wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), 0, this);
    cmbTx1Path->Disconnect(wxEVT_CHOICE, cmbTx1Path->GetId(), wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), 0, this);
    cmbRx2Path->Disconnect(wxEVT_CHOICE, cmbRx2Path->GetId(), wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), 0, this);
    cmbTx2Path->Disconnect(wxEVT_CHOICE, cmbTx2Path->GetId(), wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange), 0, this);
    cmbRx1Path2->Disconnect(wxEVT_CHOICE, cmbRx1Path2->GetId(), wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), 0, this);
    cmbTx1Path2->Disconnect(wxEVT_CHOICE, cmbTx1Path2->GetId(), wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), 0, this);
    cmbRx2Path2->Disconnect(wxEVT_CHOICE, cmbRx2Path2->GetId(), wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), 0, this);
    cmbTx2Path2->Disconnect(wxEVT_CHOICE, cmbTx2Path2->GetId(), wxCommandEventHandler(pnlCoreSDR5G::OnLoopbackChange2), 0, this);
}

void pnlCoreSDR5G::OnLoopbackChange(wxCommandEvent& event)
{
    LMS_WriteFPGAReg(lmsControl,0xFFFF, 1);
    uint16_t addr = 0x0017;
    uint16_t value = 0;

    value |= tx11En->GetValue() << 0;
    value |= tx12En->GetValue() << 1;
    value |= tx2En->GetValue() << 2;
    value |= cmbTx1Path->GetSelection() << 4;
    value |= cmbTRx1Path->GetSelection() << 5;
    value |= cmbTRx2Path->GetSelection() << 6;
    value |= cmbRx1Path->GetSelection() << 8;
    value |= lnaByp->GetValue() << 9;
    value |= cmbTx2Path->GetSelection() << 12;
    value |= cmbRx2Path->GetSelection() << 13;
    value |= cmbRxPathAB->GetSelection() << 14;


    if(LMS_WriteFPGAReg(lmsControl,addr, value))
        wxMessageBox(_("Failed to write FPGA registers"), _("Error"), wxICON_ERROR | wxOK);
}

void pnlCoreSDR5G::OnLoopbackChange2(wxCommandEvent& event)
{
    LMS_WriteFPGAReg(lmsControl,0xFFFF, 2);
    uint16_t addr = 0x0017;
    uint16_t value = 0;

    value |= tx11En2->GetValue() << 0;
    value |= tx12En2->GetValue() << 1;
    value |= tx2En2->GetValue() << 2;
    value |= cmbTx1Path2->GetSelection() << 4;
    value |= cmbTRx1Path2->GetSelection() << 5;
    value |= cmbTRx2Path2->GetSelection() << 6;
    value |= cmbRx1Path2->GetSelection() << 8;
    value |= lnaByp2->GetValue() << 9;
    value |= cmbTx2Path2->GetSelection() << 12;
    value |= cmbRx2Path2->GetSelection() << 13;

    if(LMS_WriteFPGAReg(lmsControl,addr, value))
        wxMessageBox(_("Failed to write FPGA registers"), _("Error"), wxICON_ERROR | wxOK);
}

void pnlCoreSDR5G::UpdatePanel()
{
    uint16_t addr = 0x0017;
    uint16_t value = 0;
    LMS_WriteFPGAReg(lmsControl,0xFFFF, 1);
    if(LMS_ReadFPGAReg(lmsControl,addr, &value))
    {
        wxMessageBox(_("Failed to read FPGA registers"), _("Error"), wxICON_ERROR | wxOK);
        return;
    }

    tx11En->SetValue((value)&1);
    tx12En->SetValue((value>>1)&1);
    tx2En->SetValue((value>>2)&1);
    cmbTx1Path->SetSelection((value>>4)&1);
    cmbTRx1Path->SetSelection((value>>5)&1);
    cmbTRx2Path->SetSelection((value>>6)&1);
    cmbRx1Path->SetSelection((value>>8)&1);
    lnaByp->SetValue((value>>9)&1);
    cmbTx2Path->SetSelection((value>>12)&1);
    cmbRx2Path->SetSelection((value>>13)&1);
    cmbRxPathAB->SetSelection((value>>14)&1);

    LMS_WriteFPGAReg(lmsControl,0xFFFF, 2);
    if(LMS_ReadFPGAReg(lmsControl,addr, &value))
    {
        wxMessageBox(_("Failed to read FPGA registers"), _("Error"), wxICON_ERROR | wxOK);
        return;
    }

    tx11En2->SetValue((value)&1);
    tx12En2->SetValue((value>>1)&1);
    tx2En2->SetValue((value>>2)&1);
    cmbTx1Path2->SetSelection((value>>4)&1);
    cmbTRx1Path2->SetSelection((value>>5)&1);
    cmbTRx2Path2->SetSelection((value>>6)&1);
    cmbRx1Path2->SetSelection((value>>8)&1);
    lnaByp2->SetValue((value>>9)&1);
    cmbTx2Path2->SetSelection((value>>12)&1);
    cmbRx2Path2->SetSelection((value>>13)&1);
}

void pnlCoreSDR5G::OnReadAll(wxCommandEvent &event)
{
    UpdatePanel();
}

void pnlCoreSDR5G::OnWriteAll(wxCommandEvent &event)
{
    OnLoopbackChange2(event);
    OnLoopbackChange(event);
}
