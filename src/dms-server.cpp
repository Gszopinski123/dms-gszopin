

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <arpa/inet.h>
#include <sys/socket.h>

void* handle_client(int connectfd);

int main(int argc, char** argv) {
    int port = 5551;
    if (argc > 2) {
        printf("%s\n",argv[1]);
        if (!strcmp(argv[1],"-p")) {
            port = atoi(argv[2]);
        }
    }
    struct sockaddr_in address { .sin_family=AF_INET, .sin_port=htons(port),  .sin_addr = {.s_addr = INADDR_ANY}};
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    bind(sockfd,((struct sockaddr*)&address) ,sizeof(address));
    listen(sockfd,5);
    int clientfd = accept(sockfd,0,0);
    handle_client(clientfd);
}


void* handle_client(int connectfd) {
    char buf[256];
    do {
        bzero(buf,256);
        int bytes = recv(connectfd,buf,255,0);
        if (strlen(buf) == 0) {
            printf("Shuting down!\n");
        } else {
            printf("%s %ld\n",buf,strlen(buf));    
        }
        
        
    } while (strlen(buf) != 0);
    return NULL;
}
