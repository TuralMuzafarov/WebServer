#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#pragma comment(lib , "ws2_32.lib")

int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    SOCKET Listener;
    //SOCKET Client;
    //Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //if (Client == INVALID_SOCKET)
    //{
    //  std::cout << "Error occured while creating client socket " << GetLastError() << std::endl;
    //    closesocket(Client);
    //    WSACleanup();
    //}
    Listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Listener == INVALID_SOCKET)
    {
        std::cout << "Error Occured while creating Listener socket " << GetLastError() << std::endl;
        closesocket(Listener);
        WSACleanup();
    }
    SOCKADDR_IN ListenerAddr;
    ListenerAddr.sin_family = AF_INET;
    ListenerAddr.sin_port = htons(8080);
    ListenerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int len_listAddr = sizeof(ListenerAddr);

    //SOCKADDR_IN ClientAddr;
    //ClientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //ClientAddr.sin_port = htons(5050);
    //ClientAddr.sin_family = AF_INET;
    //int len_cliAddr = sizeof(ClientAddr);

    if (bind(Listener, (SOCKADDR*)&ListenerAddr, len_listAddr) != 0)
    {
        std::cout << "Error Occured while binding Listener Socket " << GetLastError() << std::endl;
    }

    //if (bind(Client, (SOCKADDR*)&ClientAddr, len_cliAddr) != 0)
    //{
    //    std::cout << "Error Occured while binding Client Socket " << GetLastError() << std::endl;
    //}


    if (listen(Listener, 2) != 0)
    {
        std::cout << "Problem Occur while passing Listener to Listening state " << GetLastError << std::endl;
    }
    std::cout << "Listener is waiting for any upcoming call in port 8080" << std::endl;

    //if (connect(Client, (SOCKADDR*)&ListenerAddr, len_listAddr) != 0)
    //{
    //    std::cout << "Problem occur while connecting client and listener " << GetLastError() << std::endl;
    //}
    int bytes;
    while (true)
    {
        
        SOCKET newSocket = accept(Listener, (SOCKADDR*)&ListenerAddr, &len_listAddr);
        if (newSocket == INVALID_SOCKET)
        {
            std::cout << "Could not accept the connection " << GetLastError() << std::endl;
            closesocket(newSocket);
            WSACleanup();
        }
        char buffer[30750] = { 0 };
        bytes = recv(newSocket, buffer, 3075, 0);
        std::string serverMessage = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
        std::string response = "<html><h1>Hello World</h1></html>";
        serverMessage.append("\n\n");
        serverMessage.append(response);
        
        if (bytes < 0)
        {
            std::cout << "Could not read the client message" << std::endl;
        }

        int byteSent = 0;
        int byteSentTotal = 0;
        while (byteSentTotal < serverMessage.size())
        {
            byteSent = send(newSocket, serverMessage.c_str(), serverMessage.size(), 0);
            if (byteSent < 0) std::cout << "Could not response";
            byteSentTotal += byteSent;
        }
        std::cout << "All data sent" << std :: endl;
        closesocket(newSocket);
        
    }
    WSACleanup();

    
}