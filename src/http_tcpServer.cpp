#include "http_tcpServer.h"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <winsock.h>

namespace
{
    const int BUFFER_SIZE = 30720;

    void log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    void exitWithError(const std::string &errorMessage)
    {
        std::cout << "LAST ERROR: " << WSAGetLastError() << std::endl;
        log("ERROR: " + errorMessage);
        exit(1);
    }
}

namespace http
{
    TcpServer::TcpServer(std::string ip_address, int port)
        : m_ip_address{ip_address}, m_port{port}, m_socket{},
          m_new_socket{}, m_incomingMessage{}, m_socketAddress{},
          m_socketAddress_len{sizeof(m_socketAddress)},
          m_serverMessage{}
    {

        m_socketAddress.sin_family = AF_INET;
        m_socketAddress.sin_port = htons(m_port);
        m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

        if (startServer() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(m_socketAddress.sin_port);
        }
    }

    TcpServer::~TcpServer()
    {

        closeServer();
    }

    int TcpServer::startServer()
    {

        if (WSAStartup(MAKEWORD(2, 0), &m_wsaData) != 0)
        {
            exitWithError("WSAStartup failed");
        }

        m_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (m_socket < 0)
        {
            exitWithError("Cannot create socket");
            return 1;
        }

        if (bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
        {
            exitWithError("Cannot connect socket to address");
            return 1;
        }

        return 0;
    }

    void TcpServer::closeServer()
    {

        closesocket(m_socket);
        closesocket(m_new_socket);
        WSACleanup();
        exit(0);
    }

    void TcpServer::startListen()
    {

        if (listen(m_socket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }
        std::ostringstream ss;
        ss << "\n*** Listening on ADRESS: "
           << inet_ntoa(m_socketAddress.sin_addr)
           << " PORT: " << ntohs(m_socketAddress.sin_port)
           << " ***\n\n";
        log(ss.str());

        int bytesRecieved;

        while (true)
        {
            log("===== Waiting for a new connection =====\n\n\n");
            acceptConnection(m_new_socket);

            char buffer[BUFFER_SIZE] = {0};
            bytesRecieved = recv(m_new_socket, buffer, BUFFER_SIZE, 0);
            if (bytesRecieved < 0)
            {
                exitWithError("Failed to receive bytes from client socket connection");
            }

            std::ostringstream ss;
            ss << "----- Recieved Request from Client -----\n\n";
            log(ss.str());

            sendResponse();

            closesocket(m_new_socket);
        }
    }

    void TcpServer::acceptConnection(SOCKET &new_socket)
    {
        std::cout << "acceptConnection()" << std::endl;

        new_socket = accept(m_socket, (sockaddr *)&m_socketAddress, &m_socketAddress_len);
        if (new_socket < 0)
        {
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from ADDRESS: "
               << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: "
               << ntohs(m_socketAddress.sin_port);
            exitWithError(ss.str());
        }
    }

    std::string TcpServer::buildResponse()
    {
        std::cout << "buildResponse()" << std::endl;

        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n" << htmlFile;
        return ss.str();
    }

    void TcpServer::sendResponse()
    {
        std::cout << "sendResponse()" << std::endl;

        int bytesSent;
        long totalBytesSent = 0;

        while (totalBytesSent < m_serverMessage.size())
        {
            bytesSent = send(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size(), 0);
            if (bytesSent < 0)
            {
                break;
            }
            totalBytesSent += bytesSent;
        }

        if (totalBytesSent == m_serverMessage.size())
        {
            log("----- Server Response sent to Client -----\n\n");
        }
        else
        {
            log("Error sending response to Client");
        }
    }

} // namespace http