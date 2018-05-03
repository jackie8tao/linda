/*
 * 可编程中断控制器操作
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-22 16:44
 */

#ifndef __LINDA_PIC_H
#define __LINDA_PIC_H

#include <types.h>

void pic_remap(ushort_t master_offset, ushort_t slave_offset);

void pic_disable();

#endif
