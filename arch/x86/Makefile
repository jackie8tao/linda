.PHONY: default all clean

C_OBJS := trap.o
A_OBJS := entry.o vector.o

all: $(C_OBJS) $(A_OBJS)
	$(LD) -r -o arch.o $(C_OBJS) $(A_OBJS)

clean: dummy
	rm -rf *.d; rm -rf *.o;

entry.o: entry.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;

vector.o: vector.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
