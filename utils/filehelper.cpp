//
// Created by bensoer on 22/09/15.
//

#include "filehelper.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

string FileHelper::readFile(string directory) {

    stringstream sr;
    ifstream ifs;
    ifs.open(directory.c_str(), ifstream::in);

    if(ifs.is_open()){

        while(ifs.peek() != EOF){
            sr << (char) ifs.get();
        }

        ifs.close();
        return sr.str();
    }
}

void FileHelper::writeAllToFile(string text, string directory) {

    ofstream ofs;
    ofs.open (directory.c_str(), ofstream::binary | ofstream::out | ofstream::trunc);

    string fileContent(text);

    ofs << fileContent;

    ofs.close();

}

