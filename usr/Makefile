# 判断选择的平台架构是否存在
SUBDIR := bin
EXISTS = $(shell if test -d $(SUBDIR); then echo "Y"; else echo "N";fi;)
ifneq ($(EXISTS), Y)
$(error "$(SUBDIR) does not exist!")
endif

.PHONY: all clean

all: dummy
	@$(MAKE) -C $(SUBDIR)

clean: dummy
	@$(MAKE) -C $(SUBDIR) clean

dummy: # 空依赖且无任何动作
