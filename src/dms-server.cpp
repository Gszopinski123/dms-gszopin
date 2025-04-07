#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
char* convertAddress(char ip[], char* newIp);
#define PORT 5551
int main(int argc, char** argv) {
    char ip[10] = "localhost";
    char* newIp = (char*)malloc(128);
    //convertAddress(ip,newIp);
    //printf("%s\n",newIp);
    int port = 5551;
    //inet_aton(newIp, (struct in_addr*)&address.sin_addr.s_addr);
    struct sockaddr_in address { .sin_family=AF_INET, .sin_port=htons(port),  .sin_addr = {.s_addr = INADDR_ANY}};
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    bind(sockfd,((struct sockaddr*)&address) ,sizeof(address));
    listen(sockfd,5);
    int clientfd = accept(sockfd, 0, 0);
    char buf[256];
    recv(clientfd,buf,255,0);
    printf("%s\n",buf);
    return 0;
}

char* convertAddress(char ip[],char* newIp) {
    struct addrinfo hints, *res, *p;
    void* addr;
    char ipv4[128];
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(ip, NULL, &hints, &res);
    p = res;
    while (res) {
        struct sockaddr_in *address = (struct sockaddr_in*)res->ai_addr;
        addr = &(address->sin_addr);
        inet_ntop(res->ai_family, addr, ipv4, sizeof(ipv4));
        res = res->ai_next;
    }
    freeaddrinfo(p);
    for (int i = 0; i != strlen(ipv4); i++) 
        newIp[i] = ipv4[i];
    return newIp;
}
