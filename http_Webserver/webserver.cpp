#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include "webserver.h"


using namespace std;


void WebServer::init() {
    // Create socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) {
        cerr << "Error creating socket\n";
        exit(1);
    }

    // Bind socket to port
    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(m_port);
    m_serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");  // Bind to all network interfaces

    if (bind(m_socket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) < 0) {
        cerr << "Error binding socket to port " << m_port << endl;
        exit(1);
    }

    // Listen for incoming connections
    if (listen(m_socket, 5) < 0) {
        cerr << "Error listening for connections\n";
        exit(1);
    }

    cout << "Server started on port " << m_port << endl;

    while (true) {
        // Accept incoming connection
        m_clientLen = sizeof(m_clientAddr);
        int clientSock = accept(m_socket, (struct sockaddr*)&m_clientAddr, &m_clientLen);
        if (clientSock < 0) {
            cerr << "Error accepting connection\n";
            continue;
        }

        // Receive request from client
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        recv(clientSock, buffer, sizeof(buffer), 0);
        string request(buffer);

        // Extract file path from request
        size_t start = request.find("GET /") + 5;
        size_t end = request.find(" HTTP");
        string filePath = request.substr(start, end - start);
        
        string filename ; 

        
        // Serve file to client
        serveFile(clientSock, filePath , filename);

        // Close client socket
        close(clientSock);
    }
}

void WebServer::serveFile(int clientSock, string filePath, string filename ) {
    // If filePath is empty or '/', serve index.html
    if (filePath == "" || filePath == ".") {
        filePath = "index.html";
    }

    // Prepend folder path to filePath
    string fullFilePath = m_folderPath + filePath;
    cout <<""<<endl ; 
    cout << "Attempting to open file: " << fullFilePath << endl;
    ifstream file(fullFilePath);
         if (!file.is_open()) {
        cerr << "Error opening file: " << fullFilePath << endl;
        perror("file ");
    } else {
        // Read file contents into string
        stringstream buffer;
        buffer << file.rdbuf();
        string fileContents = buffer.str();

        // Get file extension
        size_t dotPos = filePath.rfind(".");
        string fileExt = "";
        if (dotPos != string::npos) {
            fileExt = filePath.substr(dotPos + 1);
        }

        // Send HTTP headers to client
        ostringstream headers;
        headers << "HTTP/1.1 200 OK\r\n";
        headers << "Content-Type: " << getContentType(fileExt) << "\r\n";
        headers << "Content-Length: " << fileContents.length() << "\r\n";
        headers << "\r\n";

        // Send headers and file contents to client
        send(clientSock, headers.str().c_str(), headers.str().length(), 0);
        send(clientSock, fileContents.c_str(), fileContents.length(), 0);

        cout << "    |" << endl;
        cout << "  Served file: " << fullFilePath << endl;
        cout << "" << endl;
    }

    // Close the file
    file.close();
}


string WebServer::getContentType(string fileExt) {
    if (fileExt == "html" || fileExt == "htm") {
        return "text/html";
    } else if (fileExt == "txt") {
        return "text/plain";
    } else if (fileExt == "css") {
        return "text/css";
    } else if (fileExt == "js") {
        return "text/javascript";
    } else if (fileExt == "png") {
        return "image/png";
    } else if (fileExt == "jpg" || fileExt == "jpeg") {
        return "image/jpeg";
    } else if (fileExt == "gif") {
        return "image/gif";
    } else if (fileExt == "ico") {
        return "image/x-icon";
    } else {
        return "application/octet-stream";
    }
}


// int main ( int argc , char** argv) {

//       if (argc < 3) {
//         cerr << "Usage: " << argv[0] << " [port] [folder path]\n";
//     }

//     int port = atoi(argv[1]);

//     string folderPath = argv[2];

//     WebServer server =  WebServer(port , folderPath);

//     server.init();

//     return 0 ;
// }
