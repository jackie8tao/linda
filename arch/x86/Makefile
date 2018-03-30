$(info x86平台部分处理中...)

.PHONY: default all clean

C_OBJS := trap.o
A_OBJS := entry.o vector.o

all: $(C_OBJS) $(A_OBJS)
	@echo "x86平台代码链接中...";
	$(LD) -r -o arch.o $(C_OBJS) $(A_OBJS)

clean: dummy
	@echo "x86平台代码清理中...";
	rm -rf *.d; rm -rf *.o;

entry.o: entry.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;

vector.o: vector.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
