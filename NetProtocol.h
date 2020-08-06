#pragma once
#include <WinSock2.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>

#pragma pack(push, 1)
struct NetworkHeader
{
    std::uint16_t transactionID;
    std::uint16_t frameNumber;
    std::uint16_t frameCount;
    std::uint32_t dataLen;
    std::uint8_t funcID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct GraphicObjectDescription
{
    std::uint32_t objectID;
    std::uint8_t objectType;
    float x, y, z;
    float yaw;
    char model[16];
    float OBB[3];
    std::uint8_t reserved[47];
};
#pragma pack(pop)

class NetProtocol
{
public:
    static NetProtocol& instance()
    {
        static NetProtocol net;
        return net;
    }
    void Connect(const char* ip, unsigned int port);
    void Disconnect();
    void Send(int code);
    void SendPosition(float x, float y, float z);
    void SendRadius(float radius);
    void Recv(void* recvData, int bytesCount);

private:
    void WaitForBytesToRead(int bytesCount);
    NetProtocol();
    NetProtocol(const NetProtocol& root) = delete;
    NetProtocol& operator=(const NetProtocol&) = delete;

private:
    SOCKET mySocket;
    NetworkHeader header;
};

