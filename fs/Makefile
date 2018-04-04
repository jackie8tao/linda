.PHONY: default all clean

C_OBJS := buf.o

all: $(C_OBJS)
	$(LD) -r -o fs.o $(C_OBJS);

clean: dummy
	rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
