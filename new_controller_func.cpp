#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <math.h>

#define port 8133
void new_controller_func(double& u, unsigned int k);

int main(int argc, char** argv) {
    int s;
    struct sockaddr_in server_addr;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0) {
        printf("socket error\n");
        return -1;
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    
    double u; unsigned int k = 55;
    new_controller_func(u, k);

    int decpt, sign;
    char* buf = fcvt(u, 4, &decpt, &sign);
    *(++buf) = '\0';
    int status = send(s, buf, 20, 0);
    if(status == -1) perror("send error");

    close(s);
    return 0;
}

void new_controller_func(double& u, unsigned int k) {
    u = cos((double) k / 45.0);
}