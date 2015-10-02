#include <cstdlib>
#include "../utils/tcpengine.h"
#include <iostream>
#include <string>
#include <string.h>
#include "../utils/filehelper.h"
#include <sstream>


using namespace std;

int main(int argc, char **argv){


    const string GET = "GET";
    const string SEND = "SEND";

    bool send = false;


    cout << "Initializing Server";
    TCPEngine manager = TCPEngine();
    manager.createSocket();
    manager.makeBind(7000);
    manager.initiateListen();




    while(1){

        manager.startSession();

        string data = manager.receiveMessage();


        cout << "fileServer.cpp: " << data << endl;

        cout << "converting to string" << endl;

        //convert it back to a string for ease of use
        string stData = data;


        //if has GET <filename> - then the client wants us to return a file to them
        if(stData.compare(0, 3, "GET") == 0){
            //this is a GET request
            cout << "This is a GET Request" << endl;

            int length = stData.size();
            int posTilEnd = length - 4 + 1; //minus 4 index, plus 1 because count start as 1
            string filename = stData.substr(4, posTilEnd);

            cout << "Filename: " << filename << endl;

            string contents = FileHelper::readFile(("./uploads/" + filename));
            const char * fileMessage = contents.c_str();


            int fileLength = strlen(fileMessage);
            stringstream ss;
            ss << fileLength;
            string strFileLength = ss.str();

            send = false;

            manager.sendMessage(contents);

        }

        //if has SEND <filename> - then the client is sending a file to use to save
        if(stData.compare(0, 4, "SEND") == 0){

            cout << "SEEEND" << endl;

            int length = stData.size();
            int posTilEnd = length - 5 + 1; //minus 5 index, plus 1 because count start as 1


            unsigned int i;
            for (i = 6; i < length; i++){


                if(stData[i] == ' '){
                    cout << "index of the space is: " << i << endl;
                    break;
                }
            }

            string filename = stData.substr(5, i-5);

            //i is the index of the space
            string fileContent = stData.substr(i+1, length-i +1);

            cout << "Filename: " << filename << endl;

            FileHelper::writeAllToFile(fileContent.c_str(), "./uploads/" + filename);

        }
    }
}