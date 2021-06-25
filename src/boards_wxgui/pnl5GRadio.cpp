#include "pnl5GRadio.h"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/msgdlg.h>
#include "lms7suiteEvents.h"

#include <ciso646>


using namespace std;

BEGIN_EVENT_TABLE(pnl5GRadio, wxPanel)
END_EVENT_TABLE()

pnl5GRadio::pnl5GRadio(wxWindow* parent,wxWindowID id, const wxPoint& pos,const wxSize& size, int style, wxString name)
{
    lmsControl = nullptr;

    Create(parent, id, pos, size, style, name);
#ifdef WIN32
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
#endif
    // LMS1
    wxFlexGridSizer* mainSizer = new wxFlexGridSizer(0, 2, 5, 5);

    // LMS 1 Ch0
    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("TX1_EN  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    Lms1tx1En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(Lms1tx1En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer->Add(Lms1tx1En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW_TX1 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms1Tx1Path = new wxChoice(this, wxNewId());
    cmbLms1Tx1Path->Append("TX1_2 -> TX1 (J6)");
    cmbLms1Tx1Path->Append("TX1_1 -> TX1 (J6)");
    cmbLms1Tx1Path->SetSelection(0);
    Connect(cmbLms1Tx1Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer->Add(cmbLms1Tx1Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("TX1DAC ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    wxSize spinBoxSize(64, -1);
    long spinBoxStyle = wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER;
    spinDac1 = new wxSpinCtrl(this, wxNewId(), wxEmptyString, wxDefaultPosition, spinBoxSize, spinBoxStyle, 0, 65535, 0);
    Connect(spinDac1->GetId(), wxEVT_TEXT_ENTER, wxCommandEventHandler(pnl5GRadio::OnDacChange), NULL, this);
    Connect(spinDac1->GetId(), wxEVT_SPINCTRL, wxCommandEventHandler(pnl5GRadio::OnDacChange), NULL, this);
    Connect(spinDac1->GetId(), wxEVT_SPIN, wxCommandEventHandler(pnl5GRadio::OnDacChange), NULL, this);
    mainSizer->Add(spinDac1, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW_RX1 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms1Rx1Path = new wxChoice(this, wxNewId());
    cmbLms1Rx1Path->Append("RX1_L <- RX1 (J1)");
    cmbLms1Rx1Path->Append("RX1_H <- RX1 (J1)");
    cmbLms1Rx1Path->SetSelection(0);
    Connect(cmbLms1Rx1Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer->Add(cmbLms1Rx1Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    // LMS 1 Ch1
    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("TX2_EN  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    Lms1tx2En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(Lms1tx2En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer->Add(Lms1tx2En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW_TX2 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms1Tx2Path = new wxChoice(this, wxNewId());
    cmbLms1Tx2Path->Append("TX2_2 -> TX2 (J7)");
    cmbLms1Tx2Path->Append("TX2_1 -> TX2 (J7)");
    cmbLms1Tx2Path->SetSelection(0);
    Connect(cmbLms1Tx2Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer->Add(cmbLms1Tx2Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("TX2DAC ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    spinDac2 = new wxSpinCtrl(this, wxNewId(), wxEmptyString, wxDefaultPosition, spinBoxSize, spinBoxStyle, 0, 65535, 0);
    Connect(spinDac2->GetId(), wxEVT_TEXT_ENTER, wxCommandEventHandler(pnl5GRadio::OnDacChange), NULL, this);
    Connect(spinDac2->GetId(), wxEVT_SPINCTRL, wxCommandEventHandler(pnl5GRadio::OnDacChange), NULL, this);
    Connect(spinDac2->GetId(), wxEVT_SPIN, wxCommandEventHandler(pnl5GRadio::OnDacChange), NULL, this);
    mainSizer->Add(spinDac2, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, _("RFSW_RX2 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms1Rx2Path = new wxChoice(this, wxNewId());
    cmbLms1Rx2Path->Append("RX2_L <- RX2 (J2)");
    cmbLms1Rx2Path->Append("RX2_H <- RX2 (J2)");
    cmbLms1Rx2Path->SetSelection(0);
    Connect(cmbLms1Rx2Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer->Add(cmbLms1Rx2Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    auto lms1Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("LMS1") ), wxVERTICAL );
    lms1Sizer->Add(mainSizer, 0, 0, 5);


    // LMS2
    wxFlexGridSizer* mainSizer2 = new wxFlexGridSizer(0, 2, 5, 5);

    // LMS2 Ch0
    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("TX1_EN  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    Lms2tx1En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(Lms2tx1En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(Lms2tx1En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW_TRX1T ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms2Trx1TPath = new wxChoice(this, wxNewId());
    cmbLms2Trx1TPath->Append("TX1_1 -> RFSW_TRX1");
    cmbLms2Trx1TPath->Append("TX1_1 -> Ground");
    cmbLms2Trx1TPath->SetSelection(0);
    Connect(cmbLms2Trx1TPath->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(cmbLms2Trx1TPath, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW_TRX1 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms2Trx1Path = new wxChoice(this, wxNewId());
    cmbLms2Trx1Path->Append("RFSW_RX1IN <- TRX1 (J8)");
    cmbLms2Trx1Path->Append("RFSW_TRX1T -> TRX1 (J8)");
    cmbLms2Trx1Path->SetSelection(0);
    Connect(cmbLms2Trx1Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(cmbLms2Trx1Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RX1_LNA  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    Lms2Lna1En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(Lms2Lna1En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(Lms2Lna1En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW_RX1C ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms2Rx1CPath = new wxChoice(this, wxNewId());
    cmbLms2Rx1CPath->Append("RX1_H <- RFSW_RX1IN (LNA)");
    cmbLms2Rx1CPath->Append("RX1_H <- LMS3 (TX1_1)");
    cmbLms2Rx1CPath->SetSelection(0);
    Connect(cmbLms2Rx1CPath->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(cmbLms2Rx1CPath, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW_RX1IN ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms2Rx1InPath = new wxChoice(this, wxNewId());
    cmbLms2Rx1InPath->Append("RFSW_RX1C <- RX1 (J9)");
    cmbLms2Rx1InPath->Append("RFSW_RX1C <- RSFW_TRX1");
    cmbLms2Rx1InPath->SetSelection(0);
    Connect(cmbLms2Rx1InPath->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(cmbLms2Rx1InPath, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    // LMS2 Ch1
    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("TX2_EN  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    Lms2tx2En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(Lms2tx2En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(Lms2tx2En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW_TRX2T ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms2Trx2TPath = new wxChoice(this, wxNewId());
    cmbLms2Trx2TPath->Append("TX2_1 -> Ground");
    cmbLms2Trx2TPath->Append("TX2_1 -> RFSW_TRX2");
    cmbLms2Trx2TPath->SetSelection(0);
    Connect(cmbLms2Trx2TPath->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(cmbLms2Trx2TPath, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW_TRX2 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms2Trx2Path = new wxChoice(this, wxNewId());
    cmbLms2Trx2Path->Append("RFSW_RX2IN <- TRX2 (J10)");
    cmbLms2Trx2Path->Append("RFSW_TRX2T -> TRX2 (J10)");
    cmbLms2Trx2Path->SetSelection(0);
    Connect(cmbLms2Trx2Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(cmbLms2Trx2Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RX2_LNA  ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    Lms2Lna2En = new wxCheckBox(this, wxNewId(), _("Enable"));
    Connect(Lms2Lna2En->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(Lms2Lna2En, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW_RX2C ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms2Rx2CPath = new wxChoice(this, wxNewId());
    cmbLms2Rx2CPath->Append("RX2_H <- RFSW_RX2IN (LNA)");
    cmbLms2Rx2CPath->Append("RX2_H <- LMS3 (TX2_1)");
    cmbLms2Rx2CPath->SetSelection(0);
    Connect(cmbLms2Rx2CPath->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(cmbLms2Rx2CPath, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer2->Add(new wxStaticText(this, wxID_ANY, _("RFSW_RX2IN ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms2Rx2InPath = new wxChoice(this, wxNewId());
    cmbLms2Rx2InPath->Append("RFSW_RX2C <- RX2 (J11)");
    cmbLms2Rx2InPath->Append("RFSW_RX2C <- RSFW_TRX2");
    cmbLms2Rx2InPath->SetSelection(0);
    Connect(cmbLms2Rx2InPath->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer2->Add(cmbLms2Rx2InPath, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);


    auto lms2Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("LMS2") ), wxVERTICAL );
    lms2Sizer->Add(mainSizer2, 0, 0, 5);

    // LMS3
    wxFlexGridSizer* mainSizer3 = new wxFlexGridSizer(0, 2, 5, 5);
    mainSizer3->Add(new wxStaticText(this, wxID_ANY, _("RFSW1_RX1 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms3Rx1Path = new wxChoice(this, wxNewId());
    cmbLms3Rx1Path->Append("RX1_H <- RX_IN (J4)");
    cmbLms3Rx1Path->Append("RX1_H <- LMS2 (TX1_1)");
    cmbLms3Rx1Path->SetSelection(0);
    Connect(cmbLms3Rx1Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer3->Add(cmbLms3Rx1Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    mainSizer3->Add(new wxStaticText(this, wxID_ANY, _("RFSW1_RX2 ")), 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
    cmbLms3Rx2Path = new wxChoice(this, wxNewId());
    cmbLms3Rx2Path->Append("RX2_H <- RX_IN (J5)");
    cmbLms3Rx2Path->Append("RX1_H <- LMS2 (TX2_1)");
    cmbLms3Rx2Path->SetSelection(0);
    Connect(cmbLms3Rx2Path->GetId(), wxEVT_CHOICE, wxCommandEventHandler(pnl5GRadio::OnInputChange), NULL, this);
    mainSizer3->Add(cmbLms3Rx2Path, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_TOP, 5);

    auto lms3Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("LMS3") ), wxVERTICAL );
    lms3Sizer->Add(mainSizer3, 0, 0, 5);


    // Main RF sizer
    wxStaticBoxSizer* mainBoxSizer;
    mainBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("RF controls") ), wxHORIZONTAL );
    mainBoxSizer->Add(lms1Sizer, 0, 0, 5);
    mainBoxSizer->Add(lms2Sizer, 0, 0, 5);
    mainBoxSizer->Add(lms3Sizer, 0, 0, 5);
    mainBoxSizer->Fit(this);
    mainBoxSizer->SetSizeHints(this);
    SetSizer(mainBoxSizer);
    Layout();

    Bind(READ_ALL_VALUES, &pnl5GRadio::OnReadAll, this, this->GetId());
    Bind(WRITE_ALL_VALUES, &pnl5GRadio::OnWriteAll, this, this->GetId());
}

void pnl5GRadio::Initialize(lms_device_t* pControl)
{
    lmsControl = pControl;
}

pnl5GRadio::~pnl5GRadio()
{
    // LMS1
    cmbLms1Rx1Path->Disconnect(wxEVT_CHOICE, cmbLms1Rx1Path->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms1Tx1Path->Disconnect(wxEVT_CHOICE, cmbLms1Tx1Path->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms1Rx2Path->Disconnect(wxEVT_CHOICE, cmbLms1Rx2Path->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms1Tx2Path->Disconnect(wxEVT_CHOICE, cmbLms1Tx2Path->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);

    spinDac1->Disconnect(wxEVT_TEXT_ENTER, spinDac1->GetId(), wxCommandEventHandler(pnl5GRadio::OnDacChange), 0, this);
    spinDac1->Disconnect(wxEVT_SPINCTRL, spinDac1->GetId(), wxCommandEventHandler(pnl5GRadio::OnDacChange), 0, this);
    spinDac1->Disconnect(wxEVT_SPIN, spinDac1->GetId(), wxCommandEventHandler(pnl5GRadio::OnDacChange), 0, this);

    spinDac2->Disconnect(wxEVT_TEXT_ENTER, spinDac2->GetId(), wxCommandEventHandler(pnl5GRadio::OnDacChange), 0, this);
    spinDac2->Disconnect(wxEVT_SPINCTRL, spinDac2->GetId(), wxCommandEventHandler(pnl5GRadio::OnDacChange), 0, this);
    spinDac2->Disconnect(wxEVT_SPIN, spinDac2->GetId(), wxCommandEventHandler(pnl5GRadio::OnDacChange), 0, this);

    // LMS2
    cmbLms2Trx1TPath->Disconnect(wxEVT_CHOICE, cmbLms2Trx1TPath->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms2Trx1Path->Disconnect(wxEVT_CHOICE, cmbLms2Trx1Path->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms2Rx1CPath->Disconnect(wxEVT_CHOICE, cmbLms2Rx1CPath->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms2Rx1InPath->Disconnect(wxEVT_CHOICE, cmbLms2Rx1InPath->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);

    cmbLms2Trx2TPath->Disconnect(wxEVT_CHOICE, cmbLms2Trx2TPath->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms2Trx2Path->Disconnect(wxEVT_CHOICE, cmbLms2Trx2Path->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms2Rx2CPath->Disconnect(wxEVT_CHOICE, cmbLms2Rx2CPath->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms2Rx2InPath->Disconnect(wxEVT_CHOICE, cmbLms2Rx2InPath->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);

    // LMS3
    cmbLms3Rx1Path->Disconnect(wxEVT_CHOICE, cmbLms3Rx1Path->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
    cmbLms3Rx1Path->Disconnect(wxEVT_CHOICE, cmbLms3Rx2Path->GetId(), wxCommandEventHandler(pnl5GRadio::OnInputChange), 0, this);
}

void pnl5GRadio::OnInputChange(wxCommandEvent& event)
{
    uint16_t addr = 0x00D1;
    uint16_t value = 0;

    value |= cmbLms3Rx1Path->GetSelection() << 0;
    value |= cmbLms3Rx2Path->GetSelection() << 1;
    value |= cmbLms2Rx1CPath->GetSelection() << 2;
    value |= cmbLms2Rx1InPath->GetSelection() << 3;
    value |= cmbLms2Rx2CPath->GetSelection() << 4;
    value |= cmbLms2Rx2InPath->GetSelection() << 5;
    value |= cmbLms2Trx1Path->GetSelection() << 6;
    value |= cmbLms2Trx1TPath->GetSelection() << 7;
    value |= cmbLms2Trx2Path->GetSelection() << 8;
    value |= cmbLms2Trx2TPath->GetSelection() << 9;
    value |= cmbLms1Rx2Path->GetSelection() << 10;
    value |= cmbLms1Rx1Path->GetSelection() << 11;
    value |= cmbLms1Tx2Path->GetSelection() << 12;
    value |= cmbLms1Tx1Path->GetSelection() << 13;

    if(LMS_WriteFPGAReg(lmsControl,addr, value))
        wxMessageBox(_("Failed to write FPGA registers"), _("Error"), wxICON_ERROR | wxOK);

    addr = 0x00D2;
    value = 0;
    
    value |= (!Lms2Lna2En->GetValue()) << 0;
    value |= (!Lms2Lna1En->GetValue()) << 1;
    value |= Lms2tx2En->GetValue() << 2;
    value |= Lms2tx1En->GetValue() << 3;
    value |= Lms1tx2En->GetValue() << 4;
    value |= Lms1tx1En->GetValue() << 5;

    if(LMS_WriteFPGAReg(lmsControl,addr, value))
        wxMessageBox(_("Failed to write FPGA registers"), _("Error"), wxICON_ERROR | wxOK);

}

void pnl5GRadio::OnDacChange(wxCommandEvent& event)
{
    LMS_WriteCustomBoardParam(lmsControl, 2, spinDac1->GetValue(), "");
    LMS_WriteCustomBoardParam(lmsControl, 3, spinDac2->GetValue(), "");

    return;
}


void pnl5GRadio::UpdatePanel()
{
    uint16_t addr = 0x00D1;
    uint16_t value = 0;
    if(LMS_ReadFPGAReg(lmsControl,addr, &value))
    {
        wxMessageBox(_("Failed to read FPGA registers"), _("Error"), wxICON_ERROR | wxOK);
        return;
    }

    cmbLms3Rx1Path->SetSelection((value>>0)&1);
    cmbLms3Rx2Path->SetSelection((value>>1)&1);
    cmbLms2Rx1CPath->SetSelection((value>>2)&1);
    cmbLms2Rx1InPath->SetSelection((value>>3)&1);
    cmbLms2Rx2CPath->SetSelection((value>>4)&1);
    cmbLms2Rx2InPath->SetSelection((value>>5)&1);
    cmbLms2Trx1Path->SetSelection((value>>6)&1);
    cmbLms2Trx1TPath->SetSelection((value>>7)&1);
    cmbLms2Trx2Path->SetSelection((value>>8)&1);
    cmbLms2Trx2TPath->SetSelection((value>>9)&1);
    cmbLms1Rx2Path->SetSelection((value>>10)&1);
    cmbLms1Rx1Path->SetSelection((value>>11)&1);
    cmbLms1Tx2Path->SetSelection((value>>12)&1);
    cmbLms1Tx1Path->SetSelection((value>>13)&1);

    addr = 0x00D2;
    value = 0;
    if(LMS_ReadFPGAReg(lmsControl,addr, &value))
    {
        wxMessageBox(_("Failed to read FPGA registers"), _("Error"), wxICON_ERROR | wxOK);
        return;
    }

    Lms2Lna2En->SetValue(!((value>>0)&1));
    Lms2Lna1En->SetValue(!((value>>1)&1));
    Lms2tx2En->SetValue((value>>2)&1);
    Lms2tx1En->SetValue((value>>3)&1);
    Lms1tx2En->SetValue((value>>4)&1);
    Lms1tx1En->SetValue((value>>5)&1);

    double dacVal = 0;
    LMS_ReadCustomBoardParam(lmsControl, 2, &dacVal, nullptr);
    spinDac1->SetValue((int)dacVal);
    LMS_ReadCustomBoardParam(lmsControl, 3, &dacVal, nullptr);
    spinDac2->SetValue((int)dacVal);
}

void pnl5GRadio::OnReadAll(wxCommandEvent &event)
{
    UpdatePanel();
}

void pnl5GRadio::OnWriteAll(wxCommandEvent &event)
{
    OnInputChange(event);
    OnDacChange(event);
}
