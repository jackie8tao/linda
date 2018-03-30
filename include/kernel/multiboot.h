/*
 * grub引导启动的相关结构信息，
 * 主要用于获取计算机相关信息。
 */

#ifndef _LINDA_MULTIBOOT_H
#define _LINDA_MULTIBOOT_H

#include <types.h>

#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

#define MULTIBOOT_INFO_MEMORY                   (1 << 0)
#define MULTIBOOT_INFO_BOOTDEV                  (1 << 1)
#define MULTIBOOT_INFO_CMDLINE                  (1 << 2)
#define MULTIBOOT_INFO_MODS                     (1 << 3)
#define MULTIBOOT_INFO_AOUT_SYMS                (1 << 4)
#define MULTIBOOT_INFO_ELF_SHDR                 (1 << 5)
#define MULTIBOOT_INFO_MEM_MAP                  (1 << 6)
#define MULTIBOOT_INFO_DRIVE_INFO               (1 << 7)
#define MULTIBOOT_INFO_CONFIG_TABLE             (1 << 8)
#define MULTIBOOT_INFO_BOOT_LOADER_NAME         (1 << 9)
#define MULTIBOOT_INFO_APM_TABLE                (1 << 10)
#define MULTIBOOT_INFO_VBE_INFO                 (1 << 11)
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO         (1 << 12)

struct multiboot_header {
    uint_t magic;
    uint_t flags;
    uint_t checksum;
    uint_t header_addr;
    uint_t load_addr;
    uint_t load_end_addr;
    uint_t bss_end_addr;
    uint_t entry_addr;
    uint_t mode_type;
    uint_t width;
    uint_t height;
    uint_t depth;
};

struct multiboot_aout_symbol_table{
    uint_t tabsize;
    uint_t strsize;
    uint_t addr;
    uint_t reserved;
};

struct multiboot_elf_section_header_table{
    uint_t num;
    uint_t size;
    uint_t addr;
    uint_t shndx;
};

#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED      0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB          1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2

struct multiboot_info {
    uint_t flags;
    uint_t mem_lower;
    uint_t mem_upper;
    uint_t boot_device;
    uint_t cmdline;
    uint_t mods_count;
    uint_t mods_addr;
    union
    {
        struct multiboot_aout_symbol_table aout_sym;
        struct multiboot_elf_section_header_table elf_sec;
    } syms;
    uint_t mmap_length;
    uint_t mmap_addr;
    uint_t drives_length;
    uint_t drives_addr;
    uint_t config_table;
    uint_t boot_loader_name;
    uint_t apm_table;
    uint_t vbe_control_info;
    uint_t vbe_mode_info;
    ushort_t vbe_mode;
    ushort_t vbe_interface_seg;
    ushort_t vbe_interface_off;
    ushort_t vbe_interface_len;
    ulong_t framebuffer_addr;
    uint_t framebuffer_pitch;
    uint_t framebuffer_width;
    uint_t framebuffer_height;
    uchar_t framebuffer_bpp;
    uchar_t framebuffer_type;
    union
    {
        struct
        {
            uint_t framebuffer_palette_addr;
            ushort_t framebuffer_palette_num_colors;
        };
        struct
        {
            uchar_t framebuffer_red_field_position;
            uchar_t framebuffer_red_mask_size;
            uchar_t framebuffer_green_field_position;
            uchar_t framebuffer_green_mask_size;
            uchar_t framebuffer_blue_field_position;
            uchar_t framebuffer_blue_mask_size;
        };
    } color_info;
};

struct multiboot_color {
    uchar_t red;
    uchar_t green;
    uchar_t bl;
} __attribute__((packed));

#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5

struct multiboot_mmap_entry {
    uint_t size;
    uint_t addr_low;
    uint_t addr_high;
    uint_t len_low;
    uint_t len_high;
    uint_t type;
} __attribute__((packed));

struct multiboot_mod_list {
    uint_t mod_start;
    uint_t mod_end;
    uint_t cmdline;
    uint_t pad;
};

struct multiboot_apm_info {
    ushort_t version;
    ushort_t cseg;
    uint_t offset;
    ushort_t cseg_16;
    ushort_t dseg;
    ushort_t flags;
    ushort_t cseg_len;
    ushort_t cseg_16_len;
    ushort_t dseg_len;
};

#endif