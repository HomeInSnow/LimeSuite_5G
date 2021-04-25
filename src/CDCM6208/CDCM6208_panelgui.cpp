#include "CDCM6208_panelgui.h"

CDCM6208_panelgui::CDCM6208_panelgui( wxWindow* parent )
:
CDCM_controlpanel( parent )
{

}

CDCM6208_panelgui::CDCM6208_panelgui( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
:
CDCM_controlpanel(parent, id, pos, size, style), lmsControl(nullptr)
{

}

void CDCM6208_panelgui::Initialize(lms_device_t* pModule)
{
   lmsControl = pModule;
   UpdateGUI();
   m_Baseaddr->SetValue(wxString::Format(_("%i"),SPI_BASEADDR));
   auto str = wxString::Format(_("%f"),Y2Y3_Frequency/1e6);
      m_Y2Y3_FREQ_req->SetValue(str);
   //Y6 Frequency
   str = wxString::Format(_("%f"),Y6_Frequency/1e6);
      m_Y6_FREQ_req->SetValue(str);
   //Y7 Frequency
   str = wxString::Format(_("%f"),Y7_Frequency/1e6);
      m_Y7_FREQ_req->SetValue(str);
   //Y0Y1 Frequency
   str = wxString::Format(_("%f"),Y0Y1_Frequency/1e6);
      m_Y0Y1_FREQ_req->SetValue(str);
   //Y4 Frequency
   str = wxString::Format(_("%f"),Y4_Frequency/1e6);
      m_Y4_FREQ_req->SetValue(str);
   //Y5 Frequency
   str = wxString::Format(_("%f"),Y5_Frequency/1e6);
      m_Y5_FREQ_req->SetValue(str);

}

void CDCM6208_panelgui::OnChange( wxCommandEvent& event )
{
   auto obj = event.GetEventObject();
   std::string str;
   double d_val;
   int i_val;

   try{
      if (obj == m_PrimaryFreq)
      {
         str = m_PrimaryFreq->GetValue();
         d_val = std::stod(str)*1e6;
         PrimaryF = d_val;
      }
      else if (obj == m_SecondaryFreq)
      {
         str = m_SecondaryFreq->GetValue();
         d_val = std::stod(str)*1e6;
         SecondaryF = d_val;
      }
      else if (obj == m_RDivider)
      {
         str = m_RDivider->GetValue();
         i_val = std::stoi(str);

         if(i_val < 1)
            i_val = 1;
         else if(i_val > 16)
            i_val = 16;

         RDivider = i_val;
      }
      else if (obj == m_MDivider)
      {
         str = m_MDivider->GetValue();
         i_val = std::stoi(str);
         int max_val = (1<<14); //max 14bit value + 1

         if(i_val < 1)
            i_val = 1;
         else if(i_val > max_val)
            i_val = max_val;

         MDivider = i_val; //offset, divider cannot be 0
      }
      else if (obj == m_NMultiplier)
      {
         str = m_NMultiplier->GetValue();
         i_val = std::stoi(str);
         //N consists of two multipliers 8 bit and 10 bit
            // +1 offset intentional
         int max_val = (1<<10) * (1<<8); // theres no +1 offset though...
         if(i_val < 1)
            i_val = 1;
         else if(i_val > max_val)
            i_val = max_val;

         //SolveN(&i_val,&NMultiplier0,&NMultiplier1);

      }
      else if (obj == m_Y0Y1_DIV)
      {
         str = m_Y0Y1_DIV->GetValue();
         i_val = std::stoi(str);
         int max_val = 1<<8;
         if(i_val < 1)
            i_val = 1;
         else if(i_val > max_val)
            i_val = max_val;
         Y0Y1_Divider = i_val;
      }
      else if (obj == m_Y2Y3_DIV)
      {
         str = m_Y2Y3_DIV->GetValue();
         i_val = std::stoi(str);
         int max_val = 1<<8;
         if(i_val < 1)
            i_val = 1;
         else if(i_val > max_val)
            i_val = max_val;
         Y2Y3_Divider = i_val;
      }
      else if (obj == m_Y4_DIV)
      {
         str = m_Y4_DIV->GetValue();
         d_val = std::stod(str);
         //SolveFracDiv(&d_val, &Y4, &Y4_Divider);
      }
      else if (obj == m_Y5_DIV)
      {
         str = m_Y5_DIV->GetValue();
         d_val = std::stod(str);
         //SolveFracDiv(&d_val, &Y5, &Y5_Divider);
      }
      else if (obj == m_Y6_DIV)
      {
         str = m_Y6_DIV->GetValue();
         d_val = std::stod(str);
         //SolveFracDiv(&d_val, &Y6, &Y6_Divider);
      }
      else if (obj == m_Y7_DIV)
      {
         str = m_Y7_DIV->GetValue();
         d_val = std::stod(str);
         //SolveFracDiv(&d_val, &Y7, &Y7_Divider);
         Y7_Divider = d_val; // Ahm, wtf is this?
      }
      else if (obj == m_Baseaddr)
      {
         str = m_Baseaddr->GetValue();
         i_val = std::stoi(str);
         SPI_BASEADDR = i_val;
      }
   }
   catch(std::invalid_argument)
   {
      return;
   }

   //Recalculate();
   UpdateGUI();
}

void CDCM6208_panelgui::OnRadio( wxCommandEvent& event )
{
   bool PrimarySel;
   bool SecondarySel;
   PrimarySel = m_PrimarySel->GetValue();
   SecondarySel = m_SecondarySel->GetValue();

   if(PrimarySel && !SecondarySel)
   {
      InMux = 1;
   }
   else if(!PrimarySel && SecondarySel)
   {
      InMux = 2;
   }
   else
   {
      InMux = 1; // select primary by default
   }
   //Recalculate();
   UpdateGUI();
}

void CDCM6208_panelgui::OnChoice( wxCommandEvent& event )
{
   auto obj = event.GetEventObject();

   if (obj == m_PSA)
   {
      auto val = m_PSA->GetSelection();
      PrescalerA = val + 4;
   }
   else if(obj == m_PSB)
   {
      auto val = m_PSB->GetSelection();
      PrescalerB = val + 4;
   }
   else if(obj == m_CDCM_VER)
   {
      auto val = m_CDCM_VER->GetSelection();
      if (val == 0)
      {
         VCO_MIN = 2390*1e6;
         VCO_MAX = 2550*1e6;
      }
      else if (val == 1)
      {
         VCO_MIN = 2940*1e6;
         VCO_MAX = 3130*1e6;
      }
      CDCM_VER = val;
   }

   //Recalculate();
   UpdateGUI();
}

void CDCM6208_panelgui::OnFreqEntry( wxCommandEvent& event )
{
   try
   {
      Frequency_plan Frequency_plan;
      // Get requested frequencies from GUI
      Frequency_plan.Y0Y1_Frequency  = std::stod((std::string)m_Y0Y1_FREQ_req->GetValue())*1e6;
      Frequency_plan.Y2Y3_Frequency  = std::stod((std::string)m_Y2Y3_FREQ_req->GetValue())*1e6;
      Frequency_plan.Y4_Frequency    = std::stod((std::string)m_Y4_FREQ_req->GetValue())*1e6;
      Frequency_plan.Y5_Frequency    = std::stod((std::string)m_Y5_FREQ_req->GetValue())*1e6;
      Frequency_plan.Y6_Frequency    = std::stod((std::string)m_Y6_FREQ_req->GetValue())*1e6;
      Frequency_plan.Y7_Frequency    = std::stod((std::string)m_Y7_FREQ_req->GetValue())*1e6;
      // Get input frequency
      if (InMux == 1)
         Frequency_plan.Input_Frequency = PrimaryF/RDivider;
      else
         Frequency_plan.Input_Frequency = SecondaryF;

      // Get care mask
      Frequency_plan.int_care_mask = int_care_mask;
      // Find VCO config
      //VCO_config VCOConfig = SolveFreqPlanSingleCDCM(Frequency_plan, VCO_MIN, VCO_MAX);
      // If valid config found
      VCO_config VCOConfig; // added so it would compile
      if (VCOConfig.valid)
      {
         // Update VCO config
         //SolveN(&VCOConfig.Multiplier,&NMultiplier0,&NMultiplier1);
         MDivider   = VCOConfig.Divisor;
         PrescalerA = VCOConfig.Prescaler;
         PrescalerB = VCOConfig.Prescaler;
         //Recalculate();
         m_FrequencyPlanRes->SetLabel("Valid Config Found");
         m_FrequencyPlanRes->SetForegroundColour(wxColour("#00ff00"));
      }
      else
      {
         // Do not update VCO config
         m_FrequencyPlanRes->SetLabel("No Valid Config Found");
         m_FrequencyPlanRes->SetForegroundColour(wxColour("#ff0000"));
      }

      // Update dividers
      Y0Y1_Divider=(int)round((VCOF/PrescalerA)/Frequency_plan.Y0Y1_Frequency);
      Y2Y3_Divider=(int)round((VCOF/PrescalerB)/Frequency_plan.Y2Y3_Frequency);
      double placeholder = (VCOF/PrescalerA)/Frequency_plan.Y4_Frequency;
      //SolveFracDiv(&placeholder,&Y4,&Y4_Divider);
      placeholder = (VCOF/PrescalerA)/Frequency_plan.Y5_Frequency;
      //SolveFracDiv(&placeholder,&Y5,&Y5_Divider);
      placeholder = (VCOF/PrescalerB)/Frequency_plan.Y6_Frequency;
      //SolveFracDiv(&placeholder,&Y6,&Y6_Divider);
      placeholder = (VCOF/PrescalerB)/Frequency_plan.Y7_Frequency;
      //SolveFracDiv(&placeholder,&Y7,&Y7_Divider);
      // Recalculate and display newly calculated values in GUI
      //Recalculate();
      UpdateGUI();
   }
   catch(std::invalid_argument)
   {
      m_FrequencyPlanRes->SetLabel("Invalid Freq Request");
      m_FrequencyPlanRes->SetForegroundColour(wxColour("#ff0000"));
   }
}
void CDCM6208_panelgui::onFP_chk( wxCommandEvent& event )
{

   int_care_mask = 0;
   int_care_mask |= (((int)m_Y0Y1_chk->GetValue())<<0);
   int_care_mask |= (((int)m_Y2Y3_chk->GetValue())<<1);
   int_care_mask |= (((int)m_Y4_chk->GetValue())<<2);
   int_care_mask |= (((int)m_Y5_chk->GetValue())<<3);
   int_care_mask |= (((int)m_Y6_chk->GetValue())<<4);
   int_care_mask |= (((int)m_Y7_chk->GetValue())<<5);
   UpdateGUI();
}

void CDCM6208_panelgui::OnButton( wxCommandEvent& event )
{
   auto obj = event.GetEventObject();
   uint16_t regval;

   if(obj == m_WriteAll)
   {
      // Write all registers
      //write all vals to fpga/cdcm
   }
   else if(obj == m_ReadAll)
   {
      //Request FPGA to read CDCM registers
      // READ ALL REGISTERS
   }
   //Recalculate();
   UpdateGUI();
}

void CDCM6208_panelgui::UpdateGUI()
{
   //Primary input
   auto str = wxString::Format(_("%.3f"), PrimaryF/1e6);
   if(m_PrimaryFreq->GetValue()!=str)
      m_PrimaryFreq->SetValue(str);
   //Secondary Input
   str = wxString::Format(_("%.3f"), SecondaryF/1e6);
   if(m_SecondaryFreq->GetValue()!=str)
      m_SecondaryFreq->SetValue(str);
   //R Divider
   str = wxString::Format(_("%i"), RDivider);
   if(m_RDivider->GetValue()!=str)
      m_RDivider->SetValue(str);
   //MUX Selection
   if (InMux == 2)
   {
      m_PrimarySel->SetValue(false);
      m_SecondarySel->SetValue(true);
   }
   else
   {
      m_PrimarySel->SetValue(true);
      m_SecondarySel->SetValue(false);
   }
   //M Divider
   str = wxString::Format(_("%i"),MDivider);
   if(m_MDivider->GetValue()!=str)
      m_MDivider->SetValue(str);
   //N Multiplier
   str = wxString::Format(_("%i"),NMultiplier0*NMultiplier1);
   if(m_NMultiplier->GetValue()!=str)
      m_NMultiplier->SetValue(str);
   //B Prescaler
   if(m_PSB->GetSelection()!=PrescalerB-4)
      m_PSB->SetSelection(PrescalerB-4);
   //A Prescaler
   if(m_PSA->GetSelection()!=PrescalerA-4)
      m_PSA->SetSelection(PrescalerA-4);
   //CDCM Version
   if(m_CDCM_VER->GetSelection()!=CDCM_VER)
      m_CDCM_VER->SetSelection(CDCM_VER);
   //VCO Range
   m_VCORANGE->SetLabel(wxString::Format(_("%.0f - %.0f"), VCO_MIN/1e6, VCO_MAX/1e6));
   //VCO Frequency
   m_VCOFREQ->SetLabel(wxString::Format(_("%.2f MHz"), VCOF/1e6));
   if(VCOF > VCO_MAX || VCOF < VCO_MIN)
      m_VCOFREQ->SetForegroundColour(wxColour("#ff0000"));
   else
      m_VCOFREQ->SetForegroundColour(wxColour("#000000"));
   //Y2Y3 divider
   str = wxString::Format(_("%i"),Y2Y3_Divider);
   if(m_Y2Y3_DIV->GetValue()!=str)
      m_Y2Y3_DIV->SetValue(str);
   //Y2Y3 Frequency
   str = wxString::Format(_("%f"),Y2Y3_Frequency/1e6);
   if(m_Y2Y3_FREQ->GetValue()!=str)
      m_Y2Y3_FREQ->SetValue(str);
   //Y6 divider
   str = wxString::Format(_("%f"),Y6_Divider);
   if(m_Y6_DIV->GetValue()!=str)
      m_Y6_DIV->SetValue(str);
   //Y6 Frequency
   str = wxString::Format(_("%f"),Y6_Frequency/1e6);
   if(m_Y6_FREQ->GetValue()!=str)
      m_Y6_FREQ->SetValue(str);
   //Y7 divider
   str = wxString::Format(_("%f"),Y7_Divider);
   if(m_Y7_DIV->GetValue()!=str)
      m_Y7_DIV->SetValue(str);
   //Y7 Frequency
   str = wxString::Format(_("%f"),Y7_Frequency/1e6);
   if(m_Y7_FREQ->GetValue()!=str)
      m_Y7_FREQ->SetValue(str);
   //Y0Y1 divider
   str = wxString::Format(_("%i"),Y0Y1_Divider);
   if(m_Y0Y1_DIV->GetValue()!=str)
      m_Y0Y1_DIV->SetValue(str);
   //Y0Y1 Frequency
   str = wxString::Format(_("%f"),Y0Y1_Frequency/1e6);
   if(m_Y0Y1_FREQ->GetValue()!=str)
      m_Y0Y1_FREQ->SetValue(str);
   //Y4 divider
   str = wxString::Format(_("%f"),Y4_Divider);
   if(m_Y4_DIV->GetValue()!=str)
      m_Y4_DIV->SetValue(str);
   //Y4 Frequency
   str = wxString::Format(_("%f"),Y4_Frequency/1e6);
   if(m_Y4_FREQ->GetValue()!=str)
      m_Y4_FREQ->SetValue(str);
   //Y5 divider
   str = wxString::Format(_("%f"),Y5_Divider);
   if(m_Y5_DIV->GetValue()!=str)
      m_Y5_DIV->SetValue(str);
   //Y5 Frequency
   str = wxString::Format(_("%f"),Y5_Frequency/1e6);
   if(m_Y5_FREQ->GetValue()!=str)
      m_Y5_FREQ->SetValue(str);
   //PLL Lock status
   if(lock_status)
   {
      m_LockStatus->SetLabel("PLL LOCKED");
      m_LockStatus->SetForegroundColour(wxColour("#00ff00"));
   }
   else
   {
      m_LockStatus->SetLabel("NOT LOCKED");
      m_LockStatus->SetForegroundColour(wxColour("#ff0000"));
   }
   m_Y0Y1_FREQ_req->Enable(int_care_mask&(1<<0));
   m_Y2Y3_FREQ_req->Enable(int_care_mask&(1<<1));
   m_Y4_FREQ_req->Enable(int_care_mask&(1<<2));
   m_Y5_FREQ_req->Enable(int_care_mask&(1<<3));
   m_Y6_FREQ_req->Enable(int_care_mask&(1<<4));
   m_Y7_FREQ_req->Enable(int_care_mask&(1<<5));
}

