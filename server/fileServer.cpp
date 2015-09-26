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

        char * data = manager.getData();


        cout << data << endl;

        cout << "converting to string" << endl;

        //convert it back to a string for ease of use
        string stData(data);

        cout << "Back in file Server" << endl;
        cout << stData << endl;

        //if has GET <filename> - then the client wants us to return a file to them
        if(stData.compare(0, 3, "GET") == 0){
            //this is a GET request
            cout << "GEEEET" << endl;

            int length = stData.size();
            int posTilEnd = length - 4 + 1; //minus 4 index, plus 1 because count start as 1
            string filename = stData.substr(4, posTilEnd);

            cout << "Filename: " << filename << endl;



            //get the file and the size
            string contents = FileHelper::readFile(("./uploads/" + filename));
            //string contents = FileHelper::readFile("data/" + filename);
            const char * fileMessage = contents.c_str();


            int fileLength = strlen(fileMessage);
            stringstream ss;
            ss << fileLength;
            string strFileLength = ss.str();

            send = false;

            string strMessage = "200:GET ENABLED " + strFileLength;
            const char * message = strMessage.c_str();

            cout << message << endl;
            manager.sendMessage(message);

            manager.sendMessage(fileMessage);




        }

        //if has SEND <filename> - then the client is sending a file to use to save
        if(stData.compare(0, 4, "SEND") == 0){

            cout << "SEEEND" << endl;

            int length = stData.size();
            int posTilEnd = length - 5 + 1; //minus 5 index, plus 1 because count start as 1
            string filename = stData.substr(5, posTilEnd);

            cout << "Filename: " << filename << endl;

            send = true;
            char * message = "200:SEND ENABLED";

            manager.sendMessage(message);

        }

    }



    //return 0 ;
}