#include <string.h>
#include <types.h>
#include <generic.h>
#include <drivers/console.h>
#include <memlayout.h>
#include <spinlock.h>

struct spinlock cons;

#define TML_ROWS 25                                         // 文本模式下25行
#define TML_COLUMNS 80                                      // 文本模式下80列

static int console_y = 0;                                   // 当前所在行
static int console_x = 0;                                   // 当前所在列
static ushort_t* console_fb = (ushort_t*)(P2V(0xB8000));    // 文本模式下的显存起始地址

// 控制台滚动
static void
console_scroll()
{
    int row, col;
    if (console_y >= TML_ROWS-1) {
        for (row = 0; row < TML_ROWS; row++){
            for (col = 0; col < TML_COLUMNS; col++) {
                const int dstIndex = row * TML_COLUMNS + col;
                const int srcIndex = (row + 1) * TML_COLUMNS + col;

                console_fb[dstIndex] = console_fb[srcIndex];
            }
        }
        console_y--;
    }
}

// 光标操作
static void
console_move()
{
    ushort_t pos = console_y * TML_COLUMNS + console_x;

    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

// 设置字符的前景色和背景色
static ushort_t
console_setcolor(ushort_t cnt, enum vga_color fg, enum vga_color bg)
{
    uchar_t color = (uchar_t)fg | (uchar_t)bg << 4;

    return cnt | (ushort_t)color << 8;
}

// console初始化
void
console_init()
{
    console_x = 0;
    console_y = 0;
    for (int y = 0; y < TML_ROWS; y++) {
        for (int x = 0; x < TML_COLUMNS; x++) {
            console_fb[y * TML_COLUMNS + x] = console_setcolor((ushort_t)' ', VGA_LIGHT_GREY, VGA_BLACK);
        }
    }
    initlock(&cons, "console lock");
}

// 默认颜色输出字符
void
console_put(char cnt)
{
    acquire(&cons);
    const int index = console_y * TML_COLUMNS + console_x;
    if(cnt == '\n'){
        console_y++;
        console_x = 0;
    }else if(cnt == 0x08){
        console_x--;
    }else if(cnt == 0x09){
        console_x = (console_x + 8) & ~(8-1);
    }else if(cnt >= ' '){
        console_fb[index] = console_setcolor((ushort_t)cnt, VGA_LIGHT_GREY, VGA_BLACK);
        console_x++;
    }

    if(console_x >= TML_COLUMNS-1){
        console_y++;
        console_x = 0;
    }

    console_scroll();
    console_move();
    release(&cons);
}

// 默认颜色输出字符串
void
console_write(char* cnt)
{
    int len = strlen(cnt);
    for (int i = 0; i < len; i++) {
        console_put(cnt[i]);
    }
}

// 自定义颜色输出字符
void
console_put_color(char cnt, enum vga_color fg, enum vga_color bg)
{
    acquire(&cons);
    const int index = console_y * TML_COLUMNS + console_x;
    if(cnt == '\n'){
        console_y++;
        console_x = 0;
    }else if(cnt == 0x08){
        console_x--;
    }else if(cnt == 0x09){
        console_x = (console_x + 8) & ~(8-1);
    }else if(cnt >= ' '){
        console_fb[index] = console_setcolor((ushort_t)cnt, fg, bg);
        console_x++;
    }

    if(console_x >= TML_COLUMNS){
        console_y++;
    }

    console_scroll();
    console_move();
    release(&cons);
}

// 自定义颜色输出字符串
void
console_write_color(char *cnt, enum vga_color fg, enum vga_color bg)
{
    int len = strlen(cnt);
    for (int i = 0; i < len; i++) {
        console_put_color(cnt[i], fg, bg);
    }
}

// 清屏
void
console_clear()
{
    // acquire(&cons);
    int i, j, index;
    for(i=0; i<TML_ROWS; ++i){
        for(j=0; j<TML_COLUMNS; ++j){
            index = i*TML_COLUMNS+j;
            console_fb[index] = console_setcolor((ushort_t)' ', VGA_LIGHT_GREY, VGA_BLACK);
        }
    }

    console_x = 0, console_y = 0;
    console_move();
    // release(&cons);
}
