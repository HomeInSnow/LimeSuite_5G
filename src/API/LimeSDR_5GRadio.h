/*
 * File:   LimeSDR_5G.h
 * Author: Ricardas G
 *
 * Created on April 22, 2021
 */

#ifndef LIMESDR_5G_H
#define	LIMESDR_5G_H
#include "lms7_device.h"

namespace lime

{

class LMS7_LimeSDR_5GRadio : public LMS7_Device
{
public:
    LMS7_LimeSDR_5GRadio(lime::IConnection* conn, LMS7_Device *obj = nullptr);
    unsigned GetNumChannels(const bool tx = false) const override;
    int EnableChannel(bool dir_tx, unsigned chan, bool enabled) override;
    int SetRate(double f_Hz, int oversample) override;
    int SetRate(bool tx, double f_Hz, unsigned oversample) override;
    int SetRate(unsigned ch, double rxRate, double txRate, unsigned oversample = 0) override;
    double GetRate(bool tx, unsigned chan, double *rf_rate_Hz = NULL) const override;
private:
};

}

#endif	/* LIMESDR_5G_H */

