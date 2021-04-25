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

         SolveN(&i_val,&NMultiplier0,&NMultiplier1);

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
         SolveFracDiv(&d_val, &Y4, &Y4_Divider);
      }
      else if (obj == m_Y5_DIV)
      {
         str = m_Y5_DIV->GetValue();
         d_val = std::stod(str);
         SolveFracDiv(&d_val, &Y5, &Y5_Divider);
      }
      else if (obj == m_Y6_DIV)
      {
         str = m_Y6_DIV->GetValue();
         d_val = std::stod(str);
         SolveFracDiv(&d_val, &Y6, &Y6_Divider);
      }
      else if (obj == m_Y7_DIV)
      {
         str = m_Y7_DIV->GetValue();
         d_val = std::stod(str);
         SolveFracDiv(&d_val, &Y7, &Y7_Divider);
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

   Recalculate();
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
   Recalculate();
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

   Recalculate();
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
      VCO_config VCOConfig = SolveFreqPlanSingleCDCM(Frequency_plan, VCO_MIN, VCO_MAX);
      // If valid config found
      if (VCOConfig.valid)
      {
         // Update VCO config
         SolveN(&VCOConfig.Multiplier,&NMultiplier0,&NMultiplier1);
         MDivider   = VCOConfig.Divisor;
         PrescalerA = VCOConfig.Prescaler;
         PrescalerB = VCOConfig.Prescaler;
         Recalculate();
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
      SolveFracDiv(&placeholder,&Y4,&Y4_Divider);
      placeholder = (VCOF/PrescalerA)/Frequency_plan.Y5_Frequency;
      SolveFracDiv(&placeholder,&Y5,&Y5_Divider);
      placeholder = (VCOF/PrescalerB)/Frequency_plan.Y6_Frequency;
      SolveFracDiv(&placeholder,&Y6,&Y6_Divider);
      placeholder = (VCOF/PrescalerB)/Frequency_plan.Y7_Frequency;
      SolveFracDiv(&placeholder,&Y7,&Y7_Divider);
      // Recalculate and display newly calculated values in GUI
      Recalculate();
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
      //REG1
      regval = ((MDivider-1) << 2) | ((NMultiplier1-1) >>8);
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+1,regval);
      //REG2
      regval = ((NMultiplier1-1)<<8) | (NMultiplier0-1);
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+2,regval);
      //REG3
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+3,&regval);
      regval &= ~0xF;
      regval |= ((PrescalerA-4)&3);
      regval |= ((PrescalerB-4)&3)<<2;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+3,regval);
      //REG4
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+4,&regval);
      regval &= ~(0x1F00);
      regval |= ((RDivider-1)&0xF)<<8;
      regval |= ((InMux-1)&1) <<12;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+4,regval);
      //REG6
      regval = 0 | ((Y0Y1_Divider-1)&0xFF);
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+6,regval);
      //REG8
      regval = 0 | ((Y2Y3_Divider-1)&0xFF);
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+8,regval);
      ////Y4
      //REG9
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+9,&regval);
      regval &= ~0x1E00;
      regval |= (((int)Y4.fractional)<<9);
      regval |= ((Y4.prescaler-2)&7)<<10;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+9,regval);
      //REG10
      regval = 0 | (((Y4.integer_part-1)&0xFF)<<4);
      regval |= (Y4.fractional_part>>16)&0xF;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+10,regval);
      //REG11
      regval = Y4.fractional_part&0xFFFF;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+11,regval);
      ////Y5
      //REG12
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+12,&regval);
      regval &= ~0x1E00;
      regval |= (((int)Y5.fractional)<<9);
      regval |= ((Y5.prescaler-2)&7)<<10;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+12,regval);
      //REG13
      regval = 0 | (((Y5.integer_part-1)&0xFF)<<4);
      regval |= (Y5.fractional_part>>16)&0xF;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+13,regval);
      //REG14
      regval = Y5.fractional_part&0xFFFF;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+14,regval);
      ////Y6
      //REG15
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+15,&regval);
      regval &= ~0x1E00;
      regval |= (((int)Y6.fractional)<<9);
      regval |= ((Y6.prescaler-2)&7)<<10;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+15,regval);
      //REG16
      regval = 0 | (((Y6.integer_part-1)&0xFF)<<4);
      regval |= (Y6.fractional_part>>16)&0xF;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+16,regval);
      //REG17
      regval = Y6.fractional_part&0xFFFF;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+17,regval);
      ////Y7
      //REG18
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+18,&regval);
      regval &= ~0x1E00;
      regval |= (((int)Y7.fractional)<<9);
      regval |= ((Y7.prescaler-2)&7)<<10;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+18,regval);
      //REG19
      regval = 0 | (((Y7.integer_part-1)&0xFF)<<4);
      regval |= (Y7.fractional_part>>16)&0xF;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+19,regval);
      //REG20
      regval = Y7.fractional_part&0xFFFF;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+20,regval);
      //LOAD TO CDCM
      regval = 1;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+21,regval);
      //GET CONFIG RESULT
      // Ask FPGA to retrieve CDCM register data
      uint8_t timeout = 5;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+24,regval);
      while(regval != 2 && timeout > 0){
         LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+24,&regval);
         timeout--;
      }
      // Lock status (21 CDCM register is mapped to 22 FPGA register)
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+22,&regval);
      lock_status = !(regval>>2)&1;


      //write all vals to fpga/cdcm
   }
   else if(obj == m_ReadAll)
   {
      //Request FPGA to read CDCM registers
      regval = 1;
      uint8_t timeout = 5;
      LMS_WriteFPGAReg(lmsControl,SPI_BASEADDR+24,regval);
      while(regval != 2 && timeout > 0){
         LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+24,&regval);
         timeout--;
      }
      //Download and parse register values from FPGA
      //REG1
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+1,&regval);
      MDivider = (regval >> 2) + 1;
      NMultiplier1 = 0 | ((regval & 3)<<8);
      //REG2
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+2,&regval);
      NMultiplier1 |= (regval>>8);
      NMultiplier1 += 1;
      NMultiplier0 = (regval & 0xFF) + 1;
      //REG3
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+3,&regval);
      PrescalerA = (regval & 3)+4;
      PrescalerB = ((regval >>2)&3)+4;
      //REG4
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+4,&regval);
      RDivider = ((regval >> 8) & 0xF)+1;
      InMux = ((regval >> 12)&1)+1;
      //REG6
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+6,&regval);
      Y0Y1_Divider = (regval&0xFF)+1;
      //REG8
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+8,&regval);
      Y2Y3_Divider = (regval&0xFF)+1;
      ////Y4 fractional
      //REG9
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+9,&regval);
      Y4.fractional = (bool)((regval>>9)&1);
      Y4.prescaler = ((regval>>10)&7)+2;
      //REG10
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+10,&regval);
      Y4.integer_part = ((regval>>4)&0xFF)+1;
      Y4.fractional_part = 0 | ((regval&0xF)<<16);
      //REG11
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+11,&regval);
      Y4.fractional_part |= regval;
      CalculateFracDiv(&Y4_Divider, &Y4);
      ////Y5 fractional
      //REG12
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+12,&regval);
      Y5.fractional = (bool)((regval>>9)&1);
      Y5.prescaler = ((regval>>10)&7)+2;
      //REG13
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+13,&regval);
      Y5.integer_part = ((regval>>4)&0xFF)+1;
      Y5.fractional_part = 0 | ((regval&0xF)<<16);
      //REG14
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+14,&regval);
      Y5.fractional_part |= regval;
      CalculateFracDiv(&Y5_Divider, &Y5);
      ////Y6 fractional
      //REG15
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+15,&regval);
      Y6.fractional = (bool)((regval>>9)&1);
      Y6.prescaler = ((regval>>10)&7)+2;
      //REG16
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+16,&regval);
      Y6.integer_part = ((regval>>4)&0xFF)+1;
      Y6.fractional_part = 0 | ((regval&0xF)<<16);
      //REG17
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+17,&regval);
      Y6.fractional_part |= regval;
      CalculateFracDiv(&Y6_Divider, &Y6);
      ////Y7 fractional
      //REG18
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+18,&regval);
      Y7.fractional = (bool)((regval>>9)&1);
      Y7.prescaler = ((regval>>10)&7)+2;
      //REG19
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+19,&regval);
      Y7.integer_part = ((regval>>4)&0xFF)+1;
      Y7.fractional_part = 0 | ((regval&0xF)<<16);
      //REG20
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+20,&regval);
      Y7.fractional_part |= regval;
      CalculateFracDiv(&Y7_Divider, &Y7);
      // Lock status (21 CDCM register is mapped to 22 FPGA register)
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+22,&regval);
      lock_status = !(regval>>2)&1;
      //CDCM version
      LMS_ReadFPGAReg(lmsControl,SPI_BASEADDR+23,&regval);
      CDCM_VER = (regval>>3)&7;
   }
   Recalculate();
   UpdateGUI();
}

int CDCM6208_panelgui::SolveN(int* Target, int* Mult8bit, int* Mult10bit)
{
   //find multiplier combination to get desired ratio
   double res;
   for(int i10 = 1; i10 <= 1024; i10++)
   {
      res = double(*Target)/i10;
      if(res < (1<<8)) //check max value
      {  // check if res is integer
         if(isInteger(res))
         {
            *Mult8bit  = (int)res;
            *Mult10bit = i10;
            return 0;
         }
      }
   }
   return 1;
}

// TODO: this should not be needed, needs to be done internally.
void CDCM6208_panelgui::Recalculate()
{
   if(InMux==1)
      VCOF = PrimaryF/RDivider;
   else
      VCOF = SecondaryF;

   VCOF = VCOF / MDivider;
   VCOF = VCOF * PrescalerA;
   VCOF = VCOF * NMultiplier0 * NMultiplier1;

   Y0Y1_Frequency = (VCOF/PrescalerA)/Y0Y1_Divider;
   Y4_Frequency = (VCOF/PrescalerA)/Y4_Divider;
   Y5_Frequency = (VCOF/PrescalerA)/Y5_Divider;

   Y2Y3_Frequency = (VCOF/PrescalerB)/Y2Y3_Divider;
   Y6_Frequency = (VCOF/PrescalerB)/Y6_Divider;
   Y7_Frequency = (VCOF/PrescalerB)/Y7_Divider;
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

void CDCM6208_panelgui::SolveFracDiv(double* target, Fractional_config* config, double* result)
{
   //Calculate max fractional val (integer value is offset by +1)
   //8 bits integer value, 20 bits fractional value
   // 1111 1111  1111 1111 1111 1111 1111
   // \  int  /  \ fractional           /
   //    +1
   // 1 0000 0000 1111 1111 1111 1111 1111
   // \  int    / \ fractional           /
   *result = *target;
   double max_fractional_val = (((1<<28) + ((1<<20)-1))/(double)(1<<20))*3;
   //Check bounds
   if(*result < 1)
      *result = 1;
   else if(*result > max_fractional_val)
   {
      *result = max_fractional_val;
   }
   config->fractional = true;

   // Divider must be multiplied by 2 or 3
   double div2_result;
   double div3_result;
   int div2_fixed;
   int div3_fixed;
   div2_result = *result / 2;
   div3_result = *result / 3;

   //cast float into 8.20 fixed point integer
   div2_fixed = (int)(div2_result * ((1<<20)&((1<<28)-1)));
   //recast back to provide float result to GUI
   div2_result = (float)div2_fixed / (1<<20);
   //cast float into 8.20 fixed point integer
   div3_fixed = (int)(div3_result * ((1<<20)&((1<<28)-1)));
   //recast back to provide float result to GUI
   div3_result = (float)div3_fixed / (1<<20);
   // find best match
   double div2_difference = abs(*result - (div2_result * 2.0));
   double div3_difference = abs(*result - (div3_result * 3.0));
   if(div2_difference < div3_difference)
   {
      config->integer_part = (div2_fixed >> 20) & ((1<<8) - 1);
      config->fractional_part = div2_fixed & ((1<<20) - 1);
      config->prescaler = 2;
      *result = div2_result*2;
   }
   else
   {
      config->integer_part = (div3_fixed >> 20) & ((1<<8) - 1);
      config->fractional_part = div3_fixed & ((1<<20) - 1);
      config->prescaler = 3;
      *result = div3_result*3;
   }
}


void CDCM6208_panelgui::CalculateFracDiv(double* target, Fractional_config* config)
{
   if(config->fractional)
   {
      *target = config->fractional_part;
      *target = *target / (1<<20);
      *target = *target + config->integer_part;
      *target = *target * config->prescaler;
   }
   else
   {
      *target = config->integer_part;
   }
}
bool CDCM6208_panelgui::isInteger(double var)
{
   return (var==(int64_t)var);
}

double CDCM6208_panelgui::dec2frac(double target, int* num, int* den)
{
   //constant for CDCM6208
   int max_num_bits = 18;
   int max_den_bits = 14;
   //
   uint64_t l_num = *num;
   uint64_t l_den = *den;
   double l_target = target;
   l_den = 1;
   while(isInteger(target)==false)
   {
      l_den   *=10;
      target *=10;
   }
   l_num=(uint64_t)target;
   uint64_t gcd = findGCD(*num,l_den);
   l_num/=gcd;
   l_den/=gcd;
   // find out how many bits are used by numerator and denominator
   int num_bits, den_bits;
   for(int i=0; i<64; i++)
   {
      if((l_num >> i) > 0)
         num_bits = i;
      if((l_den >> i) > 0)
         den_bits = i;
   }
   //offset by one
   num_bits++;
   den_bits++;
   // truncate if needed
   while((num_bits > max_num_bits) || (den_bits > max_den_bits))
   {
      l_den = l_den >> 1;
      l_num = l_num >> 1;
      num_bits--;
      den_bits--;
   }

   // Check if numerator value valid for CDCM6208
   int placeholder;
   while(SolveN((int*)&l_num,&placeholder,&placeholder) == 1)
   {
      l_den = l_den >> 1;
      l_num = l_num >> 1;
   }
   // return values
   *num = (int)l_num;
   *den = (int)l_den;
   double result_freq = *num/(double)*den;
   return (abs(1-(l_target/result_freq)));
}



std::vector<VCO_config> CDCM6208_panelgui::FindValidVCOFreqs(double lcm, double VCOmin, double VCOmax)
{
   // find number of valid vco freqs for each prescaler
   std::vector<VCO_config> Config_vector;
   VCO_config placeholder_struct;
   double lo_freq;
   double hi_freq;
   double frequency;
   int l_have_error=0;
   for (int prescaler=4; prescaler<=6; prescaler++)
   {
      //Find low and high bounds for current prescaler
      lo_freq=VCOmin/prescaler;
      hi_freq=VCOmax/prescaler;
      frequency = ceil(lo_freq/lcm)*lcm;
      while(frequency < hi_freq)
      {
            placeholder_struct.Prescaler = prescaler;
            placeholder_struct.Frequency = frequency;
            Config_vector.push_back(placeholder_struct);
            frequency += lcm;
      }
   }
   return Config_vector;
}



//Euclidean method for finding greatest common divisor
uint64_t CDCM6208_panelgui::findGCD(uint64_t a, uint64_t b)
{
      if (b == 0)
      return a;
      return findGCD(b, a % b);
}

int CDCM6208_panelgui::findlownum(std::vector<VCO_config> &input)
{
   int min_nom = 0xFFFFFFF;
   int index = 0;
   int curr_val;
   for(int i=0; i<input.size(); i++)
   {
      curr_val = input[i].Multiplier * input[i].Prescaler;
      if(curr_val < min_nom)
      {
         min_nom = curr_val;
         index = i;
      }
   }
   return index;
}

int CDCM6208_panelgui::findlowerr(std::vector<VCO_config> &input)
{
   double min_err = 100e6;
   int index = 0;
   for(int i=0; i<input.size(); i++)
   {
      if(input[i].freq_error < min_err)
      {
         min_err = input[i].freq_error;
         index = i;
      }
   }
   return index;

}

int CDCM6208_panelgui::findbestconfig(std::vector<VCO_config> &input, int have_error)
{
   int no_error = input.size()-have_error;
   // all configs have freq. errors
   if(no_error==0)
   {
      return findlowerr(input);
   }
   // no configs have freq. errors
   else if(have_error==0)
   {
      return findlownum(input);
   }
   else
   {
      for(int i=(input.size()-1);i>=0;i--)
      {
         if(input[i].freq_error > 0)
            input.erase(input.begin()+i);
      }
      return findlownum(input);
   }
}

VCO_config CDCM6208_panelgui::SolveFreqPlanSingleCDCM(Frequency_plan Frequency_plan, double VCOmin, double VCOmax)
{
   // make local variables
   double l_Y0Y1       = Frequency_plan.Y0Y1_Frequency;
   double l_Y2Y3       = Frequency_plan.Y2Y3_Frequency;
   double l_Y4         = Frequency_plan.Y4_Frequency;
   double l_Y5         = Frequency_plan.Y5_Frequency;
   double l_Y6         = Frequency_plan.Y6_Frequency;
   double l_Y7         = Frequency_plan.Y7_Frequency;
   double l_Input_freq = Frequency_plan.Input_Frequency;
   double l_VCOMin     = VCOmin;
   double l_VCOMax     = VCOmax;
   // Eliminate fractional parts by shifting left (if any)
   int8_t shifts=0;
   while(!(isInteger(l_Y0Y1)&&isInteger(l_Y2Y3)&&isInteger(l_VCOMin)&&isInteger(l_VCOMax)&&isInteger(l_Input_freq)))
   {
      shifts++;
      l_Y0Y1      *=10;
      l_Y2Y3      *=10;
      l_VCOMin    *=10;
      l_VCOMax    *=10;
      l_Input_freq*=10;
   }
   //Find lowest common multiple
   double int_gcd;
   double int_lcm;
   bool do_vco_calc=true;
   if ((Frequency_plan.int_care_mask&3) == 1)
   {
      int_lcm=l_Y0Y1;
   }
   else if ((Frequency_plan.int_care_mask&3) == 2)
   {
      int_lcm=l_Y2Y3;
   }
   else if ((Frequency_plan.int_care_mask&3) == 3)
   {
      int_lcm=(l_Y0Y1*l_Y2Y3)/findGCD((uint64_t)l_Y0Y1,(uint64_t)l_Y2Y3);
   }
   else
   {
      do_vco_calc=false;
   }
   if(do_vco_calc)
   {
      // find number of valid vco freqs for each prescaler
      std::vector<VCO_config> Config_vector;
      Config_vector = FindValidVCOFreqs(int_lcm, l_VCOMin, l_VCOMax);

      int have_error=0;
      double Frequency;
      for(int i = 0; i<Config_vector.size(); i++)
      {
         Frequency  = Config_vector[i].Frequency;
         Config_vector[i].freq_error=dec2frac(Frequency/l_Input_freq,&(Config_vector[i].Multiplier),&(Config_vector[i].Divisor));
         if(Config_vector[i].freq_error>0)
            have_error++;
      }


      if(Config_vector.size() > 0)
      {
         // find index of best config
         int best_index = findbestconfig(Config_vector,have_error);
         return Config_vector[best_index];
      }
      else
      {
         //return empty struct if no config found
         VCO_config emptystruct;
         emptystruct.valid = false;
         return emptystruct;
      }
   }
   else
   {
      //return empty struct if no config found
      //no algorithm for fractional only frequency planning
      VCO_config emptystruct;
      emptystruct.valid = false;
      return emptystruct;
   }
}
