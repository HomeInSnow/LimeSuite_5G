/**
@file	Si5351C.cpp
@author	Lime Microsystems
@brief 	Configuring of CDCM6208
*/

#include "CDCM6208.h"

CDCM6208::CDCM6208(wxWindow* parent):CDCM6208_CONTROL( parent ), device(nullptr)
{}

CDCM6208::CDCM6208(wxWindow* parent, wxWindowID id , const wxString& title , const wxPoint& pos , const wxSize& size , long style )
:
CDCM6208_CONTROL(parent,id,title,pos,size,style), device(nullptr)
{}


void CDCM6208::Initialize(lms_device_t* dev)
{
    device = dev;
    CDCM6208::m_CDCM1->SPI_BASEADDR=288;
    CDCM6208::m_CDCM1->Initialize(device);
    CDCM6208::m_CDCM2->SPI_BASEADDR=320;
    CDCM6208::m_CDCM2->Initialize(device);
}
