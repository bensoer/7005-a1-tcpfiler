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

char * TCPEngine::getData(){


        this->isServer = true;
        socklen_t client_len= sizeof(this->client);
        if ((this->sessionSocketPointer = accept(this->socketPointer, (struct sockaddr *)&client, &client_len)) == -1)
        {
            fprintf(stderr, "Can't accept client\n");
            exit(1);
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

        recv(this->sessionSocketPointer, message, this->BUFFERLEN, 0);


        printf ("recieved:%s\n", message);
        //cout << message;

        //send (new_sd, buf, BUFLEN, 0);

        return message;
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

    if(this->isServer){
        send (this->sessionSocketPointer, message, this->BUFFERLEN, 0);
    }else{
        send (this->socketPointer, message, this->BUFFERLEN, 0);
    }
}

char * TCPEngine::receiveMessage(int receiveBuffer){

    if(receiveBuffer == -1){
        receiveBuffer = this->BUFFERLEN;
    }

    char * message = new char[receiveBuffer];
    if(this->isServer){
       recv(this->sessionSocketPointer, message, receiveBuffer, 0);
    }else{
        recv(this->socketPointer, message, receiveBuffer, 0);
    }

    cout << message << endl;

    return message;
}
