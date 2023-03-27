#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"


void Client::start() {
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(m_ip_address);
    server_address.sin_port = htons(m_port);

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        std::cerr << "Could not create socket.\n";
        return;
    }

    if (connect(m_socket, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Could not connect to server.\n";
        return;
    }

    std::cout << "Connected to server.\n";

    std::thread message_thread(&Client::receive_messages, this);
    message_thread.detach();

    std::string message;
    while (getline(std::cin, message)) {
        send(m_socket, message.c_str(), message.size(), 0);
    }

    close(m_socket);
}

void Client::receive_messages() {
    char buffer[1024] = {0};
    while (true) {
        int read_size = recv(m_socket, buffer, sizeof(buffer), 0);
        if (read_size <= 0) {
            break;
        }
        std::cout << "Received message: " << buffer << "\n";
        memset(buffer, 0, sizeof(buffer));
    }
}

