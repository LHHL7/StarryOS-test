#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

// 测试消息队列的系统调用
int main() {
    printf("Testing System V message queues...\n");
    
    // 测试1: 创建消息队列
    int msqid = msgget(IPC_PRIVATE, 0666);
    if (msqid < 0) {
        perror("msgget failed");
        return 1;
    }
    printf("✓ msgget(IPC_PRIVATE) success: msqid=%d\n", msqid);
    
    // 测试2: 控制操作
    struct msqid_ds buf;
    if (msgctl(msqid, IPC_STAT, &buf) < 0) {
        perror("msgctl IPC_STAT failed");
        return 1;
    }
    printf("✓ msgctl IPC_STAT success\n");
    
    // 测试3: 发送消息
    struct {
        long mtype;
        char mtext[32];
    } msg;
    
    msg.mtype = 1;
    strcpy(msg.mtext, "Hello, StarryOS!");
    
    if (msgsnd(msqid, &msg, strlen(msg.mtext), 0) < 0) {
        perror("msgsnd failed");
        return 1;
    }
    printf("✓ msgsnd success\n");
    
    // 测试4: 接收消息
    ssize_t len = msgrcv(msqid, &msg, sizeof(msg.mtext), 0, 0);
    if (len < 0) {
        perror("msgrcv failed");
        return 1;
    }
    printf("✓ msgrcv success: received '%s'\n", msg.mtext);
    
    // 清理
    if (msgctl(msqid, IPC_RMID, NULL) < 0) {
        perror("msgctl IPC_RMID failed");
        return 1;
    }
    printf("✓ msgctl IPC_RMID success\n");
    
    printf("All tests passed! 🎉\n");
    return 0;
}
