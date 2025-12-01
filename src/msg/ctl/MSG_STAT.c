#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("测试 MSG_STAT:\n");
    printf("MSG_STAT=%d, MSG_INFO=%d\n", MSG_STAT, MSG_INFO);
    // 创建3个队列
    int queues[3];
    for (int i = 0; i < 3; i++) {
        queues[i] = msgget(IPC_PRIVATE, 0666);
        printf("创建队列 %d: id=%d\n", i, queues[i]);
    }
    
    // 先获取队列数量
    struct msqid_ds buf;
    int count = msgctl(0, MSG_INFO, &buf);
    printf("系统有 %d 个队列\n", count);
    
    // 测试每个索引
    for (int i = 0; i < count; i++) {
        struct msqid_ds stat_buf;
        int msqid = msgctl(i, MSG_STAT, &stat_buf);
        
        if (msqid < 0) {
            printf("索引 %d: 失败 - %s (errno=%d)\n", 
                   i, strerror(errno), errno);
        } else {
            printf("索引 %d: 成功 - msqid=%d, uid=%u, mode=%o\n",
                   i, msqid, stat_buf.msg_perm.uid, stat_buf.msg_perm.mode);
        }
    }
    
    // 清理
    for (int i = 0; i < 3; i++) {
        if (queues[i] >= 0) {
            msgctl(queues[i], IPC_RMID, NULL);
        }
    }
    
    return 0;
}
