#include <thread>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "barebones.h"


BareServer::BareServer() {
    // Create socket
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket < 0) {
        usleep(800000);
        std::cerr << "[-] Error opening socket\n";
        std::exit(1);
    }else{ 
        usleep(500000) ; 
        std::cout<< "[+] Socket created."<< std::endl ; 

    }
    // Set server address
    std::memset(&m_serverAddress, 0, sizeof(m_serverAddress));
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = INADDR_ANY;
    m_serverAddress.sin_port = htons(8888);
    // Bind socket to address
    if (bind(m_serverSocket, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) < 0) {
        usleep(800000) ; 
	perror ( "[-] Error binding socket to address " ) ; 
        std::exit(1);
    }else{ 
        usleep (500000); 
        std::cout << "[+] Binding socket . "<< std::endl ; 
    }
    // Listen for incoming connections
    listen(m_serverSocket, 1);
    std::cout<< "[+] Listening for connection ." << std::endl ;
}

void BareServer::start() {
    socklen_t clientLength = sizeof(m_clientAddress);
    // Accept incoming connection
    m_clientSocket = accept(m_serverSocket, (struct sockaddr *)&m_clientAddress, &clientLength);
    if (m_clientSocket < 0) {
        std::cerr << "[-] Error accepting incoming connection "<<std::endl ;
        std::exit(1);
    }
    std::cout << "[+] Connection established " << std::endl ; 
    // Handle the connection
    handleConnection();
}



void BareServer::handleConnection() {
    bool connectionOpen = true;
    std::thread recvThread([&]() {
        while (connectionOpen) {
            // Clear buffer
            std::memset(m_buffer, 0, sizeof(m_buffer));
            // Read from socket
            int bytesRead = read(m_clientSocket, m_buffer, sizeof(m_buffer)-1);
            if (bytesRead < 0) {
                std::cerr << "[-] Error reading from socket "<< std::endl ;
                std::exit(1);
            }
            std::cout << "[*] Client : " << m_buffer ; 
            if (std::strcmp(m_buffer, "exit") == 0) {
                // If the client sends the "exit" message, close the connection
                connectionOpen = false;
                std::cout << "[*] Closing connection"<<std::endl ;
            }
        }
    });

    std::thread sendThread([&]() {
        while (connectionOpen) {
            // Prompt the server to enter a message
            std::cout << "[*] Server : " ;
            std::string message;
            std::getline(std::cin, message);
            // Write to socket
            const char* messageToSend = (message + "\n").c_str();
            int bytesWritten = write(m_clientSocket, messageToSend, std::strlen(messageToSend));
            if (bytesWritten < 0) {
                std::cerr << "[-]Error writing to socket\n";
                std::exit(1);
            }
        }
    });

    // Wait for both threads to finish before exiting the function
    recvThread.join();
    sendThread.join();
}

void BareServer::close() {
    // Close the sockets
    ::close(m_clientSocket);
    ::close(m_serverSocket);
}

void  BareServer::run() {
    BareServer server;
    server.start();
    server.close();
}

//int main() { 

//	BareServer bare ; 

//	bare.run() ; 
//	return 0 ; 
//} 



