#ifndef MULTICHAT_SERVER_H
#define MULTICHAT_SERVER_H

#include <vector>
#include <string>

class MultiChatServer {
public:
    MultiChatServer(int port): m_port(port), m_server_socket(0) {}
    void start();
    void run();

private:
    void add_client(int client_socket);
    void remove_client(int client_socket);
    void broadcast_message(int sender, const std::string& message);
    void process_client_message(int sender, const std::string& message);

    int m_port;
    int m_server_socket;
    std::vector<int> m_clients;
};

#endif // MULTICHAT_SERVER_H
