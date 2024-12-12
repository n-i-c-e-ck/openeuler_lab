#include <stdio.h>      
#include <stdlib.h>     
#include <dirent.h>     
#include <string.h>     
#include <ctype.h>      
#include <errno.h>     


int main() {
    DIR *proc = opendir("/proc"); // 打开 /proc 目录，该目录包含系统中所有进程的信息
    struct dirent *entry;        

    // 检查 opendir 是否成功
    if (proc == NULL) {
        perror("opendir(/proc) 失败"); 
        return EXIT_FAILURE;          
    }

    // 循环遍历 /proc 目录下的所有条目
    while ((entry = readdir(proc)) != NULL) {
        if (isdigit(*entry->d_name)) {
            char path[256], comm[256]; // 定义两个字符数组，分别用于存储进程信息的路径和进程名称
            snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);

            FILE *fp = fopen(path, "r");
            if (fp == NULL) {
                perror("fopen 失败");
                continue; 
            }

          
            if (fgets(comm, sizeof(comm), fp) != NULL) {
                comm[strcspn(comm, "\n")] = 0;
                printf("%s\n", comm); // 打印进程名称
            } else {
                perror("fgets 失败"); 
            }
            fclose(fp); 
        }
    }
    closedir(proc); 
    return EXIT_SUCCESS; 
}
