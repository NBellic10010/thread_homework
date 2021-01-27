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
        perror("socket error");
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
    printf("%f\n", u);

    while(true) {
        int decpt, sign;
        const char* buf = ecvt(u, 1, &decpt, &sign);
        int status = send(s, buf, 20, 0);
        printf(buf);
        if(status == -1) {
            perror("send error");
            close(s);
            return -1;
        }
    }

    close(s);
    return 0;
}

void new_controller_func(double& u, unsigned int k) {
    u = 2.0 * sin((double) k / 45.0);
}