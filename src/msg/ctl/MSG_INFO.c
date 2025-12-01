#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("测试 MSG_INFO:\n");
    
    // 先创建几个队列
    for (int i = 0; i < 3; i++) {
        int id = msgget(IPC_PRIVATE, 0666);
        printf("创建队列 %d: id=%d\n", i, id);
    }
    
    // 测试 MSG_INFO
    struct msqid_ds buf;
    int count = msgctl(0, MSG_INFO, &buf);
    
    printf("MSG_INFO 返回: %d\n", count);
    printf("buf.msg_qnum: %lu\n", buf.msg_qnum);
    
    if (count < 0) {
        printf("错误: %s (errno=%d)\n", strerror(errno), errno);
    }
    
    return 0;
}
