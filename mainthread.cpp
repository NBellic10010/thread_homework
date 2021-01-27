#include <unistd.h>
#include <string.h>
#include "controller.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define port 8133

using namespace std;
typedef unsigned int uint_16;
typedef struct sockaddr_in sockad;

extern void controller_function(double& u, uint_16 k);

int main() {
    char buf[100];
    int s_client, s_server;
    sockad server_addr, client_addr;
    double u; uint_16 k = 55;
    controller_function(u, k);

    s_server = socket(AF_INET, SOCK_STREAM, 0);
    if(s_server < 0) {
        printf("socket error");
        return -1;
    } 
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int on = 1;
    if((setsockopt(s_server, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on))) < 0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    int err = bind(s_server, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err < 0) {
        perror("Bind error");
        return -1;
    }

    err = listen(s_server, 2);
    if(err < 0) {
        perror("Listen error");
        return -1;
    }

    while(true) {
        printf("u = %f, k = %d\n", u, k);
        socklen_t addrlen = sizeof(struct sockaddr);
        
        s_client = 1;
        s_client = accept(s_server, (struct sockaddr*)&client_addr, &addrlen);
        
        if(!s_client) continue;
        int pid = fork();

        ssize_t size = 0;
        size = recv(s_client, buf, 100, 0);
        if(size == -1) {
            perror("recv error");
            continue;
        }
        u = atoi(buf);

    }
    return 0;
}