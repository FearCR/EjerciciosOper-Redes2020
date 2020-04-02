
// C program to implement one side of FIFO
// This side reads first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pid;
    int fd1;

    // FIFO file path
    char * myfifo = "/home/alejandro/Desktop/pipe/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    char str1[1024];

    pid=fork();

    if(pid!=0){
      while (1)
      {
          // First open in read only and read
          fd1 = open(myfifo,O_RDONLY);
          read(fd1, str1, 1024);
          // Print the read string and close
          printf("User1: %s\n", str1);

          close(fd1);


      }
    }else{
      execlp("python3", "python3","writer.py",NULL);

    }
return 0;
}
