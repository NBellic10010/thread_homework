#include <unistd.h>
#include <string.h>
#include "controller.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sstream>
#include "head.h"

/**
 * 程序主要思想：
 * 当程序主函数的s_client接到新控制程序jack-in的信号（socket方式）的时候，执行fork函数，子进程会循环原控制过程
 * 若accept成功则杀掉子进程，接着重新编译mainthread，将函数指针替换为已编译成库的新控制函数（编译过程在new_controller_func_main中）
 * 循环上述过程
 **/

#define port 8133

using namespace std;
typedef unsigned int uint_16;
typedef struct sockaddr_in sockad;

//旧函数指针和新函数指针，用于切换控制函数
void (*fold)(double& u, uint_16 k);
void (*fnew)(double& u, uint_16 k);

//控制函数的声明
extern void controller_function(double& u, uint_16 k);

int main() {
    //以下是创建socket过程
    int s_client, s_server;
    sockad server_addr, client_addr;
    double u; uint_16 k = 55;
    fold = controller_function;

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
    //以上是创建socket过程

    //spid为-1是表示只有一个进程，没有子进程
    pid_t spid = -1;
    while(true) {
        //当只有一个进程时，执行fork
        if(spid == -1) spid = fork();
        socklen_t addrlen = sizeof(struct sockaddr);
        //原控制函数
        fold(u, k);
        printf("u = %f, k = %d\n", u, k++);
        
        s_client = 1;
        
        if(spid == 0) {
            sleep(5);
            continue;
        }
        //accept函数，由于accept函数是阻塞的，因此我们用两个进程，父进程等待accept,子进程继续控制过程
        s_client = accept(s_server, (struct sockaddr*)&client_addr, &addrlen);
        //若accept出问题，则抛出错误提示并继续控制过程
        if(!s_client) {
            perror("accept error");
            continue;
        }
        
        //若accept成功，杀掉子进程，开始控制函数的切换
        int status = kill(spid, SIGKILL);
        if(status == -1) {
            perror("kill failed\n");
            return -3;
        }
        
        //重新编译，为的是将新控制函数的库装载进来
        system("g++ -o mainthread mainthread.cpp controller.cpp -L libraries -lnewcontroller");
        char* const* v = {NULL};
        //execvp使进程重生
        execvp("mainthread.cpp", v);

        double lambda, uout;
        //使fnew指向新的控制函数
        fnew = new_controller_func;
        for(lambda = 0.00; lambda <= 1.00; lambda += 0.05) {
            double unew;
            fold(u, k);
            fnew(unew, k);
            printf("inside u = %f, k = %d, pid=%d\n", uout, k, getpid());
            uout = lambda * unew + (1 - lambda) * u;
            sleep(5);
        }
        spid = -1;
        //使fold指向fnew，将控制函数更新
        fold = fnew;
    }
        
    return 0;
}