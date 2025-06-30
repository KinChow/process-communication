#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define ZMQ_SERVER_ADDR "ipc:///tmp/zeromq_example.ipc"

static volatile int exit_flag = 0;
void handle_signal(int sig) { exit_flag = 1; }

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    
    // 绑定IPC地址
    if (zmq_bind(socket, ZMQ_SERVER_ADDR) != 0) {
        perror("zmq_bind");
        return 1;
    }

    printf("Server started...\n");
    while (!exit_flag) {
        // 接收消息
        char buffer[256];
        int size = zmq_recv(socket, buffer, 255, 0);
        
        if (size == -1) {
            if (errno == EINTR) continue;
            perror("zmq_recv");
            break;
        }
        
        buffer[size] = '\0';
        printf("Received: %s\n", buffer);
        
        // 处理退出指令
        if (strcmp(buffer, "exit") == 0) {
            printf("Received exit command, shutting down...\n");
            exit_flag = 1;
        }

        // 发送响应
        zmq_send(socket, "OK", 3, 0);
    }

    // 清理资源
    zmq_close(socket);
    zmq_ctx_destroy(context);
    printf("Server stopped.\n");
    return 0;
}