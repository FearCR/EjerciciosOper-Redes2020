/* Creates a datagram server.  The port
   number is passed as an argument.  This
   server runs forever */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h> //For exit function
#include <string.h> //for bzero
#include <unistd.h> // for write and read functions

void error(char *msg)
{
    perror(msg);
    exit(0);
}

/*
params: port number
*/
int main(int argc, char *argv[])
{
   int sock, length, fromlen, n,pid;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];
   char num[10];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   // socket file descriptor
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   // initiate struct server: bzero sets bytes of the area starting at server to zero (bytes containing '\0').
   bzero(&server,length);
   server.sin_family=AF_INET;//AF_INET: address family that is used to designate the type of addresses that your socket can communicate with (in this case, Internet Protocol v4 addresses).
   server.sin_addr.s_addr=INADDR_ANY;//it allows your server to receive packets destined to any of the interfaces
   server.sin_port=htons(atoi(argv[1]));
   //assigns the address specified by server to the socket referred to by the file descriptor sock.
   if (bind(sock,(struct sockaddr *)&server,length)<0)
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   while (1) {
     bzero(buf,1024);
      // receives data on a socket whether or not it is connection-oriented.
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);

       if (n < 0) error("recvfrom");
       write(1,"Received a datagram: ",21);
       write(1,buf,n);
       write(1," From: ",8);
       write(1,"127.0.0.1\n",10);
       //transmits a message to another socket (same usage as write in a socket file descriptor, the only difference is the presence of flags in sendto)
       n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");

   }
 }
