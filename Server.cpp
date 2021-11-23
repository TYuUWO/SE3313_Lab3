#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>
#include <thread>
#include <string>
#include <unistd.h>
#include <iostream>
#include <algorithm>

using namespace Sync;
using namespace std;
namespace{
	volatile bool typeYes = true;
}
// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;
    ByteArray data;
    std::vector<std::thread> threads;
public:
    ServerThread(SocketServer& server)
    : server(server)
    {}

    ~ServerThread()
    {
        // Cleanup
	//...
	cout << "This one";
	server.Shutdown();
    }

    virtual long ThreadMain()
    {
        // Wait for a client socket connection
        Socket* newConnection = new Socket(server.Accept());

        // A reference to this pointer 
        Socket& socketReference = *newConnection;
	//You can use this to read data from socket and write data to socket. You may want to put this read/write somewhere else. You may use ByteArray
	// Wait for data
	do {
        socketReference.Read(data);

        //Convert it to a string and capitalize the string
        string str = data.ToString();
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        
        //Convert it back to a ByteArray type
        ByteArray response = ByteArray(str);
        
        // Send it back
        socketReference.Write(response);
        }
        while (data.ToString() !="done");
	return 1;
    }
};


int threadTask(int data) {
	while(typeYes){
	cout << "Thread: " << data << endl;
	sleep(5);
	}
	return 0;
}

int main(void)
{
	string sData;
	string response;
	int n = 0;
	
    std::vector<std::thread> threads;
	
    std::cout << "I am a server." << std::endl;
	sData = "Hello from server.";
	
    std::cout << "Press 'enter' to terminate" << std::endl;
    // Create our server
    SocketServer server(3000);    
	
    // Need a thread to perform server operations
    ServerThread serverThread(server);
	
    // This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    
    // Shut down and clean up the server
    server.Shutdown();

}
