#ifndef LIMESDR_5G_H
#define	LIMESDR_5G_H
#include "lms7_device.h"
#include "CDCM6208/CDCM6208_Dev.h"
namespace lime

{

class LMS7_LimeSDR_5GRadio : public LMS7_Device
{
public:
    LMS7_LimeSDR_5GRadio(lime::IConnection* conn, LMS7_Device *obj = nullptr);
    int Init() override;
    unsigned GetNumChannels(const bool tx = false) const override;
    std::vector<std::string> GetPathNames(bool dir_tx, unsigned chan) const override;
    int SetPath(bool tx, unsigned chan, unsigned path) override;
    int SetRate(double f_Hz, int oversample) override;
    int SetRate(bool tx, double f_Hz, unsigned oversample) override;
    int SetRate(unsigned ch, double rxRate, double txRate, unsigned oversample = 0) override;
    double GetRate(bool tx, unsigned chan, double *rf_rate_Hz = NULL) const override;
    CDCM_Dev* GetCDCMDev(int index) const override;
private:
    CDCM_Dev* cdcm[2];
};
    
}

#endif	/* LIMESDR_5G_H */

