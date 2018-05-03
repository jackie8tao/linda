.PHONY: default all clean

C_OBJS := trap.o
A_OBJS := entry.o vectors.o switch.o trapasm.o

all: $(C_OBJS) $(A_OBJS)
	$(LD) -r -o arch.o $(C_OBJS) $(A_OBJS)

clean: dummy
	rm -rf *.d; rm -rf *.o; rm -rf vectors.S;

entry.o: entry.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;

switch.o: switch.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;

trapasm.o: trapasm.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;

vectors.o: vectors.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;

vectors.S: vectors.py
	python $< > vectors.S

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
