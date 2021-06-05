#include "LimeSDR_5GRadio.h"

#include "FPGA_5G.h"
#include "Logger.h"
#include "device_constants.h"

namespace lime
{

LMS7_LimeSDR_5GRadio::LMS7_LimeSDR_5GRadio(lime::IConnection* conn, LMS7_Device *obj) : LMS7_Device(obj)
{
    fpga = new lime::FPGA_5G();
    cdcm[0] = new CDCM_Dev(fpga, CDCM1_BASE_ADDR);
    cdcm[1] = new CDCM_Dev(fpga, CDCM2_BASE_ADDR);

    tx_channels.resize(GetNumChannels());
    rx_channels.resize(GetNumChannels());

    while (lms_list.size() < 3)
        lms_list.push_back(new lime::LMS7002M());

    fpga->SetConnection(conn);
    cdcm[0]->Init(30.72e6, 30.72e6);
    cdcm[1]->Init(30.72e6, 30.72e6);

    for (unsigned i = 0; i < 3; i++)
    {
        this->lms_list[i]->SetConnection(conn, i);
        lms_list[i]->SetReferenceClk_SX(false, 30.72e6);
        mStreamers.push_back(new lime::Streamer(fpga,lms_list[i],i));
    }
    connection = conn;
}

int LMS7_LimeSDR_5GRadio::Init()
{
    if(cdcm[0]->Reset(30.72e6, 30.72e6) != 0 
    || cdcm[1]->Reset(30.72e6, 30.72e6) != 0)
        return -1;
    return LMS7_Device::Init();
}

unsigned LMS7_LimeSDR_5GRadio::GetNumChannels(const bool tx) const
{
    return 6;
}

std::vector<std::string> LMS7_LimeSDR_5GRadio::GetPathNames(bool dir_tx, unsigned chan) const
{
    if(chan == 0 || chan == 1) // LMS1
    {
        if(dir_tx)
            return {"NONE", "BAND1", "BAND2"};
        else    
            return {"NONE", "LNAH", "LNAL", "LNAW_NC"};
    }

    if(chan == 2 || chan == 3) // LMS2
    {
        return {"NONE", "TDD_TX", "TDD_RX", "FDD", "Cal"};
    }
    
    if(chan == 4 || chan == 5) // LMS3
    {
        if(dir_tx)
            return {"NONE"};
        else
            return {"NONE", "LNAH", "Cal"};
    }
    return {};
}

int LMS7_LimeSDR_5GRadio::SetPath(bool tx, unsigned chan, unsigned path)
{
    uint16_t sw_addr = 0x00D1;
    uint16_t pa_addr = 0x00D2;
    uint16_t sw_val = fpga->ReadRegister(sw_addr);
    uint16_t pa_val = fpga->ReadRegister(pa_addr);
    lime::LMS7002M* lms = SelectChannel(chan);

    if(chan == 0 || chan == 1)
    {
        if(tx)
        {
            pa_val |= 1 << (4+chan);

            if(path == LMS_PATH_NONE)
                pa_val &= ~(1 << (4+chan));
            else if (path == LMS_PATH_TX1)
                sw_val |= 1 << (12+chan);
            else if (path == LMS_PATH_TX2)
                sw_val &= ~(1 << (12+chan)); 
            
            fpga->WriteRegister(sw_addr, sw_val);
            fpga->WriteRegister(pa_addr, pa_val);
            return lms->SetBandTRF(path);
        }
        else
        {
            if(path == LMS_PATH_LNAW)
                lime::warning("LNAW has no connection to RF ports");
            else if (path == LMS_PATH_LNAH)
                sw_val |= 1 << (10+chan);
            else if(path == LMS_PATH_LNAL)
                sw_val &= ~(1UL << (10+chan));

            fpga->WriteRegister(sw_addr, sw_val);
            return lms->SetPathRFE(lime::LMS7002M::PathRFE(path));
        }
    }
    if(chan == 2 || chan == 3)
    {
        int tx_path = LMS_PATH_TX1;
        int rx_path = LMS_PATH_LNAH;
        uint16_t shift = chan == 2 ? 0 : 2;
        pa_val |= 1 << (chan == 2 ? 3 : 2);         // enable PA
        pa_val &= ~(1 << (chan == 2 ? 1 : 0));      // enable LNA 
        if (path == 0)
        {
            pa_val &= ~(1 << (chan == 2 ? 3 : 2));  // disable
            pa_val = 1 << (chan == 2 ? 1 : 0);      // disable LNA
            tx_path = LMS_PATH_NONE;
            rx_path = LMS_PATH_NONE;
        }
        else if (path == 1) // TDD_TX
        {
            if(chan == 2)
                sw_val &= ~(1 << 7);                // TRX1T to RSFW_TRX1
            else
                sw_val |= 1 << 9;                   // TRX2T to RSFW_TRX2
            sw_val |= 1 << (6+shift);               // TRX1 or TRX2 to J8 or J10
            sw_val &= ~(1 << (2+shift));            // RX1C or RX2C to RX1IN or RX2IN (LNA)
            sw_val |= 1 << (3+shift);               // RX1IN or RX2IN to RFSW_TRX1 or RFSW_TRX1
        }
        else if (path == 2) // TDD_RX
        {
            if(chan == 2)
                sw_val |= 1 << 9;                   // TRX2T to ground
            else
                sw_val &= ~(1 << 7);                // TRX1T to ground
            sw_val &= ~(1 << (6+shift));            // TRX1 or TRX2 to J8 or J10
            sw_val &= ~(1 << (2+shift));            // RX1C or RX2C to RX1IN or RX2IN (LNA)
            sw_val |= 1 << (3+shift);               // RX1IN or RX2IN to RFSW_TRX1 or RFSW_TRX1
        }
        else if (path == 3) // FDD
        {
            if(chan == 2)
                sw_val &= ~(1 << 7);                // TRX1T to RSFW_TRX1
            else
                sw_val |= 1 << 9;                   // TRX2T to RSFW_TRX2
            sw_val |= 1 << (6+shift);               // TRX1 or TRX2 to J8 or J10
            sw_val &= ~(1 << (2+shift));            // RX1C or RX2C to RX1IN or RX2IN (LNA)
            sw_val &= ~(1 << (3+shift));            // RX1IN or RX2In to J9 or  J11
        }
        else if (path == 4) // Cal
        {
            if(chan == 2)
                sw_val |= 1 << 9;                   // TRX2T to ground
            else
                sw_val &= ~(1 << 7);                // TRX1T to ground
            sw_val |= 1 << (6+shift);               // TRX1 or TRX2 to J8 or J10
            sw_val |= 1 << (2+shift);            // RX1C or RX2C to LMS3 TX1_1 or TX2_1
            sw_val |= 1 << (3+shift);               // RX1IN or RX2IN to RFSW_TRX1 or RFSW_TRX1
            pa_val = 1 << (chan == 2 ? 1 : 0);      // disable LNA
        }

        fpga->WriteRegister(sw_addr, sw_val);
        fpga->WriteRegister(pa_addr, pa_val);
        lms->SetBandTRF(tx_path);
        return lms->SetPathRFE(lime::LMS7002M::PathRFE(rx_path));
    }
    if(chan == 4 || chan == 5)
    {
        if(tx)
        {
            lime::warning("TX has no connection to RF ports");
            return lms->SetBandTRF(path);
        }
        else
        {
            if(path == LMS_PATH_NONE || path > 2)
                return lms->SetPathRFE(lime::LMS7002M::PathRFE(LMS_PATH_NONE));
            else if(path == LMS_PATH_LNAH)
                sw_val &= ~(1 << (chan-4));
            else if (path == 2) // Calibration path
                sw_val |= 1 << (chan-4);

            fpga->WriteRegister(sw_addr, sw_val);
            return lms->SetPathRFE(lime::LMS7002M::PathRFE(LMS_PATH_LNAH));
        }
    }
    return 0;
}

int LMS7_LimeSDR_5GRadio::SetRate(double f_Hz, int oversample)
{
    double nco_f=0;
    for (unsigned i = 0; i < 2; i++)
    {
         if (rx_channels[i].cF_offset_nco > nco_f)
             nco_f = rx_channels[i].cF_offset_nco;
         if (tx_channels[i].cF_offset_nco > nco_f)
             nco_f = tx_channels[i].cF_offset_nco;
         tx_channels[i].sample_rate = f_Hz;
         rx_channels[i].sample_rate = f_Hz;
    }

    if (oversample == 0)
    {
        const int n = lime::cgenMax/(4*f_Hz);
        oversample = (n >= 32) ? 32 : (n >= 16) ? 16 : (n >= 8) ? 8 : (n >= 4) ? 4 : 2;
    }

    if (nco_f != 0)
    {
        int nco_over = 2+2*(nco_f-1)/f_Hz;
        if (nco_over > 32)
        {
            lime::error("Cannot achieve desired sample rate: rate too low");
            return -1;
        }
        oversample = oversample > nco_over ? oversample : nco_over;
    }

    int decim = 4;
    if (oversample <= 16)
    {
        const int decTbl[] = {0, 0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
        decim = decTbl[oversample];
    }

    oversample = 2<<decim;

    lime::LMS7002M* lms = lms_list[0];
    if ((lms->SetFrequencyCGEN(f_Hz*4*oversample) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(EN_ADCCLKH_CLKGN), 0) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(CLKH_OV_CLKL_CGEN), 2) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(MAC), 2) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(HBD_OVR_RXTSP), decim) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(HBI_OVR_TXTSP), decim) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(MAC), 1) != 0)
        || (lms->SetInterfaceFrequency(lms->GetFrequencyCGEN(), decim, decim) != 0))
        return -1;
    lms_chip_id = 0;
    if (SetFPGAInterfaceFreq(decim, decim)!=0)
        return -1;

    for (unsigned i = 0; i < GetNumChannels(); i++)
    {
        if (rx_channels[i].cF_offset_nco != 0)
           SetNCOFreq(false, i, 0, rx_channels[i].cF_offset_nco);

        if (tx_channels[i].cF_offset_nco != 0)
           SetNCOFreq(true, i, 0, -tx_channels[i].cF_offset_nco);
        auto gfir_bw = tx_channels[i].gfir_bw;
        if (gfir_bw > 0)
            ConfigureGFIR(true, i, true, gfir_bw);
        gfir_bw = rx_channels[i].gfir_bw;
        if (gfir_bw > 0)
            ConfigureGFIR(false, i, true, gfir_bw);
    }

    return 0;
}

int LMS7_LimeSDR_5GRadio::SetRate(bool tx, double f_Hz, unsigned oversample)
{
    double tx_clock;
    double rx_clock;
    double cgen;

    int decimation;
    int interpolation;

    double nco_rx=0;
    double nco_tx=0;
    int min_int = 1;
    int min_dec = 1;
    bool retain_nco = false;

    lime::LMS7002M* lms = lms_list[0];

    for (unsigned i = 0; i < 2; i++)
    {
        if (rx_channels[i].cF_offset_nco > nco_rx)
            nco_rx = rx_channels[i].cF_offset_nco;
        if (tx_channels[i].cF_offset_nco > nco_tx)
            nco_tx = tx_channels[i].cF_offset_nco;
        if (tx)
            tx_channels[i].sample_rate = f_Hz;
        else
            rx_channels[i].sample_rate = f_Hz;
    }

    if (oversample == 0)
    {
        int n = tx ? lime::cgenMax/f_Hz : lime::cgenMax/(4*f_Hz);
        oversample = (n >= 32) ? 32 : (n >= 16) ? 16 : (n >= 8) ? 8 : (n >= 4) ? 4 : 2;
    }

    if (nco_rx != 0 || nco_tx != 0)
    {
        retain_nco = true;
        min_int = 2+2*(nco_tx-1)/tx_channels[0].sample_rate;
        min_dec = 2+2*(nco_rx-1)/rx_channels[0].sample_rate;
        unsigned int nco_over = tx ? min_int : min_dec;
        if (nco_over > 32)
        {
            lime::ReportError(ERANGE, "Cannot achieve desired sample rate: rate too low");
            return -1;
        }
        oversample = oversample > nco_over ? oversample : nco_over;
    }

    int tmp = 4;
    if (oversample <= 16)
    {
        const int decTbl[] = {0, 0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
        tmp = decTbl[oversample];
    }

    int ratio = 2<<tmp;

    if (tx)
    {
        interpolation = tmp;
        decimation = lms->Get_SPI_Reg_bits(LMS7param(HBD_OVR_RXTSP));
        rx_clock = lms->GetReferenceClk_TSP(lime::LMS7002M::Rx);
        tx_clock = f_Hz*ratio;
    }
    else
    {
        decimation = tmp;
        interpolation = lms->Get_SPI_Reg_bits(LMS7param(HBI_OVR_TXTSP));
        tx_clock = lms->GetReferenceClk_TSP(lime::LMS7002M::Tx);
        rx_clock = f_Hz * ratio;
    }

    int div_index = floor(log2(tx_clock/rx_clock)+0.5);

    while (div_index < -1)
    {
        if (tx)
        {
           if ((decimation > 0) && (min_dec <= (1<<decimation)))
           {
             decimation--;
             div_index++;
           }
           else if (interpolation < 4)
           {
             interpolation++;
             div_index++;
           }
           else
           {
               div_index = -1;
               break;
           }
        }
        else
        {
           if (interpolation < 4)
           {
             interpolation++;
             div_index++;
           }
           else if ((decimation > 0) && (min_dec <= (1<<decimation)))
           {
             decimation--;
             div_index++;
           }
           else
           {
               div_index = -1;
               break;
           }
        }
    }

    while (div_index > 5)
    {
        if (tx)
        {
           if (decimation < 4)
           {
             decimation++;
             div_index--;
           }
           else if ((interpolation > 0) && (min_int <= (1<<interpolation)))
           {
             interpolation--;
             div_index--;
           }
           else
           {
             div_index = 5;
             break;
           }
        }
        else
        {
           if ((interpolation > 0) && (min_int <= (1<<interpolation)))
           {
             interpolation--;
             div_index--;
           }
           else if (decimation < 4)
           {
             decimation++;
             div_index--;
           }
           else
           {
               div_index = 5;
               break;
           }
        }
    }

    if (min_int > (2<<interpolation) || min_dec > (2<<decimation))
    {
        lime::ReportError(ERANGE, "Unable to meet NCO oversampling requirements");
        return -1;
    }

    int clk_mux;
    int clk_div;

    switch (div_index)
    {
        case -1://2:1
                clk_mux = 0;
                clk_div = 3;
                break;
        case 0://1:1
                clk_mux = 0;
                clk_div = 2;
                break;
        case 1: //1:2
                clk_mux = 0;
                clk_div = 1;
                break;
        case 2://1:4
                clk_mux = 0;
                clk_div = 0;
                break;
        case 3: //1:8
                clk_mux = 1;
                clk_div = 1;
                break;
        case 4: //1:16
                clk_mux = 1;
                clk_div = 2;
                break;
        case 5: //1:32
                clk_mux = 1;
                clk_div = 3;
                break;
    }

    if (tx)
    {
        ratio = 2<<interpolation;
        cgen = f_Hz*ratio;
    }
    else
    {
        ratio = 2<<decimation;
        cgen = f_Hz * ratio * 4;
    }

    if ((tx && clk_mux == 0)||(tx == false && clk_mux == 1))
    {
        while (cgen*(1<<clk_div)>lime::cgenMax)
        {
            if (clk_div > 0)
            {
                clk_div--;
            }
            else
            {
               lime::ReportError(ERANGE, "Cannot set desired sample rate. CGEN clock out of range");
               return -1;
            }
        }
        cgen *= (1 << clk_div);
    }

    if (cgen > lime::cgenMax)
    {
        lime::ReportError(ERANGE, "Cannot set desired sample rate. CGEN clock out of range");
        return -1;
    }

    lms = lms_list[0];
    if ((lms->SetFrequencyCGEN(cgen, retain_nco) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(EN_ADCCLKH_CLKGN), clk_mux) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(CLKH_OV_CLKL_CGEN), clk_div) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(MAC), 2) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(HBD_OVR_RXTSP), decimation) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(HBI_OVR_TXTSP), interpolation) != 0)
        || (lms->Modify_SPI_Reg_bits(LMS7param(MAC), 1) != 0)
        || (lms->SetInterfaceFrequency(cgen, interpolation, decimation) != 0))
        return -1;
    lms_chip_id = 0;
    if (SetFPGAInterfaceFreq(interpolation, decimation)!=0)
        return -1;

    for (unsigned i = 0; i < GetNumChannels(); i++)
    {
        if (rx_channels[i].cF_offset_nco != 0)
           SetNCOFreq(false, i, 0, rx_channels[i].cF_offset_nco);

        if (tx_channels[i].cF_offset_nco != 0)
           SetNCOFreq(true, i, 0, -tx_channels[i].cF_offset_nco);
        auto gfir_bw = tx ? tx_channels[i].gfir_bw : rx_channels[i].gfir_bw;
        if (gfir_bw > 0)
            ConfigureGFIR(tx, i, true, gfir_bw);
    }

    return 0;
}

int LMS7_LimeSDR_5GRadio::SetRate(unsigned ch, double rxRate, double txRate, unsigned oversample)
{
    if(ch > 3)
    {
        cdcm_output_t rx_output = ch == 4 ?  CDCM_Y6 : CDCM_Y7;
        return cdcm[1]->SetFrequency(rx_output, rxRate, true);
    }
    else if (ch > 1)
    {
        cdcm_output_t rx_output = ch == 2 ?  CDCM_Y4 : CDCM_Y5;
        cdcm_output_t tx_output = CDCM_Y0Y1;
        if(cdcm[1]->SetFrequency(tx_output, txRate, false) != 0)
            return -1;
        return cdcm[1]->SetFrequency(rx_output, rxRate, true);
    }
    return LMS7_Device::SetRate(ch,rxRate,txRate,oversample);
}

double LMS7_LimeSDR_5GRadio::GetRate(bool tx, unsigned chan, double *rf_rate_Hz) const
{
    if(chan > 3)
    {
        if(tx)
            return 0;
        cdcm_output_t rx_output = chan == 4 ?  CDCM_Y6 : CDCM_Y7;
        return cdcm[1]->GetFrequency(rx_output);
    }
    else if (chan > 1)
    {
        cdcm_output_t output;
        if(tx)
            output = CDCM_Y0Y1;
        else
            output = chan == 2 ? CDCM_Y4 : CDCM_Y5;
        return cdcm[1]->GetFrequency(output);
    }
    
    return LMS7_Device::GetRate(tx, chan, rf_rate_Hz);
}

CDCM_Dev* LMS7_LimeSDR_5GRadio::GetCDCMDev(int index) const
{
    if(index == 0)
        return cdcm[0];
    else if(index == 1)
        return cdcm[1];
    else
        return nullptr;
}

} //namespace lime
