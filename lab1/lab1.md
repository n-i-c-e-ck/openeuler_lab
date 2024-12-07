# 1. 访问所有任务（进程）

### （1）了解openeuler源代码中关于进程部分
1.输入`ls /proc`，查看proc目录内的文件：
![image](https://github.com/n-i-c-e-ck/openeuler_lab/blob/main/4f92e619edca94eeb64b44a6f2365290.png)

因此我们的任务就是解析proc目录，打印进程名称。
### （2）编写C语言程序代码
逐行分析如下：
```lab1.c
#include <stdio.h>      // 标准输入输出库，包含 printf 等函数
#include <stdlib.h>     // 标准库，包含一些常用的函数，如 atoi, exit 等
#include <dirent.h>     // 目录操作库，包含 opendir, readdir, closedir 等函数
#include <string.h>     // 字符串操作库，包含 snprintf, strcspn 等函数
#include <ctype.h>      // 字符类型判断库，包含 isdigit 函数
#include <errno.h>     // 错误处理库，包含 perror 函数


int main() {
    DIR *proc = opendir("/proc");  // 打开 /proc 目录，这是一个虚拟文件系统，包含了系统中所有进程的信息。proc 是一个 DIR* 指针，指向打开的目录
    struct dirent *entry;         // 定义一个 struct dirent 指针，readdir 函数会将读取到的目录项信息存储到这个结构体中

    // 检查 opendir 函数的返回值，如果返回 NULL，则表示打开目录失败。
    if (proc == NULL) {
        perror("opendir(/proc) 失败");  // 使用 perror 函数打印错误信息，包括错误码
        return EXIT_FAILURE;          // 返回 EXIT_FAILURE 表示程序执行失败
    }

    // 循环遍历 /proc 目录下的所有条目
    while ((entry = readdir(proc)) != NULL) {  // readdir 函数读取目录中的下一个条目，直到返回 NULL 表示到达目录结尾
        // 检查条目名称的第一个字符是否为数字，因为进程 ID 通常都是数字
        if (isdigit(*entry->d_name)) {  // isdigit 检查字符是否为数字
            char path[256], comm[256]; // 定义两个字符数组，分别用于存储进程信息的路径和进程名称。256 是足够大的缓冲区大小
            snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name); // 使用 snprintf 函数构造进程名称文件的路径。例如，如果 entry->d_name 为 "1234"，则 path 将为 "/proc/1234/comm"

            FILE *fp = fopen(path, "r"); // 打开进程名称文件，以只读模式 ("r") 打开
            if (fp == NULL) {             // 检查 fopen 函数的返回值，如果返回 NULL，则表示打开文件失败
                perror("fopen 失败");    // 打印错误信息
                continue;                 // 跳过当前条目，继续处理下一个条目
            }

            // 从文件中读取进程名称，最多读取 comm 数组大小 - 1 个字符（留一个空间用于 '\0'）
            if (fgets(comm, sizeof(comm), fp) != NULL) {
                comm[strcspn(comm, "\n")] = 0; // 移除 fgets 读取的换行符 '\n'
                printf("%s\n", comm);     // 打印进程名称
            } else {
                perror("fgets 失败");    // 打印错误信息
            }
            fclose(fp); // 关闭文件
        }
    }
    closedir(proc); // 关闭 /proc 目录
    return EXIT_SUCCESS; // 返回 EXIT_SUCCESS 表示程序执行成功
}
```
### （3）创建Makefile文档以编译程序
1. 在`Makefile`中设置编译器变量`CC`为`gcc`，传递给编译器的选项变量`CFLAGS`为`-Wall`.
2. 然后定义编译规则如下：
```makefile
CC=gcc
CFLAGS=-Wall

all: lab1

lab1: lab1.c
	$(CC) $(CFLAGS) lab1.c -o lab1
clean:
	rm -f lab1

```

### （4）运行和测试
1. 编译程序并在终端中运行，输入`make`。
2. 测试无参数情况，终端输入`./lab1`，成功输出：
![image](https://github.com/n-i-c-e-ck/openeuler_lab/blob/main/lab1.png)

