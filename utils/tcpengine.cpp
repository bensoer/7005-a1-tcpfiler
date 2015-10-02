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

/**
 * createSocket initiates a socket. If there is an error the method will terminate the program
 */
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

/**
 * makeBind binds the socket to a parameter passed port. createSocket is required to be called before this function.
 * If there is an error, this function will terminate the program.
 * @param port int the port number to bind with
 */
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

/**
 * initiateListen will call the listen function to enable the server to listen for connections
 * @param maxRequestQueue int the max number of pending connection requests that will be queued
 */
void TCPEngine::initiateListen(int maxRequestQueue){
    listen(this->socketPointer, maxRequestQueue);
    cout << "TCP Engine Now Listening" << endl;
}

/**
 * startSession is a server function that accepts a connection and establishes the TCP client and server
 */
void TCPEngine::startSession(){

        this->isServer = true; //if your starting a session, your accepting, so the tcpengine will assume your a server
        socklen_t client_len= sizeof(this->client);
        if ((this->sessionSocketPointer = accept(this->socketPointer, (struct sockaddr *)&client, &client_len)) == -1)
        {
            fprintf(stderr, "Can't accept client\n");
            exit(1);
        }else{
            cout << "Session Initiated. Now Ready To Transmit" << endl;
            printf("Session Initiated with Client of Remote Address:  %s\n", inet_ntoa(client.sin_addr));
        }
}

/**
 * disconnect closes the TCP connection
 */
void TCPEngine::disconnect(){
    if(this->isServer){
        close (this->sessionSocketPointer);
    }else{
        close(this->socketPointer);
    }
}

/**
 * connectToServer is a client function that connect to a passed in host and port
 * @param host string the host to connect to. This is the url that will be DNS resolved
 * @param port int the port number to connect on the server
 */
void TCPEngine::connectToServer(string host, int port){

    struct hostent	*hp;

    if ((hp = gethostbyname(host.c_str())) == NULL)
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

/**
 * sendMessage sends a string message across the socket. It is assumed connection has been established
 * @param message string the message being sent across the network
 */
void TCPEngine::sendMessage(string message){
    // Transmit data through the socket

    int length = message.size();

    if(this->isServer){
        send (this->sessionSocketPointer, message.c_str(), length, 0);
        this->sendDoneMessage();
    }else{
        send (this->socketPointer, message.c_str(), length, 0);
        this->sendDoneMessage();
    }
}

/**
 * sendDoneMessage is a private fuction used by receiveMessage to identify the end of a messege segment being sent
 */
void TCPEngine::sendDoneMessage() {

    char * done = "DOnE!";

    int length = strlen(done);

    if(this->isServer){
        send (this->sessionSocketPointer, done, length, 0);
    }else{
        send (this->socketPointer, done, length, 0);
    }
}

/**
 * receiverMessage listens for incoming messages being sent through the socket.
 */
string TCPEngine::receiveMessage(){


    int socket = this->isServer ? this->sessionSocketPointer : this->socketPointer;

    string totalMessage = "";

    while(1){
        char message[2048];
        int n = recv(socket, message, this->BUFFERLEN - 1, 0);

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
            message[2048] = '\0';
            string tmpString(message);
            totalMessage += tmpString;
        }
    }

    return totalMessage;
}

/**
 * findDoneIndex searches through the passed in message for the DOnE! value in the message and returns its index, so that
 * it can be parced out and the appropriate message passed back
 * @param message const char pointer the message being searched for the DOnE! message
 */
int TCPEngine::findDoneIndex(const char *message) {

    char done[] = "DOnE!";

    for(unsigned int i = 0; i < strlen(message); i++){

        if(message[i] == done[0]){

            unsigned int j = i;
            int relStart = 1;
            while(1){

                j++;
                if(message[j] != done[relStart]){
                    break;
                }else{
                    relStart++;
                }

                if(relStart == 5){
                    return i;
                }

            }


        }
    }

    return -1;

}
