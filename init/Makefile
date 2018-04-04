# 内核初始化部分
C_OBJS := bootstrap.o

.PHONY: default all clean

all: $(C_OBJS)
	$(LD) -r -o init.o $(C_OBJS)

clean: dummy
	rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
