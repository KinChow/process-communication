#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 1024


int main() {
    int shm_fd;
    char *ptr;

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    // 添加共享内存打开校验
    if (shm_fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        close(shm_fd);
        return 1;
    }

    ptr = (char *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    // 添加内存映射校验
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        return 1;
    }

    memset(ptr, 0, SHM_SIZE);

    printf("Waiting for messages...\n");
    while (1) {
        // 添加安全校验防止野指针
        if (ptr && *ptr != '\0') {  
            // 添加字符串长度限制校验
            char buffer[SHM_SIZE] = {0};
            strncpy(buffer, ptr, SHM_SIZE-1);
            
            printf("Received: %s\n", buffer);
            
            if (strcmp(buffer, "exit") == 0) {
                memset(ptr, 0, SHM_SIZE);
                break;
            }
            
            memset(ptr, 0, SHM_SIZE);
        }
        sleep(1);
    }

    printf("Cleaning resources...\n");
    // 调整释放顺序
    if (ptr != MAP_FAILED) {
        munmap(ptr, SHM_SIZE);
    }
    if (shm_fd != -1) {
        close(shm_fd);
    }
    return 0;
}