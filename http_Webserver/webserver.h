#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <iostream>
#include <string>

using namespace std ; 

class WebServer {
private:
    int m_port;
    int m_socket;
    struct sockaddr_in m_serverAddr;
    struct sockaddr_in m_clientAddr;
    socklen_t m_clientLen;
    std::string m_folderPath;
    std::string m_filename;

public:
    WebServer(int port, string folderPath , string filename ): m_port(port) , m_folderPath(folderPath) , m_filename(filename){}
    void init();
    void serveFile(int clientSock, std::string filePath , std::string filename);
    std::string getContentType(std::string fileExt);
};

#endif // WEBSERVER_H
