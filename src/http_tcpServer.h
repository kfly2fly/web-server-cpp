#ifndef HTTP_TCP_SERVER
#define HTTP_TCP_SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winsock.h>

namespace http
{
    class TcpServer
    {
    public:
        TcpServer(std::string ip_address, int port); // constructor
        ~TcpServer();                                // destructor
        void startListen();

    private:
        std::string m_ip_address;
        int m_port;
        SOCKET m_socket;
        SOCKET m_new_socket;
        long m_incomingMessage;
        struct sockaddr_in m_socketAddress;
        int m_socketAddress_len;
        std::string m_serverMessage;
        WSADATA m_wsaData;

        int startServer();
        void closeServer();
        void acceptConnection(SOCKET &new_socket);
        std::string buildResponse();
        void sendResponse();
    };
} // namespace http

#endif