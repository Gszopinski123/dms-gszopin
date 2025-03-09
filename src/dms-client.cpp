#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    int socketfd = socket(AF_INET,SOCK_STREAM,0);
    int connectfd = connect(socketfd,(struct sockaddr*)&address, sizeof(address));
    send(socketfd, "HELLO?\n", 63, 0);
}
