#include "fileClient.h"

#include <cstdlib>
#include "../utils/tcpengine.h"
#include "../utils/argparcer.h"
#include "../utils/filehelper.h"
#include <iostream>
#include <string>
#include <string.h>
#include <cstring>


using namespace std;

int main(int argc, char **argv){

    // -m for mode. Values: SEND , GET
    // -p for port
    // -h for host
    // -f for filename

    ArgParcer parcer = ArgParcer();

    string strHost = parcer.GetTagData("-h", argv, argc);
    int port = parcer.GetTagVal("-p", argv, argc);
    string mode = parcer.GetTagData("-m", argv, argc);
    string filename = parcer.GetTagData("-f", argv, argc);

    if(strHost.compare("-1") == 0){
        strHost = "localhost";
    }

    if(port == -1){
        port = 7000;
    }

    if(mode.compare("-1") == 0 || filename.compare("-1") == 0){
        cout << "Invalid parameters passed." << endl;
        cout << "fileClient [-m MODE] [-p PORT] -h HOST -f FILENAME" << endl;
        exit(0);
    }


    cout << "Initializing Client";
    TCPEngine manager = TCPEngine();

    manager.createSocket();
    //char host[] = "localhost";
    const char * host = strHost.c_str();
    //manager.connectToServer(host, 7000);
    manager.connectToServer(host, port);
    //char message[] = "SEND jkdlsklkl";
    string * strMessage = new string(mode + " " + filename);




    if(mode.compare("GET") == 0){

        cout << "Preparing to GET file" << endl;

        const char * message = strMessage->c_str();
        manager.sendMessage(message);
        string returnedFileContent = manager.receiveMessage();
        cout << "Recieved File. "<< filename << ". Now Disconnecting" << endl;
        manager.disconnect();

        size_t position = filename.find("/");
        string file = "";
        if(position != string::npos){
            cout << "There is a slash to parce out of the filename" << endl;
            file = filename.substr(position, filename.size());
        }else{
            cout << "There is no slash to parce out of the filename" << endl;
            file = filename;
        }

        cout << "Writing File Contents to File" << endl;
        FileHelper::writeAllToFile(returnedFileContent, "./data/" + file);


    }else if(mode.compare("SEND") == 0){
        //cout << "Send is not implemented" << endl ;

        cout << "Preparing to send File Contents" << endl;

        size_t position = filename.find("/");
        string file = "";
        if(position != string::npos){
            cout << "There is a slash to parce out of the filename" << endl;
            file = filename.substr(position, filename.size());
        }else{
            cout << "There is no slash to parce out of the filename" << endl;
            file = filename;
        }
        cout << "Reading File: " << file << endl;
        string fileContents = FileHelper::readFile("./data/" + file);

        *strMessage += " " + fileContents;
        const char * message = strMessage->c_str();
        manager.sendMessage(message);

        cout << "File Sent. Disconnecting" << endl;

        manager.disconnect();

    }

    cout << "Terminating Program" << endl;

    return 0 ;
}