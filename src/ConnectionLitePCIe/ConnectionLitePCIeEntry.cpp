#ifdef __unix__
#include <unistd.h>
#endif
#include "ConnectionLitePCIe.h"
using namespace lime;
#include <fstream>
#include <map>
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
    
    std::vector<std::string> LimePCIePorts;
    FILE *lsPipe;
    lsPipe = popen("ls /dev/Lime*_control -d -1", "r");
    char tempBuffer[512];
    while(fscanf(lsPipe, "%s", tempBuffer) == 1)
        LimePCIePorts.push_back(tempBuffer);
    pclose(lsPipe);

    std::map<std::string, std::string> port_name_map = {
        {"Lime5GRadio", "LimeSDR-5GRadio"},
        {"LimeQPCIe", "LimeSDR-QPCIe"},
        {"LimePCIe", "LimeSDR-PCIe"}
    };

    for(auto port:LimePCIePorts)
    {
        if(access(port.c_str(), F_OK ) != -1 )
        {
            for (auto port_name:port_name_map)
            {
                size_t pos = port.find(port_name.first);
                if(pos != std::string::npos)
                {
                    std::string dev_nr(&port[pos+port_name.first.length()], &port[port.find("_")]);
                    handle.name = port_name.second + (dev_nr == "0" ? "" : " (" + dev_nr + ")");
                }
            }
            handle.index++;
            handle.addr = port.substr(0, port.find("_"));
            handles.push_back(handle);
        }
    }
    return handles;
}

IConnection *ConnectionLitePCIeEntry::make(const ConnectionHandle &handle)
{
    return new ConnectionLitePCIe(handle.addr);
}
