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

        // switch (option) {
        //     case 1:
        //         system(CLEAR);
        //         BareServer bare = BareServer() ;
        //         bare.run();
        //         break;
        //     case 2:
        //         system(CLEAR);
        //         Server srv = Server(1234);
        //         srv.run();
        //         break;
        //     case 3:
        //         system(CLEAR);
        //         WebServer server = WebServer(8000, "/home/dt/Code/web/") ; 
        //         server.init();
        //         break;
        //     case 4:
        //         system(CLEAR);
        //         FTPServer ftp = FTPServer(5000 , "/home/dt/Code/"); 
        //         ftp.run();
        //         break;
        //     case 5:
		//         exit(0) ;break; 
        //     default:
        //         system(CLEAR);
        //         cout << "Invalid option. Please try again." << endl;
        //         break;
        // }


    if (option == 1 ) { 
        system(CLEAR);
            BareServer bare = BareServer() ;
            bare.run();
        }
    else if ( option == 2 ) { 
        system(CLEAR);
            Server srv = Server(1234);
            srv.run();
    }else if (option == 3 ) {
        system(CLEAR);
            WebServer server = WebServer(8000, "/home/dt/Code/web/") ; 
            server.init();
    } else if (option == 4 ) { 
        system(CLEAR);
            FTPServer ftp = FTPServer(5000 , "/home/dt/Code/"); 
            ftp.run();
    } else if (option == 5 ) {
        exit(0) ; 
    }else { 
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}
