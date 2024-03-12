#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include "ftpserver.h"





    FTPServer::FTPServer(int port_number, std::string root_directory) {
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1) {
            perror ("[-] Error creating socket ") ; 
            exit(EXIT_FAILURE);
        }

        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(port_number);

        if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
            perror ("[-] Error creating socket ") ; 
            exit(EXIT_FAILURE);
        }

        this->root_directory = root_directory;
    }







    void FTPServer::start() {
        listen(server_socket, 1);
        std::cout << "FTP server started, waiting for connections..." << std::endl;

        client_address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_size);
        if (client_socket < 0) {
            perror ("[-] Error accepting connection  ") ; 
            exit(EXIT_FAILURE);
        }

        std::cout << "Client connected" << std::endl;

        handle_client();
    }







    void FTPServer::handle_client() {
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            read(client_socket, buffer, sizeof(buffer)) ; 
            std::string command(buffer , 4) ; 

            command = command.substr(0, command.find("\n"));
            std::cout << "Received command: " << command << std::endl;

            if (command == "quit") {
                break;
            } else if (command == "list") {
                std::cout << "LISTING CURRENT DIRECTORY ..." << std::endl ; 
                list_files();
            } else if (command.substr(0, 4) == "get ") {
                std::string file_path = root_directory + command.substr(4);
                std::cout << "SENDING FILE ..." << std::endl ; 
                send_file(file_path);
            } else if (command.substr(0, 4) == "put ") {
                std::string file_name = command.substr(4);
                std::cout << "UPLOADING FILE ..." << std::endl ; 
                receive_file(file_name);
            } else {
                std::string message = "Invalid command\n";
                send_message(message);
            }
        }

        close(client_socket);
        std::cout << "Client disconnected" << std::endl;
    }







    void FTPServer::send_file(std::string file_path) {
        std::ifstream file(file_path, std::ios::binary);
        if (!file.is_open()) {
            std::string message = "File not found\n";
            send_message(message);
            return;
        }
        file.seekg(0, std::ios::end);
        int file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        send(client_socket, &file_size, sizeof(file_size), 0);

        char *file_buffer = new char[file_size];
        file.read(file_buffer, file_size);
        file.close();

        send(client_socket, file_buffer, file_size, 0);
        delete[] file_buffer;
    }








    void FTPServer::receive_file(std::string file_name) {
        int file_size;
        recv(client_socket, &file_size, sizeof(file_size), 0);

        char *file_buffer = new char[file_size];
        recv(client_socket, file_buffer, file_size, 0) ; 
        std::ofstream file(root_directory + file_name, std::ios::binary);
        file.write(file_buffer, file_size);
        file.close();
        delete[] file_buffer;
    }






    void FTPServer::send_message(std::string message) {
        send(client_socket, message.c_str(), message.length(), 0);
    }




   void FTPServer::list_files() {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(root_directory.c_str())) != NULL) {
            // print header
            std::cout << "Directory listing for " << root_directory << ":" << std::endl;
            std::cout << "------------------------" << std::endl;

            // print file names
            while ((ent = readdir(dir)) != NULL) {
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                    std::cout << ent->d_name << std::endl;
                }
            }
            closedir(dir);
        } else {
            // could not open directory
            perror("Error opening directory") ; 
        }
   }
   void FTPServer::run() {
    int port_number = 5000;
    std::string root_directory = "/home/dt/Code/CC++/";

    FTPServer server(port_number, root_directory);
    server.start();

}

// int main () { 
//     FTPServer ftp = FTPServer(5000 , "/home/dt/Code/CC++") ; 
//     ftp.run();
//     return 0 ; 
// }



