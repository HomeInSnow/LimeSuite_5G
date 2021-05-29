#include "FPGA_5G.h"
#include "IConnection.h"
#include "Logger.h"
#include "LMS64CProtocol.h"
#include <ciso646>
#include <vector>
#include <map>
#include <math.h>

namespace lime
{

FPGA_5G::FPGA_5G() : FPGA(){}

int FPGA_5G::SetInterfaceFreq(double txRate_Hz, double rxRate_Hz, double txPhase, double rxPhase, int channel)
{
    if(channel == 1 || channel == 0)
        return 0;

    lime::FPGA::FPGA_PLL_clock clocks[2];

    clocks[0].index = 0;
    clocks[0].outFrequency = rxRate_Hz;
    clocks[1].index = 1;
    clocks[1].outFrequency = rxRate_Hz;
    clocks[1].phaseShift_deg = rxPhase;
    if (SetPllFrequency(1, rxRate_Hz, clocks, 2)!=0)
        return -1;

    clocks[0].index = 0;
    clocks[0].outFrequency = txRate_Hz;
    clocks[1].index = 1;
    clocks[1].outFrequency = txRate_Hz;
    clocks[1].phaseShift_deg = txPhase;
    if (SetPllFrequency(0, txRate_Hz, clocks, 2)!=0)
        return -1;

    return 0;
}

int FPGA_5G::WriteRegisters(const uint32_t *addrs, const uint32_t *data, unsigned cnt)
{
    return connection->WriteRegisters(addrs, data, cnt);
}
int FPGA_5G::ReadRegisters(const uint32_t *addrs, uint32_t *data, unsigned cnt)
{
    return connection->ReadRegisters(addrs, data, cnt);
}

int FPGA_5G::SetInterfaceFreq(double txRate_Hz, double rxRate_Hz, int channel)
{
    if(channel == 1 || channel == 2)
        return 0;
    return FPGA::SetInterfaceFreq(txRate_Hz, rxRate_Hz, channel);
}

} //namespace lime
