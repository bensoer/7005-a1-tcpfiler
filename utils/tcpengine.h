//
// Created by bensoer on 22/09/15.
//

#ifndef INC_7005_A1_TCPFILER_TCPENGINE_H
#define INC_7005_A1_TCPFILER_TCPENGINE_H

#include <netinet/in.h>
#include <string>
#include <unistd.h>

using namespace std;

class TCPEngine {
    public:
        void createSocket();
        void makeBind(int port);
        void initiateListen(int maxRequestQueue = 5);
        void startSession();
        void connectToServer(string host, int port);
        void sendMessage(string message);
        string receiveMessage();
        void disconnect();

    private:
        /** identifier to a socket that may belong to a client or server. On the server this socket has not been
         * accepted yet **/
        int socketPointer;
        /** identifier to a socket that has been accepted by the server. This is only used on the server system. Clients
         * interact only using the socketPointer **/
        int sessionSocketPointer;
        struct	sockaddr_in server;
        struct  sockaddr_in client;
        bool isServer = false;

        const static int BUFFERLEN = 2048;

        void sendDoneMessage();

        int findDoneIndex(const char * message);
};


#endif //INC_7005_A1_TCPFILER_TCPENGINE_H
