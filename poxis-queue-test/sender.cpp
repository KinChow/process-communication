#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>

#define QUEUE_NAME  "/my_queue"
#define MAX_MSG     100
#define MSG_BUFFER_SIZE MAX_MSG + 10

int main() {
    mqd_t mq;
    char buffer[MSG_BUFFER_SIZE];
    
    // 创建/打开消息队列
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, NULL);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    printf("Send to server (enter \"exit\" to stop):\n");
    while (1) {
        printf("> ");
        fgets(buffer, MSG_BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // 移除换行符

        // 发送消息
        if (mq_send(mq, buffer, strlen(buffer)+1, 0) == -1) {
            perror("mq_send");
            break;
        }

        if (strcmp(buffer, "exit") == 0) break;
    }

    mq_close(mq);
    return 0;
}