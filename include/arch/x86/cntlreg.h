#ifndef __LINDA_CNTLREG_H
#define __LINDA_CNTLREG_H

// CRO寄存器
#define CRO_PE          1 << 0          // 开启保护模式
#define CRO_MP          1 << 1
#define CRO_EM          1 << 2
#define CRO_TS          1 << 3
#define CRO_ET          1 << 4
#define CRO_NE          1 << 5
#define CRO_WP          1 << 16
#define CRO_AM          1 << 18
#define CRO_NW          1 << 29
#define CRO_CD          1 << 30
#define CRO_PG          1 << 31         // 开启分页机制


// CR4寄存器
#define CR4_VME         1 << 0
#define CR4_PVI         1 << 1
#define CR4_TSD         1 << 2
#define CR4_DE          1 << 3
#define CR4_PSE         1 << 4
#define CR4_PAE         1 << 5
#define CR4_MCE         1 << 6
#define CR4_PGE         1 << 7
#define CR4_PCE         1 << 8
#define CR4_OSFXSR      1 << 9
#define CR4_OSXMMEXCPT  1 << 10
#define CR4_UMIP        1 << 11
#define CR4_LA57        1 << 12
#define CR4_VMXE        1 << 13
#define CR4_SMXE        1 << 14
#define CR4_FSGSBASE    1 << 16
#define CR4_PCIDE       1 << 17
#define CR4_OSXSAVE     1 << 18
#define CR4_SMEP        1 << 20
#define CR4_SMAP        1 << 21
#define CR4_PKE         1 << 22


// EFLAGS寄存器
#define EFLAG_CF        1 << 0
#define EFLAG_PF        1 << 2
#define EFLAG_AF        1 << 4
#define EFLAG_ZF        1 << 6
#define EFLAG_SF        1 << 7
#define EFLAG_TF        1 << 8
#define EFLAG_IF        1 << 9
#define EFLAG_DF        1 << 10
#define EFLAG_OF        1 << 11
#define EFLAG_NT        1 << 14
#define EFLAG_RF        1 << 16
#define EFLAG_VM        1 << 17
#define EFLAG_AC        1 << 18
#define EFLAG_VIF       1 << 19
#define EFLAG_VIP       1 << 20
#define EFLAG_ID        1 << 21

#endif