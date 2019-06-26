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

static const char*  control_name = "/dev/litepcie_control";

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

    if(access(control_name, F_OK ) != -1 )
    {
        handle.name = "LitePCIe";
        handle.index = 0;
        handles.push_back(handle);
    }
    return handles;
}

IConnection *ConnectionLitePCIeEntry::make(const ConnectionHandle &handle)
{
    return new ConnectionLitePCIe(control_name);
}
