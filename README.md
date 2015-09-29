#7005-a1-tcpfiler

##Setup

###To Compile

Compile the Server<br>
`g++ -pedantic -Wall -W -o fserver server/fileServer.cpp  utils/tcpengine.cpp utils/filehelper.cpp` <br>
Compile the Client<br>
`g++ -pedantic -Wall -W -o fclient client/fileClient.cpp  utils/tcpengine.cpp utils/filehelper.cpp`

###To Run

Run the Server<br>
`./fserver` <br>

To GET with the client<br>
`./fclient -m GET -p 7000 -h localhost -f testData.txt` <br>

To SEND with the client<br>
`./fclient -m SEND -p 7000 -h localhost -f testData.txt`
