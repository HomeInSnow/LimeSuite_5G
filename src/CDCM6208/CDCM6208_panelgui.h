#ifndef __CDCM6208_panelgui__
#define __CDCM6208_panelgui__

/**
@file
Subclass of MyPanel5, which is generated by wxFormBuilder.
*/

#include "CDCM6208_GUI.h"
#include "lime/LimeSuite.h"
//// end generated include

struct Fractional_config {
  bool fractional;
  int prescaler;
  int integer_part;
  int fractional_part;
} ;

/** Implementing MyPanel5 */
class CDCM6208_panelgui : public CDCM_controlpanel
{


	public:
		/** Constructor */
		CDCM6208_panelgui( wxWindow* parent );
	//// end generated class members
   CDCM6208_panelgui( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
   void Initialize(lms_device_t* pControl);
   void UpdateGUI();

   double PrimaryF=30.72*1e6;
   double SecondaryF=30.72*1e6;
   int RDivider=1;
   int InMux=1;
   int MDivider=1;
   double VCOF=3072*1e6;
   int PrescalerA=4;
   int PrescalerB=4;
   int NMultiplier0=5; //8bit
   int NMultiplier1=5; //10bit
   double VCO_MIN=2940*1e6;
   double VCO_MAX=3130*1e6;
   int CDCM_VER=1;
   int Y0Y1_Divider=25;
   int Y2Y3_Divider=25;
   double Y4_Divider=25;
   Fractional_config Y4;
   double Y5_Divider=25;
   Fractional_config Y5;
   double Y6_Divider=25;
   Fractional_config Y6;
   double Y7_Divider=25;
   Fractional_config Y7;
   double Y0Y1_Frequency=30.72*1e6;
   double Y2Y3_Frequency=30.72*1e6;
   double Y2_Frequency=30.72*1e6;
   double Y3_Frequency=30.72*1e6;
   double Y4_Frequency=30.72*1e6;
   double Y5_Frequency=30.72*1e6;
   double Y6_Frequency=30.72*1e6;
   double Y7_Frequency=30.72*1e6;
   bool lock_status=true;
   int SPI_BASEADDR=288;
protected:

   lms_device_t* lmsControl;
   // // Virtual event handlers, override them in your derived class
   void OnChange( wxCommandEvent& event )override;
   void OnRadio( wxCommandEvent& event )override;
   void OnChoice( wxCommandEvent& event )override;
   void OnFreqEntry( wxCommandEvent& event )override;
   void OnButton( wxCommandEvent& event )override;
   // Other functions
   void Recalculate();
   void SolveN(int* Target, int* Mult8bit, int* Mult10bit);
   void SolveFracDiv(double* target, Fractional_config* config, double* result);
   void CalculateFracDiv(double* target, Fractional_config* config);
};

#endif // __CDCM6208_panelgui__
