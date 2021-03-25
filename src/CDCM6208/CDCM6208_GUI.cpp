///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Mar 22 2021)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CDCM6208_panelgui.h"

#include "CDCM6208_GUI.h"

///////////////////////////////////////////////////////////////////////////

CDCM6208_CONTROL::CDCM6208_CONTROL( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer26;
	fgSizer26 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer26->SetFlexibleDirection( wxBOTH );
	fgSizer26->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	fgSizer26->SetMinSize( wxSize( 300,300 ) );
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_CDCMtabs = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_CDCMtabs->SetMinSize( wxSize( 1250,-1 ) );

	m_CDCM1 = new CDCM6208_panelgui( m_CDCMtabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_CDCMtabs->AddPage( m_CDCM1, wxT("CDCM1"), false );
	m_CDCM2 = new CDCM6208_panelgui( m_CDCMtabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_CDCMtabs->AddPage( m_CDCM2, wxT("CDCM2"), false );

	fgSizer2->Add( m_CDCMtabs, 1, wxEXPAND | wxALL, 5 );


	fgSizer26->Add( fgSizer2, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer26 );
	this->Layout();
	fgSizer26->Fit( this );

	this->Centre( wxBOTH );
}

CDCM6208_CONTROL::~CDCM6208_CONTROL()
{
}

CDCM_controlpanel::CDCM_controlpanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* Master;
	Master = new wxFlexGridSizer( 0, 2, 0, 0 );
	Master->SetFlexibleDirection( wxBOTH );
	Master->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* Left_Column;
	Left_Column = new wxFlexGridSizer( 0, 1, 0, 0 );
	Left_Column->SetFlexibleDirection( wxBOTH );
	Left_Column->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* VCO_Settings;
	VCO_Settings = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("VCO Settings") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 1, 7, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_staticText1 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("Primary Input Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer3->Add( m_staticText1, 0, wxALL, 5 );

	m_PrimaryF = new wxSpinCtrlDouble( VCO_Settings->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_PrimaryF->SetDigits( 0 );
	bSizer3->Add( m_PrimaryF, 0, wxALL, 5 );

	m_staticText2 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("Secondary Input Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALL, 5 );

	m_SecondaryF = new wxSpinCtrlDouble( VCO_Settings->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_SecondaryF->SetDigits( 0 );
	bSizer3->Add( m_SecondaryF, 0, wxALL, 5 );


	fgSizer6->Add( bSizer3, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_staticText3 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("R divider"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer4->Add( m_staticText3, 0, wxALL, 5 );

	m_RDivider = new wxSpinCtrl( VCO_Settings->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer4->Add( m_RDivider, 0, wxALL, 5 );


	fgSizer6->Add( bSizer4, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_staticText4 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("Input MUX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer5->Add( m_staticText4, 0, wxALL, 5 );

	m_PrimarySel = new wxRadioButton( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("Pri"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_PrimarySel, 0, wxALL, 5 );

	m_SecondarySel = new wxRadioButton( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("Sec"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_SecondarySel, 0, wxALL, 5 );


	fgSizer6->Add( bSizer5, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_staticText5 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("M divider"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer6->Add( m_staticText5, 0, wxALL, 5 );

	m_MDivider = new wxSpinCtrlDouble( VCO_Settings->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_MDivider->SetDigits( 0 );
	bSizer6->Add( m_MDivider, 0, wxALL, 5 );

	m_staticText6 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("N multiplier"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer6->Add( m_staticText6, 0, wxALL, 5 );

	m_NMultiplier = new wxSpinCtrlDouble( VCO_Settings->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_NMultiplier->SetDigits( 0 );
	bSizer6->Add( m_NMultiplier, 0, wxALL, 5 );


	fgSizer6->Add( bSizer6, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_staticText7 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("VCO Freq"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer7->Add( m_staticText7, 0, wxALL, 5 );

	m_VCOFREQ = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_VCOFREQ->Wrap( -1 );
	bSizer7->Add( m_VCOFREQ, 0, wxALL, 5 );


	fgSizer6->Add( bSizer7, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	m_staticText9 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("Prescaler B"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer8->Add( m_staticText9, 0, wxALL, 5 );

	wxString m_PSBChoices[] = { wxT("4"), wxT("5"), wxT("6") };
	int m_PSBNChoices = sizeof( m_PSBChoices ) / sizeof( wxString );
	m_PSB = new wxChoice( VCO_Settings->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PSBNChoices, m_PSBChoices, 0 );
	m_PSB->SetSelection( 0 );
	bSizer8->Add( m_PSB, 0, wxALL, 5 );

	m_staticText10 = new wxStaticText( VCO_Settings->GetStaticBox(), wxID_ANY, wxT("Prescaler A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer8->Add( m_staticText10, 0, wxALL, 5 );

	wxString m_PSAChoices[] = { wxT("4"), wxT("5"), wxT("6"), wxEmptyString };
	int m_PSANChoices = sizeof( m_PSAChoices ) / sizeof( wxString );
	m_PSA = new wxChoice( VCO_Settings->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PSANChoices, m_PSAChoices, 0 );
	m_PSA->SetSelection( 0 );
	bSizer8->Add( m_PSA, 0, wxALL, 5 );


	fgSizer6->Add( bSizer8, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );


	VCO_Settings->Add( fgSizer6, 1, wxEXPAND, 5 );


	Left_Column->Add( VCO_Settings, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* Misc;
	Misc = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Misc") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );

	m_staticText50 = new wxStaticText( Misc->GetStaticBox(), wxID_ANY, wxT("CDCM version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	bSizer71->Add( m_staticText50, 0, wxALL, 5 );

	wxString m_CDCM_VERChoices[] = { wxT("V1"), wxT("V2") };
	int m_CDCM_VERNChoices = sizeof( m_CDCM_VERChoices ) / sizeof( wxString );
	m_CDCM_VER = new wxChoice( Misc->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_CDCM_VERNChoices, m_CDCM_VERChoices, 0 );
	m_CDCM_VER->SetSelection( 1 );
	bSizer71->Add( m_CDCM_VER, 0, wxALL, 5 );


	fgSizer12->Add( bSizer71, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );

	m_staticText51 = new wxStaticText( Misc->GetStaticBox(), wxID_ANY, wxT("VCO Frequency range"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	bSizer81->Add( m_staticText51, 0, wxALL, 5 );

	m_VCORANGE = new wxStaticText( Misc->GetStaticBox(), wxID_ANY, wxT("???"), wxDefaultPosition, wxDefaultSize, 0 );
	m_VCORANGE->Wrap( -1 );
	bSizer81->Add( m_VCORANGE, 0, wxALL, 5 );


	fgSizer12->Add( bSizer81, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_staticText26 = new wxStaticText( Misc->GetStaticBox(), wxID_ANY, wxT("CDCM config base address"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	bSizer9->Add( m_staticText26, 0, wxALL, 5 );

	m_spinCtrl4 = new wxSpinCtrl( Misc->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer9->Add( m_spinCtrl4, 0, wxALL, 5 );


	fgSizer12->Add( bSizer9, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_staticText27 = new wxStaticText( Misc->GetStaticBox(), wxID_ANY, wxT("Lock Status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	bSizer12->Add( m_staticText27, 0, wxALL, 5 );

	m_LockStatus = new wxStaticText( Misc->GetStaticBox(), wxID_ANY, wxT("NOT LOCKED"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LockStatus->Wrap( -1 );
	bSizer12->Add( m_LockStatus, 0, wxALL, 5 );


	fgSizer12->Add( bSizer12, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );


	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );

	m_WriteAll = new wxButton( Misc->GetStaticBox(), wxID_ANY, wxT("Write All"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_WriteAll, 0, wxALL, 5 );


	fgSizer12->Add( bSizer10, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );


	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );

	m_ReadAll = new wxButton( Misc->GetStaticBox(), wxID_ANY, wxT("Read All"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_ReadAll, 0, wxALL, 5 );


	fgSizer12->Add( bSizer11, 1, wxEXPAND, 5 );


	Misc->Add( fgSizer12, 1, wxEXPAND, 5 );


	Left_Column->Add( Misc, 1, wxEXPAND, 5 );


	Master->Add( Left_Column, 1, wxEXPAND, 5 );

	wxFlexGridSizer* Right_Column;
	Right_Column = new wxFlexGridSizer( 0, 1, 0, 0 );
	Right_Column->SetFlexibleDirection( wxBOTH );
	Right_Column->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* PSB;
	PSB = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Prescaler B outputs") ), wxVERTICAL );

	wxFlexGridSizer* PSB_FLEX;
	PSB_FLEX = new wxFlexGridSizer( 0, 1, 0, 0 );
	PSB_FLEX->SetFlexibleDirection( wxBOTH );
	PSB_FLEX->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* Y2_Y3;
	Y2_Y3 = new wxStaticBoxSizer( new wxStaticBox( PSB->GetStaticBox(), wxID_ANY, wxT("Y2/Y3") ), wxVERTICAL );

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 0, 2, 0, 0 );

	m_staticText20 = new wxStaticText( Y2_Y3->GetStaticBox(), wxID_ANY, wxT("Int. Divider"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	gSizer1->Add( m_staticText20, 0, wxALL, 5 );

	m_staticText21 = new wxStaticText( Y2_Y3->GetStaticBox(), wxID_ANY, wxT("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	gSizer1->Add( m_staticText21, 0, wxALL, 5 );

	m_spinCtrl2 = new wxSpinCtrl( Y2_Y3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	gSizer1->Add( m_spinCtrl2, 0, wxALL, 5 );

	m_spinCtrlDouble7 = new wxSpinCtrlDouble( Y2_Y3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble7->SetDigits( 0 );
	gSizer1->Add( m_spinCtrlDouble7, 0, wxALL, 5 );


	Y2_Y3->Add( gSizer1, 1, wxEXPAND, 5 );


	PSB_FLEX->Add( Y2_Y3, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* Y6;
	Y6 = new wxStaticBoxSizer( new wxStaticBox( PSB->GetStaticBox(), wxID_ANY, wxT("Y6") ), wxVERTICAL );

	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 0, 2, 0, 0 );

	m_staticText38 = new wxStaticText( Y6->GetStaticBox(), wxID_ANY, wxT("Frac. Divider"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	gSizer2->Add( m_staticText38, 0, wxALL, 5 );

	m_staticText39 = new wxStaticText( Y6->GetStaticBox(), wxID_ANY, wxT("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	gSizer2->Add( m_staticText39, 0, wxALL, 5 );

	m_spinCtrlDouble8 = new wxSpinCtrlDouble( Y6->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble8->SetDigits( 0 );
	gSizer2->Add( m_spinCtrlDouble8, 0, wxALL, 5 );

	m_spinCtrlDouble9 = new wxSpinCtrlDouble( Y6->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble9->SetDigits( 0 );
	gSizer2->Add( m_spinCtrlDouble9, 0, wxALL, 5 );


	Y6->Add( gSizer2, 1, wxEXPAND, 5 );


	PSB_FLEX->Add( Y6, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* Y7;
	Y7 = new wxStaticBoxSizer( new wxStaticBox( PSB->GetStaticBox(), wxID_ANY, wxT("Y7") ), wxVERTICAL );

	wxGridSizer* gSizer6;
	gSizer6 = new wxGridSizer( 0, 2, 0, 0 );

	m_staticText24 = new wxStaticText( Y7->GetStaticBox(), wxID_ANY, wxT("Frac. Divider"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	gSizer6->Add( m_staticText24, 0, wxALL, 5 );

	m_staticText25 = new wxStaticText( Y7->GetStaticBox(), wxID_ANY, wxT("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	gSizer6->Add( m_staticText25, 0, wxALL, 5 );

	m_spinCtrlDouble13 = new wxSpinCtrlDouble( Y7->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble13->SetDigits( 0 );
	gSizer6->Add( m_spinCtrlDouble13, 0, wxALL, 5 );

	m_spinCtrlDouble14 = new wxSpinCtrlDouble( Y7->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble14->SetDigits( 0 );
	gSizer6->Add( m_spinCtrlDouble14, 0, wxALL, 5 );


	Y7->Add( gSizer6, 1, wxEXPAND, 5 );


	PSB_FLEX->Add( Y7, 1, wxEXPAND, 5 );


	PSB->Add( PSB_FLEX, 1, wxEXPAND, 5 );


	Right_Column->Add( PSB, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* PSA;
	PSA = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Prescaler A outputs") ), wxVERTICAL );

	wxFlexGridSizer* PSA_FLEX;
	PSA_FLEX = new wxFlexGridSizer( 0, 1, 0, 0 );
	PSA_FLEX->SetFlexibleDirection( wxBOTH );
	PSA_FLEX->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* Y0_Y1;
	Y0_Y1 = new wxStaticBoxSizer( new wxStaticBox( PSA->GetStaticBox(), wxID_ANY, wxT("Y0/Y1") ), wxVERTICAL );

	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 0, 2, 0, 0 );

	m_staticText40 = new wxStaticText( Y0_Y1->GetStaticBox(), wxID_ANY, wxT("Int. Divider"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	gSizer3->Add( m_staticText40, 0, wxALL, 5 );

	m_staticText41 = new wxStaticText( Y0_Y1->GetStaticBox(), wxID_ANY, wxT("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	gSizer3->Add( m_staticText41, 0, wxALL, 5 );

	m_spinCtrl3 = new wxSpinCtrl( Y0_Y1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	gSizer3->Add( m_spinCtrl3, 0, wxALL, 5 );

	m_spinCtrlDouble10 = new wxSpinCtrlDouble( Y0_Y1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble10->SetDigits( 0 );
	gSizer3->Add( m_spinCtrlDouble10, 0, wxALL, 5 );


	Y0_Y1->Add( gSizer3, 1, wxEXPAND, 5 );


	PSA_FLEX->Add( Y0_Y1, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* Y4;
	Y4 = new wxStaticBoxSizer( new wxStaticBox( PSA->GetStaticBox(), wxID_ANY, wxT("Y4") ), wxVERTICAL );

	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 0, 2, 0, 0 );

	m_staticText44 = new wxStaticText( Y4->GetStaticBox(), wxID_ANY, wxT("Frac. Divider"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	gSizer4->Add( m_staticText44, 0, wxALL, 5 );

	m_staticText45 = new wxStaticText( Y4->GetStaticBox(), wxID_ANY, wxT("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( -1 );
	gSizer4->Add( m_staticText45, 0, wxALL, 5 );

	m_spinCtrlDouble11 = new wxSpinCtrlDouble( Y4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble11->SetDigits( 0 );
	gSizer4->Add( m_spinCtrlDouble11, 0, wxALL, 5 );

	m_spinCtrlDouble12 = new wxSpinCtrlDouble( Y4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble12->SetDigits( 0 );
	gSizer4->Add( m_spinCtrlDouble12, 0, wxALL, 5 );


	Y4->Add( gSizer4, 1, wxEXPAND, 5 );


	PSA_FLEX->Add( Y4, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* Y5;
	Y5 = new wxStaticBoxSizer( new wxStaticBox( PSA->GetStaticBox(), wxID_ANY, wxT("Y5") ), wxVERTICAL );

	wxGridSizer* gSizer41;
	gSizer41 = new wxGridSizer( 0, 2, 0, 0 );

	m_staticText441 = new wxStaticText( Y5->GetStaticBox(), wxID_ANY, wxT("Frac. Divider"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText441->Wrap( -1 );
	gSizer41->Add( m_staticText441, 0, wxALL, 5 );

	m_staticText451 = new wxStaticText( Y5->GetStaticBox(), wxID_ANY, wxT("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText451->Wrap( -1 );
	gSizer41->Add( m_staticText451, 0, wxALL, 5 );

	m_spinCtrlDouble111 = new wxSpinCtrlDouble( Y5->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble111->SetDigits( 0 );
	gSizer41->Add( m_spinCtrlDouble111, 0, wxALL, 5 );

	m_spinCtrlDouble121 = new wxSpinCtrlDouble( Y5->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble121->SetDigits( 0 );
	gSizer41->Add( m_spinCtrlDouble121, 0, wxALL, 5 );


	Y5->Add( gSizer41, 1, wxEXPAND, 5 );


	PSA_FLEX->Add( Y5, 1, wxEXPAND, 5 );


	PSA->Add( PSA_FLEX, 1, wxEXPAND, 5 );


	Right_Column->Add( PSA, 1, wxEXPAND, 5 );


	Master->Add( Right_Column, 1, wxEXPAND, 5 );


	this->SetSizer( Master );
	this->Layout();
	Master->Fit( this );
}

CDCM_controlpanel::~CDCM_controlpanel()
{
}
