#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
public:
    Client(const char *ip_address, int port) : m_ip_address(ip_address), m_port(port), m_socket(0){
     start();
    }
    void start();

private:
    void receive_messages();
    const char *m_ip_address;
    int m_port;
    int m_socket;
};

#endif // CLIENT_H
