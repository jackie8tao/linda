#ifndef __LINDA_ELF_H
#define __LINDA_ELF_H

#include <types.h>

#define ELF_MAGIC       0x464C457F      // elf文件魔数

// elf file header
struct elf_hdr {
    uint_t      magic;
    uchar_t     ident[12];
    ushort_t    type;
    ushort_t    machine;
    uint_t      version;
    uint_t      entry;
    uint_t      phoff;
    uint_t      shoff;
    uint_t      flags;
    ushort_t    ehsize;
    ushort_t    phentsize;
    ushort_t    phnum;
    ushort_t    shentsize;
    ushort_t    shnum;
    ushort_t    shstrndx;
};

// program section header
struct prog_hdr {
    uint_t      type;
    uint_t      offset;
    uint_t      vaddr;
    uint_t      paddr;
    uint_t      filesz;
    uint_t      memsz;
    uint_t      flags;
    uint_t      align;

};

//
struct sect_hdr {
    uint_t      name;
    uint_t      type;
    uint_t      flags;
    uint_t      addr;
    uint_t      offset;
    uint_t      size;
    uint_t      link;
    uint_t      info;
    uint_t      addralign;
    uint_t      entsize;
};

#endif
