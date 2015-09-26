//
// Created by bensoer on 22/09/15.
//


#include "tcpengine.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>

using namespace std;


void TCPEngine::createSocket(){
    // Create a stream socket
    if ((this->socketPointer = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("Can't create a socket");
        exit(1);
    }else{
        cout << "Socket Created" << endl;
    }
}

void TCPEngine::makeBind(int port){

    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(port);
    this->server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(this->socketPointer, (struct sockaddr *)&(this->server), sizeof(this->server)) == -1)
    {
        perror("Can't bind name to socket");
        exit(1);
    }else{
        cout << "Port Binding Complete" << endl;
    }

}

void TCPEngine::initiateListen(){
    listen(this->socketPointer, 5);
    cout << "TCP Engine Now Listening" << endl;
}

void TCPEngine::startSession(){


        this->isServer = true;
        socklen_t client_len= sizeof(this->client);
        if ((this->sessionSocketPointer = accept(this->socketPointer, (struct sockaddr *)&client, &client_len)) == -1)
        {
            fprintf(stderr, "Can't accept client\n");
            exit(1);
        }else{
            cout << "Session Initiated. Now Ready To Transmit" << endl;
        }

        printf(" Remote Address:  %s\n", inet_ntoa(client.sin_addr));

        //bp = buf;
        //bytes_to_read = BUFLEN;
        char * message =  new char[this->BUFFERLEN];
        int n;
       /* while ((n = recv (this->sessionSocketPointer, &message, this->BUFFERLEN, 0)) < this->BUFFERLEN)
        {
            //message += n;
            //concattedMessage += message;
        }*/

  //      recv(this->sessionSocketPointer, message, this->BUFFERLEN, 0);


  //      printf ("recieved:%s\n", message);
        //cout << message;

        //send (new_sd, buf, BUFLEN, 0);

   //     return message;
}

void TCPEngine::disconnect(){
    if(this->isServer){
        close (this->sessionSocketPointer);
    }else{
        close(this->socketPointer);
    }

}

void TCPEngine::connectToServer(const char * host, int port){

    struct hostent	*hp;

    if ((hp = gethostbyname(host)) == NULL)
    {
        fprintf(stderr, "Unknown server address\n");
        exit(1);
    }else{
        cout << "Hostname Resolved" << endl;
    }

    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(port);

    bcopy(hp->h_addr, (char *)&(this->server.sin_addr), hp->h_length);

    // Connecting to the server
    if (connect(this->socketPointer, (struct sockaddr *)&(this->server), sizeof(this->server)) == -1)
    {
        fprintf(stderr, "Can't connect to server\n");
        perror("connect");
        exit(1);
    }else{
        cout << "Connection Established" << endl;
    }
}

void TCPEngine::sendMessage(const char * message){
    // Transmit data through the socket

    int length = strlen(message);

    if(this->isServer){
        send (this->sessionSocketPointer, message, length, 0);
        this->sendDoneMessage();
    }else{
        send (this->socketPointer, message, length, 0);
        this->sendDoneMessage();
    }
}

void TCPEngine::sendDoneMessage() {

    char * done = "DOnE!";

    int length = strlen(done);

    if(this->isServer){
        send (this->sessionSocketPointer, done, length, 0);
    }else{
        send (this->socketPointer, done, length, 0);
    }
}

const char * TCPEngine::receiveMessage(){


    int socket = this->isServer ? this->sessionSocketPointer : this->socketPointer;

    string totalMessage = "";

    while(1){
        char message[2048];
        int n = recv(socket, message, this->BUFFERLEN, 0);

        //some dumb bug it likes to recieve nothing
        if(n == 0){
            break;
        }

        int index = this->findDoneIndex(message);

        cout << "TCPEngine: The done index is: " << index << endl;

        if(index != -1){
            cout << "a DOne! was found! Were done!" << endl;
            message[index] = '\0';
            totalMessage += message;
            break;
        }else{
            cout << "a DOne! was not found, so we are going to keep going" << endl;
            string tmpString(message);
            totalMessage += tmpString;
        }
    }



    cout << "TCPEngine: " << totalMessage << endl;

    return totalMessage.c_str();
}

int TCPEngine::findDoneIndex(const char *message) {

    char done[] = "DOnE!";

    for(unsigned int i = 0; i < strlen(message); i++){

        if(message[i] == done[0]){

            cout << "found a D. Message at index: " << i << " : done: " << done[0] << endl;

            unsigned int j = i;
            int relStart = 1;
            while(1){

                j++;
                cout << message[j] << " vs " << done[relStart] << endl;


                if(message[j] != done[relStart]){
                    cout << "they do not match.Aborting" << endl;
                    break;
                }else{
                    cout << "They match!. Moving Forward" << endl;
                    relStart++;
                }

                if(relStart == 5){
                    cout << "We hit all matching!" << endl;
                    return i;
                }

            }


        }
    }

    return -1;

}
