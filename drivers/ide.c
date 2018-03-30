/*
 * ide硬盘的pio方式读写
 */

#include <drivers/ide.h>
#include <types.h>

/*
 *       Reading the harddisk using ports!
 *       +-------------------------------+   by qark
 *
 *
 *  This took me months to get working but I finally managed it.
 *
 *  This code only works for the 286+ so you must detect for 8088's somewhere
 *  in your code.
 *
 *  Technical Information on the ports:
 *      Port    Read/Write   Misc
 *     ------  ------------ -------------------------------------------------
 *       1f0       r/w       data register, the bytes are written/read here
 *       1f1       r         error register  (look these values up yourself)
 *       1f2       r/w       sector count, how many sectors to read/write
 *       1f3       r/w       sector number, the actual sector wanted
 *       1f4       r/w       cylinder low, cylinders is 0-1024
 *       1f5       r/w       cylinder high, this makes up the rest of the 1024
 *       1f6       r/w       drive/head
 *                              bit 7 = 1
 *                              bit 6 = 0
 *                              bit 5 = 1
 *                              bit 4 = 0  drive 0 select
 *                                    = 1  drive 1 select
 *                              bit 3-0    head select bits
 *       1f7       r         status register
 *                              bit 7 = 1  controller is executing a command
 *                              bit 6 = 1  drive is ready
 *                              bit 5 = 1  write fault
 *                              bit 4 = 1  seek complete
 *                              bit 3 = 1  sector buffer requires servicing
 *                              bit 2 = 1  disk data read corrected
 *                              bit 1 = 1  index - set to 1 each revolution
 *                              bit 0 = 1  previous command ended in an error
 *       1f7       w         command register
 *                            commands:
 *                              50h format track
 *                              20h read sectors with retry
 *                              21h read sectors without retry
 *                              22h read long with retry
 *                              23h read long without retry
 *                              30h write sectors with retry
 *                              31h write sectors without retry
 *                              32h write long with retry
 *                              33h write long without retry
 *
 *  Most of these should work on even non-IDE hard disks.
 *  This code is for reading, the code for writing is the next article.
 */

#define SECTOR_SIZE 512

static int 
ide_wait()
{

}

int 
ide_read()
{

}

int 
ide_write()
{

}