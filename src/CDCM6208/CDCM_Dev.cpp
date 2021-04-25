#include "CDCM_Dev.h"
#include <string>
#include <unordered_map>
#include <chrono>

namespace lime {

CDCM_Dev::CDCM_Dev(FPGA* fpga, uint16_t SPI_BASE_ADDR):
    fpga(fpga),
    SPI_BASE_ADDR(SPI_BASE_ADDR)
{
}

CDCM_VCO CDCM_Dev::getVCO() {return VCO;}

void CDCM_Dev::setVCO(CDCM_VCO VCO)
{
    this->VCO = VCO;
}

int CDCM_Dev::uploadConfiguration()
{
    struct regVal
    {
        uint16_t addr;
        uint16_t val;
    };
    
    std::unordered_map<int, regVal> CDCM_Regs = {
        {1, {(uint16_t)(SPI_BASE_ADDR+1), 0}},
        {2, {(uint16_t)(SPI_BASE_ADDR+2), 0}},
        {3, {(uint16_t)(SPI_BASE_ADDR+3), 0}},
        {5, {(uint16_t)(SPI_BASE_ADDR+5), 0}},
        {6, {(uint16_t)(SPI_BASE_ADDR+6), 0}},
        {8, {(uint16_t)(SPI_BASE_ADDR+8), 0}},
        {9, {(uint16_t)(SPI_BASE_ADDR+9), 0}},
        {10, {(uint16_t)(SPI_BASE_ADDR+10), 0}},
        {11, {(uint16_t)(SPI_BASE_ADDR+11), 0}},
        {12, {(uint16_t)(SPI_BASE_ADDR+12), 0}},
        {13, {(uint16_t)(SPI_BASE_ADDR+13), 0}},
        {14, {(uint16_t)(SPI_BASE_ADDR+14), 0}},
        {15, {(uint16_t)(SPI_BASE_ADDR+15), 0}},
        {16, {(uint16_t)(SPI_BASE_ADDR+16), 0}},
        {17, {(uint16_t)(SPI_BASE_ADDR+17), 0}},
        {18, {(uint16_t)(SPI_BASE_ADDR+18), 0}},
        {19, {(uint16_t)(SPI_BASE_ADDR+19), 0}},
        {20, {(uint16_t)(SPI_BASE_ADDR+20), 0}},
        {21, {(uint16_t)(SPI_BASE_ADDR+23), 0}}
    };
    
    for(int i = 3; i < 21; i+=3) {
        int reg = i == 6 ? 4 : i; // read 4th register instead of 6th
        int val = fpga->ReadRegister(CDCM_Regs[reg].addr);
        if(val == -1) return -1;
        CDCM_Regs[reg].val = val;
    }

    CDCM_Regs[1].val = ((VCO.M_div-1) << 2) | ((VCO.N_mul_10bit-1) >>8);
    CDCM_Regs[2].val = ((VCO.N_mul_10bit-1)<<8) | (VCO.N_mul_8bit-1);
    
    CDCM_Regs[3].val &= ~0xF;
    CDCM_Regs[3].val |= ((VCO.prescaler_A-4)&3);
    CDCM_Regs[3].val |= ((VCO.prescaler_B-4)&3)<<2;
    
    CDCM_Regs[4].val &= ~(0x1F00);
    CDCM_Regs[4].val |= ((VCO.R_div-1)&0xF)<<8;
    CDCM_Regs[4].val |= ((VCO.input_mux-1)&1) <<12;    
    
    CDCM_Regs[6].val = 0 | ((uint16_t)(Outputs.Y0Y1.divider_val-1)&0xFF);
    
    CDCM_Regs[8].val = 0 | ((uint16_t)(Outputs.Y2Y3.divider_val-1)&0xFF);
    
    CDCM_Regs[9].val &= ~0x1E00;
    CDCM_Regs[9].val |= (((int)Outputs.Y4.isFrac)<<9);
    CDCM_Regs[9].val |= ((Outputs.Y4.prescaler-2)&7)<<10;
    
    CDCM_Regs[10].val = 0 | (((Outputs.Y4.integer_part-1)&0xFF)<<4);
    CDCM_Regs[10].val |= (Outputs.Y4.fractional_part>>16)&0xF;

    CDCM_Regs[11].val = Outputs.Y4.fractional_part&0xFFFF;

    CDCM_Regs[12].val &= ~0x1E00;
    CDCM_Regs[12].val |= (((int)Outputs.Y5.isFrac)<<9);
    CDCM_Regs[12].val |= ((Outputs.Y5.prescaler-2)&7)<<10;
    
    CDCM_Regs[13].val = 0 | (((Outputs.Y5.integer_part-1)&0xFF)<<4);
    CDCM_Regs[13].val |= (Outputs.Y5.fractional_part>>16)&0xF;

    CDCM_Regs[14].val = Outputs.Y5.fractional_part&0xFFFF;

    CDCM_Regs[15].val &= ~0x1E00;
    CDCM_Regs[15].val |= (((int)Outputs.Y6.isFrac)<<9);
    CDCM_Regs[15].val |= ((Outputs.Y6.prescaler-2)&7)<<10;
    
    CDCM_Regs[16].val = 0 | (((Outputs.Y6.integer_part-1)&0xFF)<<4);
    CDCM_Regs[16].val |= (Outputs.Y6.fractional_part>>16)&0xF;

    CDCM_Regs[17].val = Outputs.Y6.fractional_part&0xFFFF;

    CDCM_Regs[18].val &= ~0x1E00;
    CDCM_Regs[18].val |= (((int)Outputs.Y7.isFrac)<<9);
    CDCM_Regs[18].val |= ((Outputs.Y7.prescaler-2)&7)<<10;

    CDCM_Regs[19].val = 0 | (((Outputs.Y7.integer_part-1)&0xFF)<<4);
    CDCM_Regs[19].val |= (Outputs.Y7.fractional_part>>16)&0xF;

    CDCM_Regs[20].val = Outputs.Y7.fractional_part&0xFFFF;

    CDCM_Regs[21].val = 1;

    for(auto reg : CDCM_Regs)
        if(fpga->WriteRegister(reg.second.addr, reg.second.val) != 0)
            return -1;

    return getLockStatus();
}

int CDCM_Dev::downloadConfiguration()
{
    struct regVal
    {
        uint16_t addr;
        uint16_t val;
    };
    
    std::unordered_map<int, regVal> CDCM_Regs = {
        {1, {(uint16_t)(SPI_BASE_ADDR+1), 0}},
        {2, {(uint16_t)(SPI_BASE_ADDR+2), 0}},
        {3, {(uint16_t)(SPI_BASE_ADDR+3), 0}},
        {5, {(uint16_t)(SPI_BASE_ADDR+5), 0}},
        {6, {(uint16_t)(SPI_BASE_ADDR+6), 0}},
        {8, {(uint16_t)(SPI_BASE_ADDR+8), 0}},
        {9, {(uint16_t)(SPI_BASE_ADDR+9), 0}},
        {10, {(uint16_t)(SPI_BASE_ADDR+10), 0}},
        {11, {(uint16_t)(SPI_BASE_ADDR+11), 0}},
        {12, {(uint16_t)(SPI_BASE_ADDR+12), 0}},
        {13, {(uint16_t)(SPI_BASE_ADDR+13), 0}},
        {14, {(uint16_t)(SPI_BASE_ADDR+14), 0}},
        {15, {(uint16_t)(SPI_BASE_ADDR+15), 0}},
        {16, {(uint16_t)(SPI_BASE_ADDR+16), 0}},
        {17, {(uint16_t)(SPI_BASE_ADDR+17), 0}},
        {18, {(uint16_t)(SPI_BASE_ADDR+18), 0}},
        {19, {(uint16_t)(SPI_BASE_ADDR+19), 0}},
        {20, {(uint16_t)(SPI_BASE_ADDR+20), 0}},
        {23, {(uint16_t)(SPI_BASE_ADDR+23), 0}}
    };

    
    // Ask FPGA to get CDCM reg valuas
    if(prepareToReadRegs() != 0)
        return -1;
    
    for(auto &reg : CDCM_Regs)
    {
        int val = fpga->ReadRegister(reg.second.addr);
        if(val == -1) return -1;
        else reg.second.val = val;
    }

    VCO.M_div = (CDCM_Regs[1].val >> 2) + 1; 
    
    VCO.N_mul_10bit = 0 | ((CDCM_Regs[1].val & 3)<<8);     
    VCO.N_mul_10bit |= (CDCM_Regs[2].val>>8);
    VCO.N_mul_10bit += 1;
    VCO.N_mul_8bit = (CDCM_Regs[2].val & 0xFF) + 1;
    
    VCO.prescaler_A = (CDCM_Regs[3].val & 3)+4;
    VCO.prescaler_B = ((CDCM_Regs[3].val >>2)&3)+4;
    
    VCO.R_div = ((CDCM_Regs[4].val >> 8) & 0xF)+1;

    VCO.input_mux = ((CDCM_Regs[4].val >> 12)&1)+1;
    
    Outputs.Y0Y1.divider_val = (CDCM_Regs[6].val&0xFF)+1;
    Outputs.Y2Y3.divider_val = (CDCM_Regs[8].val&0xFF)+1;
    
    Outputs.Y4.isFrac = (bool)((CDCM_Regs[9].val>>9)&1);
    Outputs.Y4.prescaler = ((CDCM_Regs[9].val>>10)&7)+2;
    Outputs.Y4.integer_part = ((CDCM_Regs[10].val>>4)&0xFF)+1;
    Outputs.Y4.fractional_part = 0 | ((CDCM_Regs[10].val&0xF)<<16); // TODO: figure out whether this zero is needed
    Outputs.Y4.fractional_part |= CDCM_Regs[11].val;
    calculateFracDiv(&Outputs.Y4);
    
    Outputs.Y5.isFrac = (bool)((CDCM_Regs[12].val>>9)&1);
    Outputs.Y5.prescaler = ((CDCM_Regs[12].val>>10)&7)+2;
    Outputs.Y5.integer_part = ((CDCM_Regs[13].val>>4)&0xFF)+1;
    Outputs.Y5.fractional_part = 0 | ((CDCM_Regs[13].val&0xF)<<16);
    Outputs.Y5.fractional_part |= CDCM_Regs[14].val;
    calculateFracDiv(&Outputs.Y5);

    Outputs.Y6.isFrac = (bool)((CDCM_Regs[15].val>>9)&1);
    Outputs.Y6.prescaler = ((CDCM_Regs[15].val>>10)&7)+2;
    Outputs.Y6.integer_part = ((CDCM_Regs[16].val>>4)&0xFF)+1;
    Outputs.Y6.fractional_part = 0 | ((CDCM_Regs[16].val&0xF)<<16);
    Outputs.Y6.fractional_part |= CDCM_Regs[17].val;
    calculateFracDiv(&Outputs.Y6);

    Outputs.Y7.isFrac = (bool)((CDCM_Regs[18].val>>9)&1);
    Outputs.Y7.prescaler = ((CDCM_Regs[18].val>>10)&7)+2;
    Outputs.Y7.integer_part = ((CDCM_Regs[19].val>>4)&0xFF)+1;
    Outputs.Y7.fractional_part = 0 | ((CDCM_Regs[19].val&0xF)<<16);
    Outputs.Y7.fractional_part |= CDCM_Regs[20].val;
    calculateFracDiv(&Outputs.Y7);

    CDCM_Ver = (CDCM_Regs[23].val>>3)&7;
    
    return getLockStatus();
}

double CDCM_Dev::SolveFracDiv(double target, CDCM_Output *Output)
{
   //Calculate max fractional val (integer value is offset by +1)
   //8 bits integer value, 20 bits fractional value
   // 1111 1111  1111 1111 1111 1111 1111
   // \  int  /  \ fractional           /
   //    +1
   // 1 0000 0000 1111 1111 1111 1111 1111
   // \  int    / \ fractional           /
   
   double max_fractional_val = (((1<<28) + ((1<<20)-1))/(double)(1<<20))*3;

   double result = target;
   //Check bounds
   if(result < 1)
      result = 1;
   else if(result > max_fractional_val)
      result = max_fractional_val;
   
   Output->isFrac = true; // This seems highly unecessary
 
   // Divider must be multiplied by 2 or 3
   double div2_result;
   double div3_result;
   int div2_fixed;
   int div3_fixed;
   div2_result = result / 2;
   div3_result = result / 3;

   //cast float into 8.20 fixed point integer
   div2_fixed = (int)(div2_result * ((1<<20)&((1<<28)-1)));
   //recast back
   div2_result = (float)div2_fixed / (1<<20);
   //cast float into 8.20 fixed point integer
   div3_fixed = (int)(div3_result * ((1<<20)&((1<<28)-1)));
   //recast back
   div3_result = (float)div3_fixed / (1<<20);
   // find best match
   double div2_difference = abs(result - (div2_result * 2.0));
   double div3_difference = abs(result - (div3_result * 3.0));
   if(div2_difference < div3_difference)
   {
      Output->integer_part = (div2_fixed >> 20) & ((1<<8) - 1);
      Output->fractional_part = div2_fixed & ((1<<20) - 1);
      Output->prescaler = 2;
      result = div2_result*2;
   }
   else
   {
      Output->integer_part = (div3_fixed >> 20) & ((1<<8) - 1);
      Output->fractional_part = div3_fixed & ((1<<20) - 1);
      Output->prescaler = 3;
      result = div3_result*3;
   }
   
   return result;
}

int CDCM_Dev::SolveN(int Target, int* Mult8bit, int* Mult10bit)
{
   //find multiplier combination to get desired ratio
   double res;
   for(int i10 = 1; i10 <= 1024; i10++)
   {
      res = double(Target)/i10;
      if(res < (1<<8)) //check max value
      {  
         // check if res is integer
         if(isInteger(res))
         {
            *Mult8bit  = (int)res;
            *Mult10bit = i10;
            return 0;
         }
      }
   }
   return -1;
}

void CDCM_Dev::calculateFracDiv(CDCM_Output *Output)
{
    if(Output->isFrac)
    {
        Output->divider_val = Output->fractional_part;
        Output->divider_val = Output->divider_val / (1<<20);
        Output->divider_val = Output->divider_val + Output->integer_part;
        Output->divider_val = Output->divider_val * Output->prescaler;
    }
    else
    {
        Output->divider_val = Output->integer_part;
    }
}

int CDCM_Dev::getLockStatus()
{
    if(prepareToReadRegs() != 0)
        return -1;

    // Lock status (21 CDCM register is mapped to 22 FPGA register)
    return !(fpga->ReadRegister(SPI_BASE_ADDR+22)>>2)&1;
}

int CDCM_Dev::prepareToReadRegs()
{
    const auto timeout = std::chrono::seconds(2); //TODO: ask Servenikas for timeout value
    uint16_t status = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto t2 = t1;
    fpga->WriteRegister(SPI_BASE_ADDR+24, 1);
    do
    {
        status = fpga->ReadRegister(SPI_BASE_ADDR+24);
        t2 = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } while (status != 2 && (t2-t1) < timeout);
    
    if(t2 - t1 > timeout)
        return -1;
    
    return 0;
}

double CDCM_Dev::dec2frac(double target, int* num, int* den)
{
   //constant for CDCM6208
   const int max_num_bits = 18;
   const int max_den_bits = 14;
   uint64_t l_num = *num;
   uint64_t l_den = *den;
   double l_target = target;
   l_den = 1;
   while(isInteger(target)==false)
   {
      l_den  *=10;
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
   while(SolveN(l_num,&placeholder,&placeholder) == 1)
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

std::vector<CDCM_VCO> CDCM_Dev::FindValidVCOFreqs(double lcm, double VCOmin, double VCOmax)
{
   // find number of valid vco freqs for each prescaler
   std::vector<CDCM_VCO> Config_vector;
   CDCM_VCO placeholder_struct;
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
            placeholder_struct.prescaler_A = prescaler;
            placeholder_struct.prescaler_B = prescaler;
            placeholder_struct.output_frequency = frequency;
            Config_vector.push_back(placeholder_struct);
            frequency += lcm;
      }
   }
   return Config_vector;
}

//Euclidean method for finding greatest common divisor
uint64_t CDCM_Dev::findGCD(uint64_t a, uint64_t b)
{
   if (b == 0)
      return a;
   return findGCD(b, a % b);
}

//TODO: N_mul_full should be done
//Find lowest multiplier index?
int CDCM_Dev::FindLowestPSAOutput(std::vector<CDCM_VCO> input)
{
   int min_nom = 0xFFFFFFF;
   int index = 0;
   int curr_val;
   for(int i=0; i<input.size(); i++)
   {
      curr_val = input[i].N_mul_full * input[i].prescaler_A;
      if(curr_val < min_nom)
      {
         min_nom = curr_val;
         index = i;
      }
   }
   return index;
}

//TODO make this not use VCO, instead hagve a vector of freq errors
int CDCM_Dev::GetLowestFreqErr(std::vector<CDCM_VCO> input)
{
   double min_err = 100e6;
   int index = 0;
   for(int i=0; i<input.size(); i++)
   {
      if(input[i].freq_err < min_err)
      {
         min_err = input[i].freq_err;
         index = i;
      }
   }
   return index;

}

//TODO: really not a fan of input erasure here.
//Might actually delete this whole
int CDCM_Dev::FindBestVCOConfig(std::vector<CDCM_VCO> &input, int num_errors)
{
   int no_error = input.size()-num_errors;
   // all configs have freq. errors
   if(no_error==0)
   {
      return GetLowestFreqErr(input);
   }
   // no configs have freq. errors
   else if(num_errors==0)
   {
      return FindLowestPSAOutput(input);
   }
   else
   {
      for(int i=(input.size()-1);i>=0;i--)
      {
         if(input[i].freq_err > 0)
            input.erase(input.begin()+i); // TODO:check over this
      }
      return FindLowestPSAOutput(input);
   }
}

CDCM_VCO CDCM_Dev::FindVCOConfig(CDCM_Outputs Outputs, double VCOmin, double VCOmax)
{
   // make local variables
   double l_Y0Y1       = Outputs.Y0Y1.output_freq;
   double l_Y2Y3       = Outputs.Y2Y3.output_freq;
   double l_Y4         = Outputs.Y4.output_freq;
   double l_Y5         = Outputs.Y5.output_freq;
   double l_Y6         = Outputs.Y6.output_freq;
   double l_Y7         = Outputs.Y7.output_freq;
   double l_Input_freq = VCO.output_frequency;
   double l_VCOMin     = VCOmin;
   double l_VCOMax     = VCOmax;
   
   // Eliminate fractional parts by shifting left (if any)
   while(!(isInteger(l_Y0Y1)&&isInteger(l_Y2Y3)&&isInteger(l_VCOMin)&&isInteger(l_VCOMax)&&isInteger(l_Input_freq)))
   {
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

   if(Outputs.Y0Y1.used && Outputs.Y2Y3.used)
      int_lcm=(l_Y0Y1*l_Y2Y3)/findGCD((uint64_t)l_Y0Y1,(uint64_t)l_Y2Y3);
   else if(Outputs.Y0Y1.used)
      int_lcm=l_Y0Y1;
   else if (Outputs.Y2Y3.used)
      int_lcm=l_Y2Y3;
   else
      do_vco_calc=false;
   
   if(do_vco_calc)
   {
      // find number of valid vco freqs for each prescaler
      std::vector<CDCM_VCO> Config_vector;
      Config_vector = FindValidVCOFreqs(int_lcm, l_VCOMin, l_VCOMax);

      int have_error=0;
      double Frequency;
      for(int i = 0; i<Config_vector.size(); i++)
      {
         Frequency  = Config_vector[i].output_frequency;
         Config_vector[i].freq_err=dec2frac(Frequency/l_Input_freq,&(Config_vector[i].N_mul_full),&(Config_vector[i].M_div));
         if(Config_vector[i].freq_err>0)
            have_error++;
      }


      if(Config_vector.size() > 0)
      {
         // find index of best config
         int best_index = FindBestVCOConfig(Config_vector,have_error);
         return Config_vector[best_index];
      }
      else
      {
         //return empty struct if no config found
         CDCM_VCO emptystruct;
         emptystruct.valid = false;
         return emptystruct;
      }
   }
   else
   {
      //return empty struct if no config found
      //no algorithm for fractional only frequency planning
      CDCM_VCO emptystruct;
      emptystruct.valid = false;
      return emptystruct;
   }
}

}
