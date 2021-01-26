#include <unistd.h>
#include <string.h>
#include "controller.h"

using namespace std;
typedef unsigned int uint_16;

extern void controller_function(double& u, uint_16 k);

int main() {
    char buf[100];
    cout << "hello" << endl;
    while(1) {
        cin >> buf;
        if(!strcmp(buf, "exit")) break;
        double u; uint_16 k = 55;
        
        controller_function(u, k); 

        //若收到修改程序的指令，则这一段时间内程序新开一个进程来重新编译修改后的程序并装载，然后forK一个新的子进程，在子进程fork成功后
        if(!strcmp(buf, "fixctl")) {
            if(system("g++ -o mainthread mainthread.cpp controller.cpp") < 0) {
                perror("Compile and build failed!");
                exit(127);
            } 

            char* const argv[] = {NULL};
            int pid = execv("/home/hfyh/thread_homework/mainthread", argv);
            if(pid < 0) {
                perror("REPROCESS FAILED\n");
                exit(-1);
            }
        }

        sleep(10);
    }
    return 0;
}