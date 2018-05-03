.PHONY: default all clean

A_OBJS := processA.o processB.o

all: $(A_OBJS)

processA.o: processA.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;
	$(LD) -N -e start -Ttext 0 -o processA.out $@;
	$(OBJCOPY) -S -O binary processA.out processA;
	$(LD) -r -o processA.bin -b binary processA;

processB.o: processB.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;
	$(LD) -N -e start -Ttext 0 -o processB.out $@;
	$(OBJCOPY) -S -O binary processB.out processB;
	$(LD) -r -o processB.bin -b binary processB;

processC.o: processC.S
	$(AS) $(INCLUDE_FLAGS) -o $@ $<;
	$(LD) -N -e start -Ttext 0 -o processC.out $@;
	$(OBJCOPY) -S -O binary processC.out processC;
	$(LD) -r -o processC.bin -b binary processC;

clean: dummy
	rm -rf *.d; rm -rf *.o; rm -rf processA; rm -rf processB;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
