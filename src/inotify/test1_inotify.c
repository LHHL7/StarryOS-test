#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//测试系统调用接口
int main() {
     printf("IN_NONBLOCK : %d\n",IN_NONBLOCK);
     printf("IN_CLOEXEC  : %d\n\n",IN_CLOEXEC);
     // 测试所有标志组合
     struct {
         int flags;
         const char *name;
     } tests[] = {
         {0, "无标志"},
         {IN_NONBLOCK, "IN_NONBLOCK"},
         {IN_CLOEXEC, "IN_CLOEXEC"},
         {IN_NONBLOCK | IN_CLOEXEC, "IN_NONBLOCK|IN_CLOEXEC"},
         {0xFFFFFFFF, "无效标志 (应该失败)"},
     };
     
     for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
         printf("测试: %s (flags=0x%08x)... ", tests[i].name, tests[i].flags);
         
         int fd = inotify_init1(tests[i].flags);
         
         if (fd >= 0) {
             printf("成功 fd=%d\n", fd);
             
             // 如果是有效实例，测试基本功能
             if (tests[i].flags != 0xFFFFFFFF) {
                 int wd = inotify_add_watch(fd, ".", IN_CREATE);
                 if (wd >= 0) {
                     printf("  添加监控: wd=%d\n", wd);
                     inotify_rm_watch(fd, wd);
                     printf("  移除监控: 成功\n");
                 }
             }
             
             close(fd);
         } else {
             printf("失败: %s (errno=%d)\n", strerror(errno), errno);
             
             // 检查是否是预期错误
             if (tests[i].flags == 0xFFFFFFFF && errno == EINVAL) {
                 printf("  ✓ 正确拒绝了无效标志\n");
             }
         }
         
         printf("\n");
     }
     
     return 0;
}
