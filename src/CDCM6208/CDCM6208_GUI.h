///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Mar 22 2021)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class CDCM6208_panelgui;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/radiobut.h>
#include <wx/choice.h>
#include <wx/statbox.h>
#include <wx/button.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CDCM6208_CONTROL
///////////////////////////////////////////////////////////////////////////////
class CDCM6208_CONTROL : public wxFrame
{
	private:

	protected:
		wxNotebook* m_CDCMtabs;
		CDCM6208_panelgui* m_CDCM2;

	public:
		CDCM6208_panelgui* m_CDCM1;

		CDCM6208_CONTROL( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~CDCM6208_CONTROL();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CDCM_controlpanel
///////////////////////////////////////////////////////////////////////////////
class CDCM_controlpanel : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText1;
		wxSpinCtrlDouble* m_PrimaryF;
		wxStaticText* m_staticText2;
		wxSpinCtrlDouble* m_SecondaryF;
		wxStaticText* m_staticText3;
		wxSpinCtrl* m_RDivider;
		wxStaticText* m_staticText4;
		wxRadioButton* m_PrimarySel;
		wxRadioButton* m_SecondarySel;
		wxStaticText* m_staticText5;
		wxSpinCtrlDouble* m_MDivider;
		wxStaticText* m_staticText6;
		wxSpinCtrlDouble* m_NMultiplier;
		wxStaticText* m_staticText7;
		wxStaticText* m_VCOFREQ;
		wxStaticText* m_staticText9;
		wxChoice* m_PSB;
		wxStaticText* m_staticText10;
		wxChoice* m_PSA;
		wxStaticText* m_staticText50;
		wxChoice* m_CDCM_VER;
		wxStaticText* m_staticText51;
		wxStaticText* m_VCORANGE;
		wxStaticText* m_staticText26;
		wxSpinCtrl* m_spinCtrl4;
		wxStaticText* m_staticText27;
		wxStaticText* m_LockStatus;
		wxButton* m_WriteAll;
		wxButton* m_ReadAll;
		wxStaticText* m_staticText20;
		wxStaticText* m_staticText21;
		wxSpinCtrl* m_spinCtrl2;
		wxSpinCtrlDouble* m_spinCtrlDouble7;
		wxStaticText* m_staticText38;
		wxStaticText* m_staticText39;
		wxSpinCtrlDouble* m_spinCtrlDouble8;
		wxSpinCtrlDouble* m_spinCtrlDouble9;
		wxStaticText* m_staticText24;
		wxStaticText* m_staticText25;
		wxSpinCtrlDouble* m_spinCtrlDouble13;
		wxSpinCtrlDouble* m_spinCtrlDouble14;
		wxStaticText* m_staticText40;
		wxStaticText* m_staticText41;
		wxSpinCtrl* m_spinCtrl3;
		wxSpinCtrlDouble* m_spinCtrlDouble10;
		wxStaticText* m_staticText44;
		wxStaticText* m_staticText45;
		wxSpinCtrlDouble* m_spinCtrlDouble11;
		wxSpinCtrlDouble* m_spinCtrlDouble12;
		wxStaticText* m_staticText441;
		wxStaticText* m_staticText451;
		wxSpinCtrlDouble* m_spinCtrlDouble111;
		wxSpinCtrlDouble* m_spinCtrlDouble121;

	public:

		CDCM_controlpanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~CDCM_controlpanel();

};

