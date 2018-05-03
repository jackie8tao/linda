# 判断选择的平台架构是否存在
EXISTS = $(shell if test -d $(ARCH); then echo "Y"; else echo "N";fi;)
ifneq ($(EXISTS), Y)
$(error "$(ARCH) platform does not exist!")
endif

.PHONY: all clean

all: dummy
	@$(MAKE) -C $(ARCH)

clean: dummy
	@$(MAKE) -C $(ARCH) clean

dummy: # 空依赖且无任何动作
