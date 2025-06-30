#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 1024

int main() {
    int shm_fd;
    char *ptr;

    // 创建共享内存对象
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    
    // 内存映射
    ptr = (char *)mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    printf("Enter messages (type 'exit' to quit):\n");
    while (1) {
        printf("> ");
        fgets(ptr, SHM_SIZE, stdin);
        ptr[strcspn(ptr, "\n")] = 0; // 移除换行符
        
        if (strcmp(ptr, "exit") == 0) break;
    }

    // 清理资源
    munmap(ptr, SHM_SIZE);
    shm_unlink(SHM_NAME);
    return 0;
}