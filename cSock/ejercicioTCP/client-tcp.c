#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h> //For exit function
#include <string.h> //for strlen
#include <unistd.h> // for write and read functions

void error(char *msg)
{
    perror(msg);
    exit(0);
}

/*
params: host name and port number
*/
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    // socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
     // initiate struct socketaddr_in: bzero sets bytes of the area starting at serv_addr to zero (bytes containing '\0').
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //AF_INET: address family that is used to designate the type of addresses that your socket can communicate with (in this case, Internet Protocol v4 addresses).
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    do{
      printf("Please enter the message: ");
      bzero(buffer,256);
      fgets(buffer,255,stdin);
      //writes buffer to the file referred to by the file descriptor socketfd.
      n = write(sockfd,buffer,strlen(buffer));
      if (n < 0)
           error("ERROR writing to socket");
      bzero(buffer,256);
      //reads up to 255 bytes from file descriptor socketfd into the buffer starting at buffer.
      n = read(sockfd,buffer,255);
      if (n < 0)
           error("ERROR reading from socket");
      printf("%s\n",buffer);
    }while(buffer[0]!='x');//REVISAR
    close(sockfd);

    return 0;
}
