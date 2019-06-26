#pragma once
#include <ConnectionRegistry.h>
#include "LMS64CProtocol.h"
#include <string>
#include <mutex>
#include <atomic>

namespace lime{

class ConnectionLitePCIe : public LMS64CProtocol
{
public:
    ConnectionLitePCIe(const char* control_ep);
    ~ConnectionLitePCIe(void);

    bool IsOpen();
    int GetOpenedIndex();

    int Write(const unsigned char *buffer, int length, int timeout_ms = 100) override;
    int Read(unsigned char *buffer, int length, int timeout_ms = 100) override;
protected:
    int GetBuffersCount() const override;
    int CheckStreamSize(int size) const override;

    int ReceiveData(char* buffer, int length, int epIndex, int timeout = 100) override;
    int SendData(const char* buffer, int length, int epIndex, int timeout = 100) override;

    int BeginDataReading(char* buffer, uint32_t length, int ep) override;
    bool WaitForReading(int contextHandle, unsigned int timeout_ms) override;
    int FinishDataReading(char* buffer, uint32_t length, int contextHandle) override;
    void AbortReading(int epIndex) override;

    int BeginDataSending(const char* buffer, uint32_t length, int ep) override;
    bool WaitForSending(int contextHandle, uint32_t timeout_ms) override;
    int FinishDataSending(const char* buffer, uint32_t length, int contextHandle) override;
    void AbortSending(int epIndex) override;
    
private:
    static const int MAX_EP_CNT = 3;
    int control_fd;
    int rd_ep_fd[MAX_EP_CNT];
    int wr_ep_fd[MAX_EP_CNT];
    eConnectionType GetType(void) {return PCIE_PORT;}

    bool isConnected;
};

class ConnectionLitePCIeEntry : public ConnectionRegistryEntry
{
public:
    ConnectionLitePCIeEntry(void);

    ~ConnectionLitePCIeEntry(void);

    std::vector<ConnectionHandle> enumerate(const ConnectionHandle &hint);

    IConnection *make(const ConnectionHandle &handle);
};

}
