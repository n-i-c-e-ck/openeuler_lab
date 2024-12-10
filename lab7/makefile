# 内核源码路径
KERNEL_DIR := /lib/modules/$(shell uname -r)/build

# 当前目录
PWD := $(shell pwd)

# 目标模块
obj-m := list_processes.o

# 默认目标：编译模块
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

# 清理生成文件
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
