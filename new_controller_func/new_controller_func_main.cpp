#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include "new_controller_func.h"

#define port 8133
using namespace std;

int main(int argc, char** argv) {
    int s;
    struct sockaddr_in server_addr;
    cout << "hello" << endl;

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

    int status = connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    
    double u; unsigned int k = 55;
    if(!status) {
        system("g++ -c new_controller_func.cpp");
        system("ar -crv ../libraries/libnewcontroller.a new_controller_func.o ");
    }
    
    close(s);
    return 0;
}

