#include <iostream>
#include "ftp_server/ftpserver.h"
#include "http_Webserver/webserver.h"
#include "multi_clientChatserver/server.h" 
#include "Solo_clientChatserver/barebones.h" 

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

using namespace std;


int main() {

    
    int option;

    while (true) {
        cout << "Please select an option:" << endl;
        cout << "1. Barebones Client-Server Chat" << endl;
        cout << "2. Multi-Client Chat Server" << endl;
        cout << "3. HTTP Web Server" << endl;
        cout << "4. FTP Server" << endl;
        cout << "5. Exit" << endl;
        cin >> option;

        switch (option) {
            case 1:
                system(CLEAR);
                BareServer bare ;
                bare.run(); 
                break;
            case 2:
                system(CLEAR);
                MultiChatServer::run();
                break;
            case 3:
                system(CLEAR);
                WebServer server ; 
                server.run(8000 , "/home/dt/Code/web/");
                break;
            case 4:
                system(CLEAR);
                FTPServer ftp ; 
                ftp.run();
                break;
            case 5:
                cout << "Exiting the program." << endl;
                return 0;
            default:
                system(CLEAR);
                cout << "Invalid option. Please try again." << endl;
                break;
        }
    }

    return 0;
}
