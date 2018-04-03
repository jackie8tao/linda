$(info 文件系统代码处理中...)

.PHONY: default all clean

C_OBJS := buf.o

all: $(C_OBJS)
	@echo "文件系统代码链接中...";
	$(LD) -r -o fs.o $(C_OBJS);

clean: dummy
	@echo "文件系统代码清理中...";
	rm -rf *.o;

# 包含全局规则和自动规则
include $(MAKE_RULES) $(OBJS:.o=.d)
