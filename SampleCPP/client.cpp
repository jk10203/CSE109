#include <iostream>     // standard IO
#include <string.h>     // strerror
#include <unistd.h>     // close
#include <sys/socket.h> // socket stuff
#include <netinet/in.h> // sockaddr_in, htonl|s, ntohl|s
#include <netdb.h>      // gethostbyname, hostent (host entry)
using namespace std;

// program to open a socket, listen for a client to make a connection,
// then send and receive a mesage
//
// g++ -Wall -Wextra -o client client.cpp
//
// execute "server" in one window, execute "client" in another window on
// same server; can run both in same window, but difficult to see which
// program is printing, sending, or receiving

int main(void) {
  int socketFD;
  struct sockaddr_in serverAddr;
  struct hostent* host;
  int serverPort = 9000;
  char msg[1024];
  int bytes_xferd;

  // create a socket
  if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cerr << "Socket creation error (" << errno << "): " << strerror(errno);
    exit(10);
  } else
    cerr << "Socket created" << endl;

  // create server address
  memset(&serverAddr, '\0', sizeof(struct sockaddr_in));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(serverPort);
  // resolve the server address (convert from symbolic name to IP number)
  if ((host = gethostbyname("localhost")) == NULL) { // convert symbol to IP
    cerr << "Server address error (" << errno << "): " << strerror(errno);
    exit(11);
  } else
    cerr << "Server address resolved" << endl;
  // write resolved IP address of a server to the address structure
  memmove(&(serverAddr.sin_addr.s_addr), host->h_addr_list[0], 4);

  // print a resolved address of server (the first IP of the host)
  cout << "Server address = ";
  cout << (int)(host->h_addr_list[0][0] & 0xff) << ".";
  cout << (int)(host->h_addr_list[0][1] & 0xff) << ".";
  cout << (int)(host->h_addr_list[0][2] & 0xff) << ".";
  cout << (int)(host->h_addr_list[0][3] & 0xff) << ", ";
  cout << "port = " << (int)serverPort << endl;

  // Connect to a remote server
  if ((connect(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0) {
    cerr << "Connection error (" << errno << "): " << strerror(errno);
    exit(12);
  } else
    cerr << "Connection established" << endl;

  if ((bytes_xferd = recv(socketFD, msg, 1023, 0)) < 0) {
    cerr << "Receive error (" << errno << "): " << strerror(errno);
    exit(13);
  } else {
    cout << "Message received (" << bytes_xferd << " bytes)" << endl;
    cout << msg << endl;
  }
  
  // send message
  if ((bytes_xferd = send(socketFD, "Thanks! Server-dude!", 21, 0)) < 0) {
    cerr << "Send error (" << errno << "): " << strerror(errno);
    exit(14);
  } else
    cout << "Message sent" << endl;

  // close socketFD
  if (close(socketFD) < 0) {
    cerr << "Closing socketFD error (" << errno << "): " << strerror(errno);
    exit(15);
  } else 
    cerr << "Connection closed" << endl;

  return 0;
}
