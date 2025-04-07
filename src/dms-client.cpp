using namespace std;
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#define PORT "5551"
char* convertAddress(char ip[], char* newIp);
int main(int argc, char** argv) {
    char ip[128] = "dms-gszopin-0.dms-gszopin-svc.default.svc.cluster.local";
    char* newIp = (char*)malloc(128);
    printf("%s\n",newIp);
    convertAddress(ip,newIp);
    //inet_aton(newIp, (struct in_addr*)&address.sin_addr.s_addr);
    // struct sockaddr_in address;
    // ipbasic == -1 ? inet_aton(ip, (struct in_addr*)&address.sin_addr.s_addr)  : address.sin_addr.s_addr = ipbasic;
    // // address.sin_addr.s_addr = INADDR_ANY;
    // address.sin_port = htons(port);
    // address.sin_family = AF_INET;
    // int socketfd = socket(AF_INET,SOCK_STREAM,0);
    // int connectfd = connect(socketfd,(struct sockaddr*)&address, sizeof(address));
    // char buf[256] = "Hello there!";
    // send(socketfd, buf, 255, 0);
    return 0;
}

char* convertAddress(char ip[],char* newIp) {
    struct addrinfo hints, *res, *p;
    void* addr;
    char ipv4[128];
    char buf[128] = "Howdy!\n";
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(ip, PORT, &hints, &res);
    p = res;
    while (res) {
        struct sockaddr_in *address = (struct sockaddr_in*)res->ai_addr;
        addr = &(address->sin_addr);
        inet_ntop(res->ai_family, addr, ipv4, sizeof(ipv4));
        int sockfd = socket(res->ai_family, res->ai_socktype,0);
        int connectfd = connect(sockfd, res->ai_addr, res->ai_addrlen);
        send(sockfd,buf,127,0);
        res = res->ai_next;
    }
    freeaddrinfo(p);
    for (int i = 0; i != strlen(ipv4); i++) 
        newIp[i] = ipv4[i];
    return newIp;
}
