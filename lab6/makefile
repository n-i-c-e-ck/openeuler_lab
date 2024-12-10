# 指定编译器
CC=gcc

# 编译选项
CFLAGS=-Wall

# 目标可执行文件
TARGET=lab6

# 源文件
SRC=lab6.c

# 默认目标
all: $(TARGET)

# 生成目标
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

# 清理目标
clean:
	rm -f $(TARGET)
