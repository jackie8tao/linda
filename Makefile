# Linda Operating System项目所有

# 包含配置文件
include ./linda.cfg

ROOT := $(PWD)	
ARCH ?= x86	
PREFIX ?= i686-elf

.EXPORT_ALL_VARIABLES:

# gnu编译工具链
AS := $(PREFIX)-as
CC := $(PREFIX)-gcc
LD := $(PREFIX)-ld

