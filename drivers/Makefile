$(info 驱动部分处理中...)

.PHONY: default all clean

C_OBJS := console.o pic.o timer.o

all: $(C_OBJS) $(A_OBJS)
	@echo "驱动代码链接中...";
	$(LD) -r -o drivers.o $(C_OBJS) $(A_OBJS)

clean: dummy
	@echo "驱动代码清理中...";
	rm -rf *.d; rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)