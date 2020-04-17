/**
    @file ConnectionSTREAMEntry.cpp
    @author Lime Microsystems
    @brief Implementation of STREAM board connection.
*/
#ifdef __unix__
#include <unistd.h>
#endif
#include "ConnectionLitePCIe.h"
using namespace lime;
#include <fstream>

//! make a static-initialized entry in the registry
void __loadConnectionLitePCIeEntry(void) //TODO fixme replace with LoadLibrary/dlopen
{
    static ConnectionLitePCIeEntry litePCIeEntry;
}

ConnectionLitePCIeEntry::ConnectionLitePCIeEntry(void):
    ConnectionRegistryEntry("LitePCIe")
{
}

ConnectionLitePCIeEntry::~ConnectionLitePCIeEntry(void)
{
}

std::vector<ConnectionHandle> ConnectionLitePCIeEntry::enumerate(const ConnectionHandle &hint)
{
    std::vector<ConnectionHandle> handles;
    ConnectionHandle handle;
    handle.media = "PCI-E";
    int n_qpcie = 0;
    int n_pcie = 0;
    for(int i = 0; i < ConnectionLitePCIe::MAX_DEV_CNT; i++)
    {
        std::string device_addr = "/dev/litepcie" + std::to_string(i);
        std::string control_name = device_addr + "_control";
        if(access(control_name.c_str(), F_OK ) != -1 )
        {
            if (hint.index < 0 || hint.index == i)
            {
                std::string ep = device_addr + "_read2";
                if(access(ep.c_str(), F_OK) != -1)
                {
                    handle.name = "LimeSDR-QPCIe" + ((n_qpcie>0) ? " (" + std::to_string(n_qpcie) + ")" : "");
                    n_qpcie++;
                }
                else
                {
                    handle.name = "LimeSDR-PCIe" + ((n_pcie>0) ? " (" + std::to_string(n_pcie) + ")" : "");
                    n_pcie++;
                }
                handle.index = i;
                handle.addr = device_addr;
                handles.push_back(handle);
            }
        }
    }
    return handles;
}

IConnection *ConnectionLitePCIeEntry::make(const ConnectionHandle &handle)
{
    return new ConnectionLitePCIe(handle.addr);
}
