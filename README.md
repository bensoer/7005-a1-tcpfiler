#7005-a1-tcpfiler

##Setup

Compile from root:

g++ -pedantic -Wall -W -o fserver server/fileServer.cpp  utils/tcpengine.cpp utils/filehelper.cpp

g++ -pedantic -Wall -W -o fclient client/fileClient.cpp  utils/tcpengine.cpp utils/filehelper.cpp

Run:

./fserver

./fclient -m GET -p 7000 -h localhost -f testData.txt
