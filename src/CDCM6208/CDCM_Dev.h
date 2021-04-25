#ifndef CDCM_DEV_H
#define CDCM_DEV_H
#include "LimeSuiteConfig.h"
#include "FPGA_common.h"

namespace lime {

#define CDCM_VCO_MIN_V1 2390*1e6
#define CDCM_VCO_MAX_V1 2550*1e6
#define CDCM_VCO_MIN_V2 2940*1e6
#define CDCM_VCO_MAX_V2 3130*1e6

#define CDCM1_BASE_ADDR 0x120
#define CDCM2_BASE_ADDR 0x140

// These structures might be private and outputs are accessed through enums
struct CDCM_VCO
{
    bool valid;             // Is this configuration valid
    double prim_freq;
    double sec_freq;
    int input_mux;
    
    double output_frequency;// Output frequency of VCO
    double freq_err;        // Probably remove it in the future
    
    int R_div;  // Divider of primary frequency
    int M_div;  // Divider of input frequency(either primary or secondary)
    
    int N_mul_full; // Multiplier of 
    int N_mul_10bit;
    int N_mul_8bit;
    
    int prescaler_A; // Prescaler A value. A and B are kept the same
    int prescaler_B; // Prescaler B value. A and B are kept the same
};

struct CDCM_Output
{
    bool isFrac;
    bool used;
    
    double divider_val; 
    
    int integer_part;
    int fractional_part;
    int prescaler;

    double output_freq;
};

struct CDCM_Outputs
{
    CDCM_Output Y0Y1 = {false, false, 0, 0, 0, 0}; // integer LMS2_DAC1 LMS2_DAC2
    CDCM_Output Y2Y3 = {false, false, 0, 0, 0, 0}; // integer LMS2_DAC1 LMS2_DAC2 (unused)
    CDCM_Output Y4 = {false, false, 0, 0, 0, 0};  // fractional LMS2_ADC1
    CDCM_Output Y5 = {false, false, 0, 0, 0, 0};  // fractional LMS2_ADC2
    CDCM_Output Y6 = {false, false, 0, 0, 0, 0};  // fractional LMS3_ADC1
    CDCM_Output Y7 = {false, false, 0, 0, 0, 0};  // fractional LMS3_ADC2
};



// TODO: decentNamingConvention or DecentNamingConvention
class LIME_API CDCM_Dev
{
public:
    CDCM_Dev(FPGA* fpga, uint16_t SPI_BASE_ADDR);
    int setFrequency(CDCM_Output *output);
    double getFrequency(CDCM_Output output);
    void setVCO(CDCM_VCO VCO);
    CDCM_VCO getVCO();
    int uploadConfiguration();
    int downloadConfiguration();
    int getLockStatus();
private:
    int SolveN(int Target, int* Mult8bit, int* Mult10bit);
    void calculateFracDiv(CDCM_Output *Output);
    double SolveFracDiv(double target, CDCM_Output *Output);
    double dec2frac(double target, int* num, int* den);
    bool isInteger(double var) {return var==(int64_t)var;}
    std::vector<CDCM_VCO> FindValidVCOFreqs(double lcm, double VCOmin, double VCOmax);
    uint64_t findGCD(uint64_t a, uint64_t b);
    int GetLowestFreqErr(std::vector<CDCM_VCO> input);
    int FindLowestPSAOutput(std::vector<CDCM_VCO> input);
    int FindBestVCOConfig(std::vector<CDCM_VCO> &input, int num_errors);
    CDCM_VCO FindVCOConfig(CDCM_Outputs Outputs, double VCOmin, double VCOmax);
    int prepareToReadRegs();
    
    // Might be possible to just use conntection.
    FPGA *fpga;
    CDCM_VCO VCO;
    CDCM_Outputs Outputs;
    uint16_t SPI_BASE_ADDR;
    bool is_locked;
    int CDCM_Ver;
};

}
#endif // CDCM_DEV_H
