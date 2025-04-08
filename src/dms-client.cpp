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
    convertAddress(ip,newIp);
    return 0;
}

char* convertAddress(char ip[],char* newIp) {
    struct addrinfo hints, *res, *p;
    void* addr;
    char ipv4[128];
    char buf[128] = "Howdy!";
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
        if (sockfd < 0) {
            printf("Socket Failure!\n");
        }
        int connectfd = connect(sockfd, res->ai_addr, res->ai_addrlen);
        if (connectfd < 0) {
            printf("Connection Failure!\n");
        }
        send(sockfd,buf,127,0);
        printf("%s\n",buf);
        recv(sockfd,buf,127,0);
        printf("%s\n",buf);
        res = res->ai_next;
    }
    freeaddrinfo(p);
    for (int i = 0; i != strlen(ipv4); i++) 
        newIp[i] = ipv4[i];
    return newIp;
}
