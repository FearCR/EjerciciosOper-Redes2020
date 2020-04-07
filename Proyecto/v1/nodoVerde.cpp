#include <iostream>//cin/cout
#include <sys/socket.h>//socket
#include <unistd.h>//fork read write
#include <string.h>//string
#include <arpa/inet.h>//serverAddr clientAddr inet_ntoa etc
#include <stdio.h>//fopen fprint etc
#include <sys/wait.h>//waitpid
//#include <netinet/in.h>
//#include <netdb.h>
//#include <sys/types.h>//u_char u_long..

using namespace std;

#define SERVER_PORT htons(2302)
#define BUFF_SIZE 1024

string toSt(char *a){//funcion que pasa de array de caracteres a string
  string s(a);
  return s;
}

int main() {

  char buffer[BUFF_SIZE];//buffer para mensajes recibidos por socket
  string ipSend="127.0.0.1";//string para guardar la IP de la maquina a la que se quiere enviar un msj(udp)
  string portSend;//string para guardar el Puertp de la maquina a la que se quiere enviar un msj(udp)
  int n;//control de errores
  int pid;//id del proceso (padre/hijo)

  //definicion de socket TCP
  int serverSock=socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = SERVER_PORT;
  serverAddr.sin_addr.s_addr = INADDR_ANY;


  //------------------------------PIPE A NODO NARANJA(solo una vez)------------------------------
  /*se necesita recibir los datos de sus vecinos y su propia IP/puerto, y guardarlos en las
  variables correspondientes.
  Se ejecuta por medio de un exec el nodo naranja, no necesita fork ya que lee los datos y no se
  vuelve a usar*/
  //------------------------------FIN DE PIPE A NODO NARANJA-------------------------------------


  //------------------------------SOCKET TCP A NODO AZUl(constante)------------------------------
  /*se necesita memoria compartida, para guardar el mensaje,  la ip y puerto al
  que se envia el mensaje, ya que funciona de manera concurrente con el envio de mensajes por
  UDP*/

  pid=fork();
  if(pid>0){//proceso Padre pid=ID del proceso hijo
    if((bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)))<0){//se hace el bind del socket
      cout<<"error en el bind "<<endl;
      exit(0);
    }

    listen(serverSock,1);
    bzero(buffer, BUFF_SIZE);//se limpia el buffer
    sockaddr_in clientAddr;
    socklen_t sin_size=sizeof(struct sockaddr_in);
    int clientSock=accept(serverSock,(struct sockaddr*)&clientAddr, &sin_size);//se acepta la conceccion del nodo azul
    if(clientSock<0){
      cout<<"error en el accept "<<endl;
      exit(0);
    }
    cout<<"Se ha conectado el cliente: "<<inet_ntoa(clientAddr.sin_addr)<<endl<<endl;
    //ciclo en el que se reciben mensajes constantemente, no necesita conecciones multiples
    while (1) {
      //Recibe un puerto y la guarda en variable
      if((n = read(clientSock, buffer, BUFF_SIZE))<0){
        cout<<"error en el Read"<<endl;
        exit(0);
      }
      portSend=toSt(buffer);
      bzero(buffer, BUFF_SIZE);
      //Recibe el mensaje a enviar
      if((n = read(clientSock, buffer, BUFF_SIZE))<0){
        cout<<"error en el Read"<<endl;
        exit(0);
      }
      cout << "El servidor recibio:  " << buffer << endl;
      //TODO:programar el envio por UDP, o algun tipo de señal al proceso(signal si se  usan semaforos)
      //para que envie el mensaje usando la memoria compartida de lo que se guarda en este proceso
      cout << "Se debe intentar enviar a la ip " << ipSend <<" Puerto: "<<portSend<<endl;
      cout<< "Enviando mensaje de reibido..."<<endl;

      strncpy(buffer, "Mensaje Recibido y enviado", BUFF_SIZE);//se copia este mensaje en el buffer para enviar de vuelta
      if((n = write(clientSock, buffer, strlen(buffer)))<0){//se envia el mensaje de recibidp al cliente(nodo azul)
        cout<<"error en el mensaje de recibido "<<endl;
      }
      bzero(buffer, BUFF_SIZE);//se limpia el buffer para continuar con el ciclo
      cout<< "Listo!"<<endl<<endl;
    }//fin de while
  }

  //------------------------------FIN DE SOCKET TCP A NODO AZUl------------------------------


  //------------------------------SOCKET UDP A OTRO NODO VERDE-------------------------------
  /*se necesita memoria compartida, para enviar el mensaje, y para conocer la ip y puerto al
  que se envia el mensaje*/
  else{//proceso Hijo pid=0
    waitpid(pid,NULL,WNOHANG);
  }
  //------------------------------FIN DE SOCKET UDP A OTRO NODO VERDE------------------------
  return 0;
}
