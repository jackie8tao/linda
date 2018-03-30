$(info 库文件代码处理中...)

.PHONY: default all clean

C_OBJS := ctypes.o kprintf.o string.o

all: $(C_OBJS)
	@echo "库文件链接中...";
	$(LD) -r -o lib.o $(C_OBJS);

clean: dummy
	@echo "库文件删除中...";
	rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)