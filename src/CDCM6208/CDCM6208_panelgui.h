#ifndef __CDCM6208_panelgui__
#define __CDCM6208_panelgui__

/**
@file
Subclass of MyPanel5, which is generated by wxFormBuilder.
*/

#include "CDCM6208_GUI.h"
#include "lime/LimeSuite.h"
//// end generated include

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
protected:
   lms_device_t* lmsControl;
};

#endif // __CDCM6208_panelgui__
