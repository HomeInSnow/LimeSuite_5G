/**
@file	Si5351C.cpp
@author	Lime Microsystems
@brief 	Configuring of CDCM6208
*/

#include "CDCM6208_framegui.h"

CDCM6208_framegui::CDCM6208_framegui(wxWindow* parent):CDCM6208_CONTROL( parent ), device(nullptr)
{}

CDCM6208_framegui::CDCM6208_framegui(wxWindow* parent, wxWindowID id , const wxString& title , const wxPoint& pos , const wxSize& size , long style )
:
CDCM6208_CONTROL(parent,id,title,pos,size,style), device(nullptr)
{}

void CDCM6208_framegui::Initialize(lms_device_t* dev)
{
    device = dev;
    CDCM6208_framegui::m_CDCM1->Initialize(device, CDCM1_BASE_ADDR);
    CDCM6208_framegui::m_CDCM2->Initialize(device, CDCM2_BASE_ADDR);
}
