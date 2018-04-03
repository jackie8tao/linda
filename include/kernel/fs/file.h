#ifndef __LINDA_FILE_H
#define __LINDA_FILE_H

struct file{};

void file_init();

struct file* file_alloc();
void file_close();
struct file* file_dup(struct file*);

int file_read(struct file*, char*, int);
int file_stat(struct file*, struct stat*);
int file_write(struct file*, char*, int);

#endif
