# 内核初始化部分
$(info 内核初始化部分处理中...)
C_OBJS := bootstrap.o

.PHONY: default all clean

all: $(C_OBJS)
	@echo "初始化代码链接中...";
	$(LD) -r -o init.o $(C_OBJS)

clean: dummy
	@echo "清理初始化代码中..."
	rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)