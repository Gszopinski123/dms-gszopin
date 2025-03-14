using namespace std;

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
void * keep_connection(int socketfd);

int main(int argc, char** argv) {
    char ip[16];
    int ipbasic = INADDR_ANY;
    int port = 5551;
    if (argc > 2) {
        if (!strcmp(argv[1],"-i")) {
            strcpy(ip,argv[2]);
            ipbasic = -1;
        }
        printf("%s\n",ip);
    }
    struct sockaddr_in address;
    ipbasic == -1 ? inet_aton(ip, (struct in_addr*)&address.sin_addr.s_addr)  : address.sin_addr.s_addr = ipbasic;
    // address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    int socketfd = socket(AF_INET,SOCK_STREAM,0);
    int connectfd = connect(socketfd,(struct sockaddr*)&address, sizeof(address));
    keep_connection(socketfd);
}

void * keep_connection(int socketfd) {
    char buf[256];
    do  {
        bzero(buf,256);
        cin.getline(buf,255);
        
        int bytes = send(socketfd,buf,255,0);
        if (strlen(buf) == 0) {
            printf("Shuting down!\n");
        } else {
            printf("Message Sent! %s",buf);
            printf("%ld\n",strlen(buf));
        }
        

    } while (strlen(buf) != 0);
    return NULL;
}

