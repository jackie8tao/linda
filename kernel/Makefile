.PHONY: default all clean

C_OBJS := spinlock.o process.o scheduler.o syscall.o sysapi.o

all: $(C_OBJS)
	$(LD) -r -o kernel.o $(C_OBJS);

clean: dummy
	rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
