#include "CDCM6208_panelgui.h"

CDCM6208_panelgui::CDCM6208_panelgui( wxWindow* parent )
:
CDCM_controlpanel( parent )
{

}

CDCM6208_panelgui::   CDCM6208_panelgui( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
:
CDCM_controlpanel(parent, id, pos, size, style), lmsControl(nullptr)
{

}

void CDCM6208_panelgui::Initialize(lms_device_t* pModule)
{
   lmsControl = pModule;
   UpdateGUI();
   m_Baseaddr->SetValue(wxString::Format(_("%i"),SPI_BASEADDR));
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
         int max_val = (1<<10) * (1<<8);
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
         Y7_Divider = d_val;
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
   auto obj = event.GetEventObject();
   // TODO: add solver function here

   // Display newly calculated values in GUI
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

void CDCM6208_panelgui::SolveN(int* Target, int* Mult8bit, int* Mult10bit)
{
   //find multiplier combination to get desired ratio
   double res;
   for(int i8 = 1; i8 <= 256; i8++)
   {
      res = double(*Target)/i8;
      if(res < (1<<10)) //check max value
      {  // check if res is integer
         if(res-(int)res == 0)
         {
            *Mult8bit  = i8;
            *Mult10bit = (int)res;
            return;
         }
      }
   }
}

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
   double max_fractional_val = ((1<<28) + ((1<<20)-1))/(double)(1<<20);
   int max_integer_val = 1<<8 * 3;
   //Check bounds
   if(*result < 1)
      *result = 1;
   // If desired divider is higher than max fractional value,
   // attempt to find closest integer divider
   else if(*result > max_fractional_val)
   {
      if(*result > max_integer_val)
         *result = max_integer_val;

      config->fractional = false;

      // Find non fractional config
      double div2_result;
      double div3_result;
      // in non fractional mode output must be divided
      // by 2 or 3
      div2_result = *result / 2;
      div3_result = *result / 3;
      // check bounds
      if(div2_result > (1<<8))
         div2_result = 1<<8;
      else if(div2_result < 1)
         div2_result = 1;

      if(div3_result > (1<<8))
         div3_result = 1<<8;
      else if(div3_result < 1)
         div3_result = 1;
      // find best match
      double div2_difference = abs(*result - round(div2_result) * 2.0);
      double div3_difference = abs(*result - round(div3_result) * 3.0);

      if(div2_difference < div3_difference)
      {
         config->prescaler = 2;
         config->integer_part = (int)round(div2_result);
         *result = (int)round(div2_result)*2;
      }
      else
      {
         config->prescaler = 3;
         config->integer_part = (int)round(div3_result);
         *result = (int)round(div3_result)*3;
      }
      return;
   }

   // If divider is in bounds for fractional mode
   // solve for fractional divider
   config->fractional = true;
   // Find fractional config
   int placeholder_int;
   placeholder_int = (int)(*result * (1<<20));
   *result = (float)placeholder_int / (1<<20);
   config->integer_part = (placeholder_int >> 20) & ((1<<8) - 1);
   config->fractional_part = placeholder_int & ((1<<20) - 1);
}

void CDCM6208_panelgui::CalculateFracDiv(double* target, Fractional_config* config)
{
   if(config->fractional)
   {
      *target = config->fractional_part;
      *target = *target / (1<<20);
      *target = *target + config->integer_part;
   }
   else
   {
      *target = config->integer_part * config->prescaler;
   }
}
