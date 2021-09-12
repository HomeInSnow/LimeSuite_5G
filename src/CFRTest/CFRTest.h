#ifndef PNL_QSPARK_H
#define PNL_QSPARK_H

#include <map>
#include <wx/frame.h>
#include <wx/panel.h>
#include "lime/LimeSuite.h"

class wxStaticText;
class wxFlexGridSizer;
class wxButton;
class wxSpinCtrl;
class wxComboBox;
class wxCheckBox;
class wxTextCtrl;
class wxChoice;
class wxRadioButton;
class wxSpinCtrlDouble;

class CFRTest : public wxFrame
{
public:
    
    CFRTest(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString &title = wxEmptyString, 
    const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, int style = 0);
    
    void Initialize(lms_device_t *pControl);
    virtual ~CFRTest();
 
    wxButton* btnUpdateAll;
    wxButton* btnLoadSettings;
    wxButton* btnSaveSettings;

    wxCheckBox* chkEN_RXTSP;
    wxCheckBox* chkRX_DCCORR_BYP;
    wxCheckBox* chkRX_PHCORR_BYP;
    wxCheckBox* chkRX_GCORR_BYP;
    wxStaticText * txtRX_PHCORR;
    wxStaticText * txtRX_GCORR;
    wxSpinCtrl* spinRX_GCORRQ;
    wxSpinCtrl* spinRX_GCORRI;
    wxSpinCtrl* spinRX_PHCORR;
    wxSpinCtrl* spinRX_DCCORRAVG;
    
    wxButton*  setFIR1;
    wxButton*  setFIR2;    
    wxRadioButton* rbChannelA;
    wxRadioButton* rbChannelB;
  
	// reg 0x45
    wxCheckBox * chkSLEEP_CFR;
    wxCheckBox * chkBYPASS_CFR;
	wxCheckBox * chkODD_CFR;
    wxCheckBox * chkBYPASSGAIN_CFR;
	wxCheckBox * chkSLEEP_FIR;
	wxCheckBox * chkBYPASS_FIR;
	wxCheckBox * chkODD_FIR;
	wxCheckBox * chkDEL_HB;
    wxChoice * cmbINTER_CFR;
	wxCheckBox * chkSLEEP_CFR_chB;
	wxCheckBox * chkBYPASS_CFR_chB;
	wxCheckBox * chkODD_CFR_chB;
    wxCheckBox * chkBYPASSGAIN_CFR_chB;
	wxCheckBox * chkSLEEP_FIR_chB;
	wxCheckBox * chkBYPASS_FIR_chB;
	wxCheckBox * chkODD_FIR_chB;
	wxCheckBox * chkDEL_HB_chB; 
    wxChoice* cmbINTER_CFR_chB; 

    wxCheckBox * chkTxEN;
    wxCheckBox * chkCaptureEN;
    wxCheckBox * chkResetN;
    
    wxSpinCtrlDouble * thresholdSpin; 
    wxSpinCtrlDouble * thresholdGain;    
    wxSpinCtrlDouble * thresholdSpin_chB;   
    wxSpinCtrlDouble * thresholdGain_chB;  
	wxSpinCtrl * spinCFR_ORDER;
	wxSpinCtrl * spinCFR_ORDER_chB;


    void OnThresholdChanged(wxCommandEvent& event);
    void OnGainChanged(wxCommandEvent& event);
    void OnOrderChanged(wxCommandEvent& event);
    void OnInterpolationChanged(wxCommandEvent &event);

    static const long ID_BUTTON_UPDATEALL;
    static const long ID_VCXOCV;

    void OnbtnUpdateAll(wxCommandEvent& event);  	
    void LoadBoardSettings(wxCommandEvent& event);
    void SaveBoardSettings(wxCommandEvent& event);
   
    int chA;

protected:
    void OnReadAll(wxCommandEvent &event);
    void OnWriteAll(wxCommandEvent &event);
    void OnSwitchToChannelA(wxCommandEvent& event);
    void OnSwitchToChannelB(wxCommandEvent& event);
    int SPI_write(lms_device_t * lmsControl, uint16_t address, uint16_t data);
    void UpdateHannCoeff(lms_device_t * lmsControl, uint16_t Filt_N, int chA, int interpolation);
    void onbtnFIRCoefA( wxCommandEvent& event );
    void onbtnFIRCoefB( wxCommandEvent& event );
    void onbtnFIRCoef(int ch, int interpolation);
    void UpdateDegrees();

    struct Register {
        Register();
        Register(unsigned short address, unsigned char msb, unsigned char lsb, unsigned short defaultValue, unsigned short twocomplement);
        unsigned short address;
        unsigned char msb;
        unsigned char lsb;
        unsigned short defaultValue;
        unsigned short twocomplement; 
    };
    std::map<wxObject*, Register> controlsPtr2Registers;
    void RegisterParameterChangeHandler(wxCommandEvent& event);
    void SetRegValue(Register reg, uint16_t newValue);

protected:
    lms_device_t *lmsControl;
    DECLARE_EVENT_TABLE()
};

#endif
