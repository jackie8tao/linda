$(info 内存管理代码处理中...)

.PHONY: default all clean

C_OBJS := memory.o mmap.o kmalloc.o vm.o

all: $(C_OBJS)
	@echo "内存管理部分链接中...";
	$(LD) -r -o mm.o $(C_OBJS);

clean: dummy
	@echo "内存管理部分清理中...";
	rm -rf *.o

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
