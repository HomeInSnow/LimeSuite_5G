#include "ConnectionLitePCIe.h"
#include <unistd.h>
#include <fcntl.h>
#include "Logger.h"

using namespace std;

using namespace lime;

static const char* rd_ep_names[] = { "/dev/litepcie_read0",  "/dev/litepcie_read1",  "/dev/litepcie_read2"};
static const char* wr_ep_names[] = { "/dev/litepcie_write0",  "/dev/litepcie_write1",  "/dev/litepcie_write2"};

ConnectionLitePCIe::ConnectionLitePCIe(const char* control_ep) :
    isConnected(true)
{
    control_fd = open(control_ep, O_RDWR);
    if (control_fd<0)
    {
        isConnected = false;
        lime::error("Failed to open Lite PCIe");
        return;
    }
    for (int i = 0; i < MAX_EP_CNT; i++)
        rd_ep_fd[i] = wr_ep_fd[i] = -1;
}

ConnectionLitePCIe::~ConnectionLitePCIe()
{
    if (control_fd >=0)
        close(control_fd);
}

bool ConnectionLitePCIe::IsOpen()
{
    return isConnected;
}

int ConnectionLitePCIe::Write(const unsigned char *buffer, const int length, int /*timeout_ms*/)
{
    return write(control_fd, buffer, length);
}

int ConnectionLitePCIe::Read(unsigned char *buffer, const int length, int timeout_ms)
{
    uint32_t status = 0;
    timeout_ms = 500;
    auto t1 = chrono::high_resolution_clock::now();
    do
    {   //wait for status byte to change
        read(control_fd, &status, sizeof(status));
        if ((status&0xFF00) != 0)
            break;
        std::this_thread::sleep_for(std::chrono::microseconds(250));
    }
    while (std::chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - t1).count() < timeout_ms);

    if ((status&0xFF00)== 0)
        return 0;   //timeout
    return read(control_fd, buffer, length);
}

int ConnectionLitePCIe::GetBuffersCount() const
{
    return 1;
}

int ConnectionLitePCIe::CheckStreamSize(int size) const
{
    return 8; //size > 8 ? 8 : size;
}

int ConnectionLitePCIe::ReceiveData(char *buffer, int length, int epIndex, int timeout_ms)
{
    if (rd_ep_fd[epIndex] == -1)
    {
       if ((rd_ep_fd[epIndex] = open(rd_ep_names[epIndex], O_RDONLY | O_NOCTTY | O_NONBLOCK))==-1)
       {
            lime::error("open read endpoint failed");
            return 0;
       }
    }

    int totalBytesReaded = 0;
    int bytesToRead = length;
    auto t1 = chrono::high_resolution_clock::now();

    do
    {
        int bytesReceived = read(rd_ep_fd[epIndex], buffer+totalBytesReaded, length-totalBytesReaded);
 	if (bytesReceived < 0)
	    std::terminate();
        if (bytesReceived == 0)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            continue;
        }
        totalBytesReaded += bytesReceived;
        if (totalBytesReaded < length)
            bytesToRead -= bytesReceived;
        else
            break;
    }while (std::chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - t1).count() < timeout_ms);

    return totalBytesReaded;
}

void ConnectionLitePCIe::AbortReading(int epIndex)
{
    if (rd_ep_fd[epIndex] >= 0)
    {
        close(rd_ep_fd[epIndex]);
        rd_ep_fd[epIndex] = -1;
    }
}

int ConnectionLitePCIe::SendData(const char *buffer, int length, int epIndex, int timeout_ms)
{
    if (wr_ep_fd[epIndex] == -1)
    {
       if ((wr_ep_fd[epIndex] = open(wr_ep_names[epIndex], O_WRONLY | O_NOCTTY | O_NONBLOCK))==-1)
       {
            lime::error("open write endpoint failed");
            return 0;
       }
    }

    int totalBytesSent = 0;
    int bytesToSend = length;
    auto t1 = chrono::high_resolution_clock::now();
    do
    {
        int bytesSent = write(wr_ep_fd[epIndex], buffer+totalBytesSent, length-totalBytesSent);
 	if (bytesSent < 0)
	    std::terminate();
        if (bytesSent == 0)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(500));
            continue;
        }
        totalBytesSent += bytesSent;
        if (totalBytesSent < length)
            bytesToSend -= bytesSent;
        else
            break;
    }while (std::chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - t1).count() < timeout_ms);
    return totalBytesSent;
}

void ConnectionLitePCIe::AbortSending(int epIndex)
{
    if (wr_ep_fd[epIndex] >= 0)
    {
        close(wr_ep_fd[epIndex]);
        wr_ep_fd[epIndex] = -1;
    }
}

int ConnectionLitePCIe::BeginDataReading(char* buffer, uint32_t length, int ep)
{
    return ep;
}

bool ConnectionLitePCIe::WaitForReading(int contextHandle, unsigned int timeout_ms)
{
    return true;
}

int ConnectionLitePCIe::FinishDataReading(char* buffer, uint32_t length, int contextHandle)
{
    return ReceiveData(buffer, length, contextHandle, 3000);
}

int ConnectionLitePCIe::BeginDataSending(const char* buffer, uint32_t length, int ep)
{
    return SendData(buffer, length,  ep, 3000);
}

bool ConnectionLitePCIe::WaitForSending(int contextHandle, uint32_t timeout_ms)
{
    return true;
}

int ConnectionLitePCIe::FinishDataSending(const char* buffer, uint32_t length, int contextHandle)
{
    return contextHandle;
}
