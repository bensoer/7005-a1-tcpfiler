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
        void initiateListen();
        void startSession();
        void connectToServer(const char * host, int port);
        void sendMessage(const char * message);
        const char * receiveMessage();
        void disconnect();

    private:
        int socketPointer;
        int sessionSocketPointer;
        struct	sockaddr_in server;
        struct  sockaddr_in client;
        bool isServer = false;

        const static int BUFFERLEN = 2048;

        void sendDoneMessage();

        int findDoneIndex(const char * message);
};


#endif //INC_7005_A1_TCPFILER_TCPENGINE_H
