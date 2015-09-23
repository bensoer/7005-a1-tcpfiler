//
// Created by bensoer on 22/09/15.
//

#ifndef INC_7005_A1_TCPFILER_FILEHELPER_H
#define INC_7005_A1_TCPFILER_FILEHELPER_H

#include <string>

using namespace std;

class FileHelper {
    public:
        static string readFile(string directory);
        static void writeAllToFile(char * text, string directory);

};

#endif //INC_7005_A1_TCPFILER_FILEHELPER_H
