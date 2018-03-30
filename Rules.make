# 自动生成依赖的模式规则
%.d: %.c
	set -e; rm -f $@; \
    gcc $(INCLUDES) -MM $< > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
    rm -f $@.$$$$$

# 目标文件生成规则
%.o: %.c
	$(CC) $(INCLUDE_FLAGS) $(C_FLAGS) -c -o $@ $<

# 无任何依赖
dummy: