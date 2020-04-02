/* A simple server in the internet domain using TCP
The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> //For exit function
#include <string.h> //for bzero
#include <unistd.h> // for write and read sys calls

void error(char *msg)
{
  perror(msg);
  exit(1);
}

/*
params: port number
*/
int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, clilen, pid;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  // socket file descriptor
  sockfd = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM: indicates TCP socket
  if (sockfd < 0)
  error("ERROR opening socket");
  // initiate struct socketaddr_in: bzero sets bytes of the area starting at serv_addr to zero (bytes containing '\0').
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET; //AF_INET: address family that is used to designate the type of addresses that your socket can communicate with (in this case, Internet Protocol v4 addresses).
  serv_addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY:it allows your server to receive packets destined to any of the interfaces
  serv_addr.sin_port = htons(portno);
  //assigns the address specified by serv_addr to the socket referred to by the file descriptor sockfd.
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
  sizeof(serv_addr)) < 0)
  error("ERROR on binding"); //Ex: port already used
  //marks the socket referred to by sockfd as a passive socket,that is, as a socket that will be used to accept incoming connection requests using accept()
  listen(sockfd,5); //blocking sys call //2nd param: creates a queue of size 5 (for accepting connections)
  clilen = sizeof(cli_addr);
  //creates a new connected socket, and returns a new file descriptor referring to that socket
  while(1){
    newsockfd = accept(sockfd,
      (struct sockaddr *) &cli_addr,
      &clilen);
    pid=fork();
    if(pid!=0){
      do{
        if (newsockfd < 0)
        error("ERROR on accept");
        bzero(buffer,256);
        //reads up to 255 bytes from file descriptor newsockfd into the buffer starting at buffer.
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");//Communication error
        printf("Here is the message: %s\n",buffer);
        //writes buffer to the file referred to by the file descriptor newsockfd.
        n = write(newsockfd,"I got your message ",255);
        if (n < 0) error("ERROR writing to socket");
      }while(buffer[0]!='x')//REVISAR

        break;
    }


    }
    waitpid(pid);
    return 0;
  }
