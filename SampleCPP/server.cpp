#include <iostream>     // standard IO
#include <unistd.h>     // close
#include <errno.h>      // errno
#include <string.h>     // strerror
#include <stdlib.h>     // exit
#include <sys/socket.h> // socket stuff
#include <netinet/in.h> // sockaddr_in, INADDR_ANY, htonl|s, ntohl|s
using namespace std;

// program to open a socket, listen for a client to make a connection, 
// then send and receive a mesage
//
// g++ -Wall -Wextra -Wwrite-strings -o server server.cpp
// 
// execute "server" in one window, execute "client" in another window on
// same server; can run both in same window, but difficult to see which
// program is printing, sending, or receiving

int main(void) {
  int listenFD, acceptFD;
  struct sockaddr_in serverAddr, clientAddr;
  int listenPort = 9000;
  socklen_t clientAddr_len;
  char msg[1024] = "Hello, Client!";
  char *msg_ptr = &msg[0];
  int bytes_xferd, bytes_left;

  // create socket FD (PF = Protocol Family)
  if ((listenFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cerr << "Socket creation error (" << errno << "): " << strerror(errno);
    exit(10);
  } else 
    cerr << "Socket created" << endl;

  // create server address (no need for htonl|s on Sun, but makes code portable)
  // By setting the sin_addr.s_addr field to the constant INADDR_ANY, we are
  // requesting the socket be bound (when bind() is called) to all network 
  // interfaces on the host; subsequently, UDP packets and TCP connections from 
  // all interfaces (which match the bound name) are routed to the application. 
  // This becomes important when a server offers a service to multiple networks.
  // By leaving the address unspecified, the server can accept all UDP packets 
  // and TCP connection requests made for its port, regardless of the network 
  // interface on which the requests arrived.
  // For a server, you typically want to bind to all interfaces.
  // To bind socket to localhost only, the syntax would be
  // serverAddr.sin_addr.s_addr = inet_aton("127.0.0.1");
  // AF = Address Family
  memset(&serverAddr, '\0', sizeof(serverAddr));  // clear structure
  serverAddr.sin_family = AF_INET;               // set address domain to IPv4
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);// set address to any (all 0s)
  serverAddr.sin_port = htons(listenPort);       // set port on which to listen

  // from /usr/include/netinet/in.h ...
  // s_addr is type uint32_t (long); sin_port is type uint16_t (short)

  // bind socket to the address (cast to generic)
  // sockaddr is a generic descriptor for any kind of socket operation, whereas 
  // sockaddr_in is a struct specific to IP-based communication
  if ((bind(listenFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0) {
    cerr << "Socket binding error (" << errno << "): " << strerror(errno);
    exit(11);
  } else 
    cerr << "Address bound" << endl;

  // Listen for a connection request from a client; set max queue length to 1
  if ((listen(listenFD, 1)) < 0) {  
    cerr << "Socket listening error (" << errno << "): " << strerror(errno);
    exit(12);
  } else 
    cerr << "Listening" << endl;

  // Accept a connection request (when it comes in); clientAddr_len not used
  if ((acceptFD = accept(listenFD, (struct sockaddr*)&clientAddr, &clientAddr_len)) < 0) {
    cerr << "Connection accept error (" << errno << "): " << strerror(errno);
    exit(13);
  } else 
    cerr << "Connection request accepted" << endl;

  // listenFD no longer needs to be open; can leave open for more connections
  if (close(listenFD) < 0) {
    cerr << "Closing listenFD error (" << errno << "): " << strerror(errno);
    exit(13);
  }

  // send message to client
  bytes_left = strlen(msg)+ 1;  // strlen does not count terminating '\0'
  while (bytes_left > 0) {
    if ((bytes_xferd = send(acceptFD, msg_ptr, bytes_left, 0)) < 0) {
      cerr << "Send error (" << errno << "): " << strerror(errno);
      exit(14);
    } else {
      bytes_left -= bytes_xferd; // subtract bytes xferd to calculate remaining
      msg_ptr += bytes_xferd;    // update pointer for next attempt
      if (bytes_left > 0) 
        cerr << "Send incomplete, sending again" << endl; 
      else 
        cerr << "Message sent" << endl;
    }
  }

  // receive message from client
  memset(msg, '\0', 1024*sizeof(char));  // clear string buffer
  bytes_xferd = recv(acceptFD, msg, 1023, 0);  // receive up to 1023 bytes
  msg[bytes_xferd] = '\0';  // terminate string received
  cout << "Message received (" << bytes_xferd << " bytes)" << endl;
  cout << msg << endl;

  // close acceptFD 
  if (close(acceptFD) < 0) {
    cerr << "Closing acceptFD error (" << errno << "): " << strerror(errno);
    exit(15);
  } else
    cerr << "Connection closed" << endl;

  return 0;
}
