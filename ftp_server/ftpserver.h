#ifndef FTP_SERVER_H
#define FTP_SERVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>

class FTPServer {
  private:
    int server_socket;
    int client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int client_address_size;
    char buffer[1024];
    std::string root_directory;

  public:
    FTPServer(int port_number, std::string root_directory);
    void start();
    void handle_client();
    void send_file(std::string file_path);
    void receive_file(std::string file_name);
    void send_message(std::string message);
    void list_files();
    void run();
};

#endif // FTP_SERVER_H
