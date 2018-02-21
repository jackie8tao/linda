#include <mm/pgmgr.h>
#include <mm/kalloc.h>
#include <apic.h>

void 
kmain()
{
    apic_init();
}