#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ZMQ_SERVER_ADDR "ipc:///tmp/zeromq_example.ipc"

int main() {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    
    // 连接服务器
    if (zmq_connect(socket, ZMQ_SERVER_ADDR) != 0) {
        perror("zmq_connect");
        return 1;
    }

    printf("Send to server (enter \"exit\" to stop):\n");
    while (1) {
        printf("> ");
        char buffer[256];
        fgets(buffer, 256, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // 保留原处理逻辑
        
        

        // 发送消息（添加空字符结尾）
        if (zmq_send(socket, buffer, strlen(buffer)+1, 0) == -1) {
            perror("zmq_send");
            break;
        }

        if (strcmp(buffer, "exit") == 0) break;

        // 等待响应
        char reply[256];
        zmq_recv(socket, reply, 256, 0);
        
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}