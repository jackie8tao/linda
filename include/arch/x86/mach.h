/*
 * 机器相关结构和定义
 *
 * @author Jackie Tao <jackie8tao@gmail.com>
 * @date 2018-04-19 13:32
 */

#ifndef __X86_MACH_H
#define __X86_MACH_H

#include <memlayout.h>

// Eflags register
// @help "https://en.wikipedia.org/wiki/FLAGS_register"
#define FL_CF           1 << 0          // Carry Flag
#define FL_PF           1 << 2          // Parity Flag
#define FL_AF           1 << 4          // Auxiliary carry Flag
#define FL_ZF           1 << 6          // Zero Flag
#define FL_SF           1 << 7          // Sign Flag
#define FL_TF           1 << 8          // Trap Flag
#define FL_IF           1 << 9          // Interrupt Enable
#define FL_DF           1 << 10         // Direction Flag
#define FL_OF           1 << 11         // Overflow Flag
#define FL_IOPL_MASK    0x00003000      // I/O Privilege Level bitmask
#define FL_IOPL_0       0x00000000      //   IOPL == 0
#define FL_IOPL_1       0x00001000      //   IOPL == 1
#define FL_IOPL_2       0x00002000      //   IOPL == 2
#define FL_IOPL_3       0x00003000      //   IOPL == 3
#define FL_NT           1 << 14         // Nested Task
#define FL_RF           1 << 16         // Resume Flag
#define FL_VM           1 << 17         // Virtual 8086 mode
#define FL_AC           1 << 18         // Alignment Check
#define FL_VIF          1 << 19         // Virtual Interrupt Flag
#define FL_VIP          1 << 20         // Virtual Interrupt Pending
#define FL_ID           1 << 21         // ID flag

// Control Register flags
// @help "https://en.wikipedia.org/wiki/Control_register#CR0"
#define CR0_PE          1 << 0          // Protection Enable
#define CR0_MP          1 << 1          // Monitor coProcessor
#define CR0_EM          1 << 2          // Emulation
#define CR0_TS          1 << 3          // Task Switched
#define CR0_ET          1 << 4          // Extension Type
#define CR0_NE          1 << 5          // Numeric Errror
#define CR0_WP          1 << 16         // Write Protect
#define CR0_AM          1 << 18         // Alignment Mask
#define CR0_NW          1 << 29         // Not Writethrough
#define CR0_CD          1 << 30         // Cache Disable
#define CR0_PG          1 << 31         // Paging

// @help "https://en.wikipedia.org/wiki/Control_register#CR4"
#define CR4_VME         1 << 0          // Virtual 8086 Mode Extension
#define CR4_PVI         1 << 1          // Protected-mode Virtual Interrupts
#define CR4_TSD         1 << 2          // Time Stamp Disable
#define CR4_DE          1 << 3          // Debugging Extensions
#define CR4_PSE         1 << 4          // Page size extension
#define CR4_PAE         1 << 5          // Physical Address Extension
#define CR4_MCE         1 << 6          // Machine Check Exception
#define CR4_PGE         1 << 7          // Page Global Enabled
#define CR4_PCE         1 << 8          // Performance-Monitoring Counter enable
#define CR4_OSFXSR      1 << 9          // OS support for FXSAVE and FXRSTOR instructions
#define CR4_OSXMMEXCPT  1 << 10         // OS Support for Unmasked SIMD Floating-Point Exceptions
#define CR4_UMIP        1 << 11         // User-Mode Instruction Prevention
#define CR4_LA57        1 << 12         // 5-Level Paging
#define CR4_VMXE        1 << 13         // Virtual Machine Extensions Enable
#define CR4_SMXE        1 << 14         // Safer Mode Extensions Enable
#define CR4_FSGSBASE    1 << 16         // Enables the instructions RDFSBASE,
                                        // RDGSBASE, WRFSBASE, and WRGSBASE
#define CR4_PCIDE       1 << 17         // PCID Enable
#define CR4_OSXSAVE     1 << 18         // XSAVE and Processor Extended States Enable
#define CR4_SMEP        1 << 20         // Supervisor Mode Execution Protection Enable
#define CR4_SMAP        1 << 21         // Supervisor Mode Access Prevention Enable
#define CR4_PKE         1 << 22         // Protection Key Enable

// various segment selectors.
#define SEG_KCODE 1  // kernel code
#define SEG_KDATA 2  // kernel data+stack
#define SEG_UCODE 3  // user code
#define SEG_UDATA 4  // user data+stack
#define SEG_TSS   5  // this process's task state

// cpu->gdt[NSEGS] holds the above segments.
#define NSEGS     6

// Segment Descriptor
// @help "https://en.wikipedia.org/wiki/Segment_descriptor"
struct segdesc {
  uint_t lim_15_0 : 16;  // Low bits of segment limit
  uint_t base_15_0 : 16; // Low bits of segment base address
  uint_t base_23_16 : 8; // Middle bits of segment base address
  uint_t type : 4;       // Segment type (see STS_ constants)
  uint_t s : 1;          // 0 = system, 1 = application
  uint_t dpl : 2;        // Descriptor Privilege Level
  uint_t p : 1;          // Present
  uint_t lim_19_16 : 4;  // High bits of segment limit
  uint_t avl : 1;        // Unused (available for software use)
  uint_t rsv1 : 1;       // Reserved
  uint_t db : 1;         // 0 = 16-bit segment, 1 = 32-bit segment
  uint_t g : 1;          // Granularity: limit scaled by 4K when set
  uint_t base_31_24 : 8; // High bits of segment base address
};

// Normal segment
#define SEG(type, base, lim, dpl) (struct segdesc)      \
{ ((lim) >> 12) & 0xffff, (uint_t)(base) & 0xffff,      \
  ((uint_t)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (uint_t)(lim) >> 28, 0, 0, 1, 1, (uint_t)(base) >> 24 }

#define SEG16(type, base, lim, dpl) (struct segdesc)    \
{ (lim) & 0xffff, (uint_t)(base) & 0xffff,              \
  ((uint_t)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (uint_t)(lim) >> 16, 0, 0, 1, 0, (uint_t)(base) >> 24 }

#define DPL_KERN    0x0     // Kenel DPL
#define DPL_USER    0x3     // User DPL

// Application segment type bits
#define STA_X       0x8     // Executable segment
#define STA_E       0x4     // Expand down (non-executable segments)
#define STA_C       0x4     // Conforming code segment (executable only)
#define STA_W       0x2     // Writeable (non-executable segments)
#define STA_R       0x2     // Readable (executable segments)
#define STA_A       0x1     // Accessed

// System segment type bits
#define STS_T16A    0x1     // Available 16-bit TSS
#define STS_LDT     0x2     // Local Descriptor Table
#define STS_T16B    0x3     // Busy 16-bit TSS
#define STS_CG16    0x4     // 16-bit Call Gate
#define STS_TG      0x5     // Task Gate / Coum Transmitions
#define STS_IG16    0x6     // 16-bit Interrupt Gate
#define STS_TG16    0x7     // 16-bit Trap Gate
#define STS_T32A    0x9     // Available 32-bit TSS
#define STS_T32B    0xB     // Busy 32-bit TSS
#define STS_CG32    0xC     // 32-bit Call Gate
#define STS_IG32    0xE     // 32-bit Interrupt Gate
#define STS_TG32    0xF     // 32-bit Trap Gate

// A virtual address 'la' has a three-part structure as follows:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/

// page directory index
#define PDX(va)         (((uint_t)(va) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(va)         (((uint_t)(va) >> PTXSHIFT) & 0x3FF)

// construct virtual address from indexes and offset
#define PGADDR(d, t, o) ((uint_t)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// Page directory and page table constants.
#define NPDENTRIES      1024        // # directory entries per page directory
#define NPTENTRIES      1024        // # PTEs per page table
#define PGSIZE          4096        // bytes mapped by a page
#define PGMASK          0xFFFFF000  // page mask used to 4k aligned
#define PTSIZE          4096        // page table memory size
#define PDSIZE          4096        // page directory memory size

#define PGSHIFT         12      // log2(PGSIZE)
#define PTXSHIFT        12      // offset of PTX in a linear address
#define PDXSHIFT        22      // offset of PDX in a linear address

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

// Page table/directory entry flags.
#define PTE_P           1 << 0  // Present
#define PTE_W           1 << 1  // Writeable
#define PTE_U           1 << 2  // User
#define PTE_PWT         1 << 3  // Write-Through
#define PTE_PCD         1 << 4  // Cache-Disable
#define PTE_A           1 << 5  // Accessed
#define PTE_D           1 << 6  // Dirty
#define PTE_PS          1 << 7  // Page Size, 4M page or reference as page table
#define PTE_G           1 << 8  // Global, ignored & useless
#define PTE_MBZ         0x180   // Bits must be zero

// Address in page table or page directory entry
#define PTE_ADDR(pte)   ((uint_t)(pte) & ~0xFFF)
#define PTE_FLAGS(pte)  ((uint_t)(pte) &  0xFFF)

typedef uint_t pte_t;
typedef uint_t pde_t;

// Task state segment format
struct taskstate {
  uint_t link;          // Old ts selector
  uint_t esp0;          // Stack pointers and segment selectors
  ushort_t ss0;         //   after an increase in privilege level
  ushort_t padding1;
  uint_t *esp1;
  ushort_t ss1;
  ushort_t padding2;
  uint_t *esp2;
  ushort_t ss2;
  ushort_t padding3;
  void *cr3;            // Page directory base
  uint_t *eip;          // Saved state from last task switch
  uint_t eflags;
  uint_t eax;           // More saved state (registers)
  uint_t ecx;
  uint_t edx;
  uint_t ebx;
  uint_t *esp;
  uint_t *ebp;
  uint_t esi;
  uint_t edi;
  ushort_t es;          // Even more saved state (segment selectors)
  ushort_t padding4;
  ushort_t cs;
  ushort_t padding5;
  ushort_t ss;
  ushort_t padding6;
  ushort_t ds;
  ushort_t padding7;
  ushort_t fs;
  ushort_t padding8;
  ushort_t gs;
  ushort_t padding9;
  ushort_t ldt;
  ushort_t padding10;
  ushort_t t;           // Trap on task switch
  ushort_t iomb;        // I/O map base address
};

// PAGEBREAK: 12
// Gate descriptors for interrupts and traps
struct gatedesc {
  uint_t off_15_0 : 16;     // low 16 bits of offset in segment
  uint_t cs : 16;           // code segment selector
  uint_t args : 5;          // # args, 0 for interrupt/trap gates
  uint_t rsv1 : 3;          // reserved(should be zero I guess)
  uint_t type : 4;          // type(STS_{TG,IG32,TG32})
  uint_t s : 1;             // must be 0 (system)
  uint_t dpl : 2;           // descriptor(meaning new) privilege level
  uint_t p : 1;             // Present
  uint_t off_31_16 : 16;    // high bits of offset in segment
};

// Set up a normal interrupt/trap gate descriptor.
// - istrap: 1 for a trap (= exception) gate, 0 for an interrupt gate.
//   interrupt gate clears FL_IF, trap gate leaves FL_IF alone
// - sel: Code segment selector for interrupt/trap handler
// - off: Offset in code segment for interrupt/trap handler
// - dpl: Descriptor Privilege Level -
//        the privilege level required for software to invoke
//        this interrupt/trap gate explicitly using an int instruction.
#define SETGATE(gate, istrap, sel, off, d)                  \
{                                                           \
  (gate).off_15_0 = (uint_t)(off) & 0xffff;                 \
  (gate).cs = (sel);                                        \
  (gate).args = 0;                                          \
  (gate).rsv1 = 0;                                          \
  (gate).type = (istrap) ? STS_TG32 : STS_IG32;             \
  (gate).s = 0;                                             \
  (gate).dpl = (d);                                         \
  (gate).p = 1;                                             \
  (gate).off_31_16 = (uint_t)(off) >> 16;                   \
}

// Layout of the trap frame built on the stack by the
// hardware and by trapasm.S, and passed to trap().
struct trapframe {
  // registers as pushed by pusha
  uint_t edi;
  uint_t esi;
  uint_t ebp;
  uint_t oesp;      // useless & ignored
  uint_t ebx;
  uint_t edx;
  uint_t ecx;
  uint_t eax;

  // rest of trap frame
  ushort_t gs;
  ushort_t padding1;
  ushort_t fs;
  ushort_t padding2;
  ushort_t es;
  ushort_t padding3;
  ushort_t ds;
  ushort_t padding4;
  uint_t trapno;

  // below here defined by x86 hardware
  uint_t err;
  uint_t eip;
  ushort_t cs;
  ushort_t padding5;
  uint_t eflags;

  // below here only when crossing rings, such as from user to kernel
  uint_t esp;
  ushort_t ss;
  ushort_t padding6;
};

#endif
