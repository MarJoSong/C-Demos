#include <iostream>
#include <WinSock2.h>
#include <ip2string.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;

    char recvData[255], sendData[255];

    if (WSAStartup(sockVersion, &wsaData)!=0)
        return 0;

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket==INVALID_SOCKET) {
        std::cout << "�׽��ִ���ʧ��" << std::endl;
        WSACleanup();
        return 0;
    }

    sockaddr_in addrListen{};
    addrListen.sin_family = AF_INET;
    addrListen.sin_port = htons(7692);
    addrListen.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(listenSocket, (SOCKADDR*)&addrListen, sizeof(addrListen))==SOCKET_ERROR) {
        std::cout << "��ʧ��" << std::endl;
        closesocket(listenSocket);
        return 0;
    }

    if (listen(listenSocket, 5)==SOCKET_ERROR) {
        std::cout << "��������" << std::endl;
        closesocket(listenSocket);
        return 0;
    }

    while (true) {
        SOCKET revSocket;
        sockaddr_in remoteAddr{};
        int remoteAddrLen = sizeof(remoteAddr);
        std::cout << "�ȴ�����..." << std::endl;
        revSocket = accept(listenSocket, (SOCKADDR*)&remoteAddr, &remoteAddrLen);
        if (INVALID_SOCKET==revSocket) {
            std::cout << "����˽��տͻ�������ʧ��: " << WSAGetLastError() <<std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 0;
        } else {
            char ipClient[16];
            RtlIpv4AddressToString((const in_addr*)&remoteAddr.sin_addr, ipClient);
            std::cout << "�������ͻ��˽������ӳɹ�: " << ipClient << std::endl;
        }
        /*
        int res = recv(revSocket, recvData, 255, 0);
        if (res>0) {
            std::cout << "�յ� " << res << " �ֽ�����" << std::endl;
            std::cout << "����Ϊ: " << recvData << std::endl;
        } else if (res==0) {
            std::cout << "���ӹر�" << std::endl;
        } else {
            std::cout << "����ʧ��: " << WSAGetLastError() << std::endl;
        }

        while (std::cin>>sendData) {
            if (strcmp(sendData, "q")==0) {
                std::cout << "������ֹͣ��������" << std::endl;
                break;
            }
            send(revSocket, sendData, strlen(sendData), 0);
        }
         */
    }

    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
