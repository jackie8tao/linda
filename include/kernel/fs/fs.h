// fat32文件系统

#ifndef __LINDA_FS_H
#define __LINDA_FS_H

#include <types.h>

#define BLOCK_SIZE  1024

struct fat32_bpb{
    uchar_t jmpcode[3];
    uchar_t oem_id[8];
    ushort_t bytes_per_sector;
    uchar_t sectors_per_cluster;
    ushort_t reserved_sectors;
    uchar_t fat_no;
    ushort_t root_no;
    ushort_t total_sectors;
    uchar_t media_type;
    ushort_t sectors_per_fat;
    ushort_t sectors_per_track;
    ushort_t head_no;
    uint_t hidden_sectors;
    uint_t large_sectors;
    uchar_t drive_no;
    uchar_t extend_flags;
    uchar_t signature;
    uint_t volume_id;
    char volume_label[11];
    char fat_name[8];
} __attribute__((packed));

struct fat32_dir{
    char name[11];
    char attr;
    char reserved;
    uchar_t create_time_100milliseconds;
    ushort_t create_time;
    ushort_t create_date;
    ushort_t accessed_date;
    ushort_t cluster_no_high;
    ushort_t modified_time;
    ushort_t modified_date;
    ushort_t cluster_no_low;
    uint_t size;
};

struct long_file_name{

};

void fat32_init();

int fat32_fopen();
void fat32_fclose();
int fat32_fread();
int fat32_fwrite();

int fat32_opendir();
void fat32_closedir();
int fat32_readdir();

//fat32_findfirst();
//fat32_findnext();

#endif
