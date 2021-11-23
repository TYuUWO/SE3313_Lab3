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
public:
    ServerThread(SocketServer& server)
    : server(server)
    {}

    ~ServerThread()
    {
        // Cleanup
	//...
    }

    virtual long ThreadMain()
    {
        // Wait for a client socket connection
        Socket* newConnection = new Socket(server.Accept());

        // A reference to this pointer 
        Socket& socketReference = *newConnection;
	//You can use this to read data from socket and write data to socket. You may want to put this read/write somewhere else. You may use ByteArray
	// Wait for data
        //socketReference.Read(data);
        // Send it back
        //socketReference.Write(data);
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
    // Create our server
    SocketServer server(3000);    

    // Need a thread to perform server operations
    ServerThread serverThread(server);
	
    // This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    
    while (typeYes) {
    		n++;
		std::cout << "Do you want to make a new thread? (type \"yes\" to continue writing)" << std::endl;
		cin >> response;
		typeYes = (response == "yes");
		if (typeYes){
			threads.push_back(thread(threadTask, n));
		}
	}
	for (thread &th : threads){
		th.join();
	}
    // Shut down and clean up the server
    server.Shutdown();

}
