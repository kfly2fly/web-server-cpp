#include "http_tcpServer.h"

int main()
{
    using namespace http;
    TcpServer server = TcpServer("127.0.0.1", 8088);
    server.startListen();

    return 0;
}