.PHONY: default all clean

C_OBJS := console.o pic.o timer.o ide.o kbd.o

all: $(C_OBJS) $(A_OBJS)
	$(LD) -r -o drivers.o $(C_OBJS) $(A_OBJS)

clean: dummy
	rm -rf *.d; rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
