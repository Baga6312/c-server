#ifndef BARE_SERVER_H
#define BARE_SERVER_H

#include <thread>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class BareServer {
private:
    int m_serverSocket;
    int m_clientSocket;
    struct sockaddr_in m_serverAddress;
    struct sockaddr_in m_clientAddress;
    char m_buffer[256];

public:
    BareServer();
    void run();
    void start();
    void handleConnection();
    void close();
};

#endif // BARE_SERVER_H
