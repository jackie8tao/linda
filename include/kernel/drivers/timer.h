/* 
 * 时钟硬件管理
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-23 02:55
 */

#ifndef __LINDA_TIMER_H
#define __LINDA_TIMER_H

#include <types.h>
#include <trap.h>

void timer_setup(trap_handler_t handler);

#endif