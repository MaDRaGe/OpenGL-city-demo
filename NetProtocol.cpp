#include "NetProtocol.h"

NetProtocol::NetProtocol()
{
    mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == INVALID_SOCKET)
    {
        printf("Could not create socket : %d\n", WSAGetLastError());
    }
    else
    {
        printf("Socket created.\n");
    }
}

/*
    Подключиться к серверу по указанному ip-адресу

    @params:
        ip - адрес
        port - порт
*/
void NetProtocol::Connect(
    const char* ip, 
    unsigned int port
)
{
    sockaddr_in server;
    server.sin_family = AF_INET;
    #pragma warning(push)
    #pragma warning(disable: 4996)
    server.sin_addr.s_addr = inet_addr(ip);
    #pragma warning(pop)
    server.sin_port = htons(port);

    int connect_error = connect(mySocket, (struct sockaddr*) & server, sizeof(server));
    if (connect_error == SOCKET_ERROR)
    {
        printf("Connection failed : %d\n", WSAGetLastError());
    }
    else
    {
        printf("Connection is established.\n");
    }
}

void NetProtocol::Disconnect()
{
    closesocket(mySocket);
}

/*
    Отправить запрос на сервер с указанным кодом
*/
void NetProtocol::Send(int code)
{
    header;
    header.transactionID = 0;
    header.frameNumber = 1;
    header.frameCount = 1;
    header.dataLen = 1;
    header.funcID = code;
    send(mySocket, (const char*)&header, sizeof(header), 0);
}

void NetProtocol::SendPosition(float x, float y, float z)
{
    float position[] = { x, y, z };
    send(mySocket, (const char*)position, sizeof(float) * 3, 0);
}

void NetProtocol::SendRadius(float radius)
{
    send(mySocket, (const char*)&radius, sizeof(float), 0);
}


/*
    Получить ответ с сервера
*/
void NetProtocol::Recv(void* recvData, int bytesCount)
{
    WaitForBytesToRead(bytesCount);
    recv(mySocket, (char*)recvData, bytesCount, 0);
}

/*
    Ожидать пока станут доступным для чтения указанное число байт

    @params: 
        bytesCount - количество байт
*/
void NetProtocol::WaitForBytesToRead(int bytesCount)
{
    unsigned long bytesToRead = 0;
    ioctlsocket(mySocket, FIONREAD, &bytesToRead);
    while (bytesToRead < bytesCount)
    {
        ioctlsocket(mySocket, FIONREAD, &bytesToRead);
    }
}
