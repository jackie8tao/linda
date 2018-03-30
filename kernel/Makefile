$(info 内核代码处理中...)

.PHONY: default all clean

C_OBJS := sched.o task.o

all: $(C_OBJS)
	@echo "内核代码链接中...";
	$(LD) -r -o kernel.o $(C_OBJS);

clean: dummy
	@echo "内核代码清理中...";
	rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)