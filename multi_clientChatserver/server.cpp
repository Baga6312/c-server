#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include "server.h"
#include "client.cpp"











void Server::add_client(int client_socket) {
    std::cout << "Client connected.\n";
    m_clients.push_back(client_socket);
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
}











void Server::remove_client(int client_socket) {
    std::cout << "Client disconnected.\n";
    auto position = std::find(m_clients.begin(), m_clients.end(), client_socket);
    if (position != m_clients.end()) {
        m_clients.erase(position);
    }
    close(client_socket);
}












void Server::broadcast_message(int sender, const std::string& message) {
    // Get the current time
    std::time_t current_time = std::time(nullptr);
    std::tm* time_info = std::localtime(&current_time);

    // Format the time as a string
    char time_string[50];
    std::strftime(time_string, sizeof(time_string), "[%H:%M:%S] ", time_info);

    // Print the time, client number, and message to the console
    std::cout << time_string << "Client #" << sender - 3  << ": " << message ;

    // Broadcast the message to all clients
    for (auto client : m_clients) {
        if (client != sender) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}
















void Server::process_client_message(int sender, const std::string &message) {
    broadcast_message(sender, message);
}











void Server::start() {
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(m_port);

    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket == -1) {
        std::cerr << "Could not create socket.\n";
        return;
    }

    if (bind(m_server_socket, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Could not bind to port.\n";
        return;
    }

    if (listen(m_server_socket, 3) < 0) {
        std::cerr << "Could not start listening on socket.\n";
        return;
    }

    std::cout << "Server started.\n";

    fd_set read_fds, temp_fds;
    FD_ZERO(&read_fds);
    FD_SET(m_server_socket, &read_fds);

    int max_fd = m_server_socket;

    while (true) {
        temp_fds = read_fds;
        if (select(max_fd + 1, &temp_fds, nullptr, nullptr, nullptr) == -1) {
            std::cerr << "Error in select.\n";
            break;
        }

        if (FD_ISSET(m_server_socket, &temp_fds)) {
            int client_socket = accept(m_server_socket, nullptr, nullptr);
            if (client_socket == -1) {
                std::cerr << "Could not accept client.\n";
            } else {
                add_client(client_socket);
                FD_SET(client_socket, &read_fds);
                max_fd = std::max(max_fd, client_socket);
            }
        }


        for  ( auto client : m_clients) { 
            if (FD_ISSET(client,&temp_fds)) { 
                char buffer[1024] = {0} ;
                int read_size = recv(client,buffer ,sizeof(buffer),0);
                if (read_size <= 0) { 
                    remove_client(client); 
                    FD_CLR(client, &read_fds);

                }else{ 
                    process_client_message(client, std::string(buffer, read_size)) ; 
                }
            }
        }
    }
}





















void Server::run() {
    Server(1234);
    Server::start();

    Client("127.0.0.1", 1234);
    Client:start() ; 

}

// int main () { 
//     Server srv = Server(1234) ; 
//     srv.run() ; 
// 	return 0 ; 
// } 


