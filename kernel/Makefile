.PHONY: default all clean

C_OBJS := sched.o task.o

all: $(C_OBJS)
	$(LD) -r -o kernel.o $(C_OBJS);

clean: dummy
	rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
