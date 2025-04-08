#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#define PORT "5551"
char* convertAddress(char ip[], char* newIp, char buf[]);
int* handleOptions(int argc, char**argv);
int setNext(int id, int nNodes, char**argv);
int setNext(int id, int Nnodes, char**argv) {
    int set = 1;
    if (atoi(argv[id]) >= atoi(argv[Nnodes])) {
        //printf("The id should never be higher than the number of Nodes!\n");
        //exit(1);
    } else if (atoi(argv[id])+1 == atoi(argv[Nnodes]) && atoi(argv[id]) == 0) {
        set = 0;
    } else if (atoi(argv[id])+1 == atoi(argv[Nnodes])) {
        set = -1;
    } 
    return set;
}
int main(int argc, char** argv) {
    int Nnodes = 4;
    int id = -1; // can change this manually or through command line with -id
    char ip[128] = "dms-gszopin-<.dms-gszopin-svc.default.svc.cluster.local";
    char message[16] = ", Hello From ";
    if (argc < 3) {
        printf("Not Enough Arguments!\n");
        exit(1);
    }
    int* info = handleOptions(argc,argv);
    id = info[1];
    Nnodes = info[0];
    for (int i = 0, j = 0; i+j != strlen(message)+strlen(argv[id]); i++) {
        if (i+j >= strlen(message)) {
            message[i] = argv[id][j];
            j++;
        }
    }
    int set = setNext(id, Nnodes, argv);
    int stringlen = strlen(argv[id]);
    for (int i = 0, j = 0; j+i != 128; i++) {
        if (ip[i] == '<') {
            for (; j != stringlen && j+i != 128; j++){
                if (j == stringlen-1) {
                    ip[i+j] = (char)(((int)argv[id][j]) + set);
                } else {
                    ip[i+j] = argv[id][j];
                }
            }
            if (i + j >= 128) {
                printf("Seg fault!\n");
                exit(1);
            }
        } else {
           ip[i+j] = ip[i+j];
        }
    }
    Nnodes = atoi(argv[Nnodes]); id = atoi(argv[id]);
    if (id == 0) {

    }
    while (1) {
        int saveOriClient = -1;
        int clientfd;
        char buf[256];
        char* newIp = (char*)malloc(128);
        int port = 5551;
        struct sockaddr_in address { .sin_family=AF_INET, .sin_port=htons(port),  .sin_addr = {.s_addr = INADDR_ANY}};
        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        if (sockfd < 0) {
            printf("Socket Failure\n");
        }
        bind(sockfd,((struct sockaddr*)&address) ,sizeof(address));
        listen(sockfd,5);
        if (id == 0) {
            clientfd = accept(sockfd, 0, 0);
            if (clientfd < 0) {
                printf("Accept failure!\n");
            }
            saveOriClient = clientfd;
            recv(clientfd,buf,255,0);
            printf("Starting with from the client: %s\n",buf);
            for (int i = 0, j = 0; i+j != strlen(buf)+strlen(message); i++) {
                if (i+j >= strlen(buf)) {
                    buf[i] = message[j];
                    j++;
                }
            }
            convertAddress(ip,newIp,buf);
        }
        clientfd = accept(sockfd, 0, 0);
        if (clientfd < 0) {
            printf("Accept failure!\n");
        }
        recv(clientfd,buf,255,0);
        if (id != 0) { 
            for (int i = 0, j = 0; i+j != strlen(buf)+strlen(message); i++) {
                if (i+j >= strlen(buf)) {
                    buf[i] = message[j];
                    j++;
                }
            }
            printf("DEBUG 1! %s\n",ip);
            convertAddress(ip,newIp,buf);
        }
        printf("%s\n",buf);
        if (id == 0) {
            printf("Back at the start!\n");
            send(saveOriClient,buf,255,0);
        }
        close(sockfd);
        return 0;
    }
    return 0;
}

char* convertAddress(char ip[],char* newIp,char buf[]) {
    struct addrinfo hints, *res, *p;
    void* addr;
    char ipv4[128];
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(ip, PORT, &hints, &res);
    p = res;
    while (res) {
        struct sockaddr_in *address = (struct sockaddr_in*)res->ai_addr;
        addr = &(address->sin_addr);
        inet_ntop(res->ai_family, addr, ipv4, sizeof(ipv4));
        inet_ntop(res->ai_family, addr, ipv4, sizeof(ipv4));
        int sockfd = socket(res->ai_family, res->ai_socktype,0);
        int connectfd = connect(sockfd, res->ai_addr, res->ai_addrlen);
        send(sockfd,buf,255,0);
        close(sockfd);
        res = res->ai_next;
    }
    freeaddrinfo(p);
    for (int i = 0; i != strlen(ipv4); i++) 
        newIp[i] = ipv4[i];
    return buf;
}
int* handleOptions(int argc, char**argv) {
    int* info = (int*)malloc(2*sizeof(int));
    int Nnodes = 4;
    int id = -1;
    for (int i = 1; i != argc-1; i++) {
        if (!strcmp(argv[i],"-N")) {
            if (!strcmp(argv[i+1],"0")) {
                Nnodes = i+1;
            } else {
                Nnodes = atoi(argv[i+1]);
                if (Nnodes == 0) {
                    printf("NaN\n");    
                    exit(1);
                }
                Nnodes = i+1;
            }
        }
        if (!strcmp(argv[i],"-id")) {
            if (!strcmp(argv[i+1],"0")) {
                id = i+1;
            } else {
                id = atoi(argv[i+1]);
                if (id == 0) {
                    printf("NaN\n");
                    exit(1);
                }
                id = i+1;
            }
        }
    }
    if (id == -1) {
        printf("No id given\n");
        exit(1);
    }
    info[0] = Nnodes;
    info[1] = id;
    return info;
}
