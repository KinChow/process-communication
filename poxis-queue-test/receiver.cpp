#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <cstring>

#define QUEUE_NAME  "/my_queue"
#define MAX_MSG     100
#define MSG_BUFFER_SIZE MAX_MSG + 10

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MSG_BUFFER_SIZE];
    ssize_t bytes_read;

    // 设置队列属性
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG;
    attr.mq_curmsgs = 0;

    // 创建/打开消息队列
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    printf("Waiting for messages...\n");
    while (1) {
        bytes_read = mq_receive(mq, buffer, MSG_BUFFER_SIZE, NULL);
        if (bytes_read == -1) {
            if (errno == EINTR) {  // 添加信号中断处理
                printf("\nExit by user\n");
                break;
            }
            perror("mq_receive");
            break;
        }
        
        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Exit command received\n");
            break;
        }
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    return 0;
}