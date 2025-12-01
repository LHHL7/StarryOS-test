#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

int main() {
    printf("=== 简单消息队列发送/接收测试开始 ===\n");
    
    // 1. 创建队列
    int msqid = msgget(IPC_PRIVATE, 0666);
    if (msqid < 0) {
        perror("msgget失败");
        return 1;
    }
    printf("1. 创建队列成功: msqid = %d\n", msqid);
    
    // 2. 发送第一条消息
    struct {
        long mtype;
        char mtext[100];
    } msg;
    
    msg.mtype = 1;
    strcpy(msg.mtext, "第一条测试消息");
    
    printf("2. 发送第一条消息 (type=1):\n");
    if (msgsnd(msqid, &msg, strlen(msg.mtext), 0) < 0) {
        perror("   msgsnd失败");
    } else {
        printf("   发送成功: '%s'\n", msg.mtext);
    }
    
    // 3. 发送第二条消息
    msg.mtype = 2;
    strcpy(msg.mtext, "第二条测试消息");
    
    printf("3. 发送第二条消息 (type=2):\n");
    if (msgsnd(msqid, &msg, strlen(msg.mtext), 0) < 0) {
        perror("   msgsnd失败");
    } else {
        printf("   发送成功: '%s'\n", msg.mtext);
    }
    // 4. 发送第三条消息
    msg.mtype = 3;
    strcpy(msg.mtext, "第三条测试消息");
    
    printf("4. 发送第三条消息 (type=3):\n");
    if (msgsnd(msqid, &msg, strlen(msg.mtext), 0) < 0) {
        perror("   msgsnd失败");
    } else {
        printf("   发送成功: '%s'\n", msg.mtext);
    }
    
    // 5. 查看当前状态
    struct msqid_ds buf;
    if (msgctl(msqid, IPC_STAT, &buf) < 0) {
        perror("msgctl IPC_STAT失败");
    } else {
        printf("5. 当前队列状态:\n");
        printf("   消息数量: %lu\n", buf.msg_qnum);
        printf("   字节总数: %lu\n", buf.msg_cbytes);
        printf("   权限模式: %o\n", buf.msg_perm.mode);
        printf("   所有者UID: %u\n", buf.msg_perm.uid);
    }
    
    // 6. 接收特定类型的消息 (type=1)
    printf("6. 接收type=1的消息:\n");
    struct {
        long mtype;      // 预留 mtype 的位置
        char mtext[100]; // 数据部分
    } rcv_msg;
    ssize_t size = msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), 1, 0);
    if (size < 0) {
        perror("   msgrcv失败");
    } else {
        // 注意：rcv_buf的前4/8字节是mtype，后面才是数据
        printf("   接收成功\n");
        printf("   消息类型: %ld\n", rcv_msg.mtype);
        printf("   数据大小: %ld字节\n", size);
        printf("   内容: '%.*s'\n", (int)size, rcv_msg.mtext);
    }
    
    // 7. 再次查看状态
    if (msgctl(msqid, IPC_STAT, &buf) < 0) {
        perror("msgctl IPC_STAT失败");
    } else {
        printf("7. 接收一条后队列状态:\n");
        printf("   剩余消息数: %lu\n", buf.msg_qnum);
        printf("   剩余字节数: %lu\n", buf.msg_cbytes);
    }
    
    // 8. 接收剩余消息 (type=0表示接收任何类型)
    printf("8. 接收另一条消息 (type=0):\n");
    size = msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), 0, 0);
    if (size < 0) {
        perror("   msgrcv失败");
    } else {
        printf("   接收成功\n");
        printf("   消息类型: %ld\n", rcv_msg.mtype);
        printf("   数据大小: %ld字节\n", size);
        printf("   内容: '%.*s'\n", (int)size, rcv_msg.mtext);
    }
    
    // 9. 最终状态
    if (msgctl(msqid, IPC_STAT, &buf) < 0) {
        perror("msgctl IPC_STAT失败");
    } else {
        printf("9. 最终队列状态:\n");
        printf("   消息数量: %lu\n", buf.msg_qnum);
        printf("   字节总数: %lu\n", buf.msg_cbytes);
    }
    
    printf("\n=== 测试完成 ===\n");
    printf("队列 %d 仍然存在，请使用 ipcs -q 查看\n", msqid);
    printf("使用 ipcrm -q %d 删除队列\n", msqid);
    printf("=== 简单消息队列发送/接收测试结束 ===\n");
    return 0;
}
