/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h> //For exit function
#include <string.h> //for strlen
#include <unistd.h> // for write and read functions

void error(char *);

/*
params: host name and port number
*/
int main(int argc, char *argv[])
{
   int sock, length, n;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];

   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   // socket file descriptor
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET; //AF_INET: address family that is used to designate the type of addresses that your socket can communicate with (in this case, Internet Protocol v4 addresses).
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr,
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
while(1){
   printf("Please enter the message: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);
   //transmits a message to another socket (same usage as write in a socket file descriptor, the only difference is the presence of flags in sendto)
   n=sendto(sock,buffer,
            strlen(buffer),0,(struct sockaddr *)&server,length);
   if (n < 0) error("Sendto");
   // receives data on a socket whether or not it is connection-oriented.
   n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
   if (n < 0) error("recvfrom");
   write(1,"Got an ack: ",12);
   write(1,buffer,n);
 }
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}
