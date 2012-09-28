#include <stdint.h>
#include "multiboot.h"
#include "io.h"
//#include "vga.h"

/**
 * Clear Screen
 */
void cls()
{
    unsigned char *videoram = (unsigned char *)0xB8000;
    unsigned int a;
    for (a = 0; a < 4000; a++)
    {
        *videoram++ = 0;
    }
}

/**
 * Print String on Screen with given location
 */
void print(char* string, unsigned char col, unsigned char row)
{
    if (row > 24) return;
    if (col > 79) return;
    
    unsigned char *videoram = (unsigned char *)0xB8000;
    while (*string != 0)
    {
        videoram[(row*80+col)*2] = *string++;
        videoram[(row*80+col++)*2+1] = 0x07;
        if (col == 160)
        {
            col = 0;
            row++;
            if (row > 79) return;
        }
    }
}

/**
 * Check, whether CPU is in protected oder real mode
 */
char get_cpu_mode()
{
    int reg_a = 0;
    asm (
        "mov %%cr0, %0"
        : "=r" (reg_a)
    );
    reg_a %= 2;
    return reg_a;
}

/**
 * Main
 */
void kmain(void)
{
    extern uint32_t magic;
    extern multiboot_info_t *mbd;
 
    if ( magic != MULTIBOOT_BOOTLOADER_MAGIC )
    {
        print("Multiboot error!",0,24);
        return;
    }
 
    cls(); // Clear Screen

    /* Funny colors */
    outb(0x3C7, 0x00);
    outb(0x3C8, 0x00);
    outb(0x3C9, 0x3F);
    outb(0x3C9, 0x1F);
    outb(0x3C9, 0x00);
    outb(0x3C7, 0x07);
    outb(0x3C8, 0x07);
    outb(0x3C9, 0x3F);
    outb(0x3C9, 0x3F);
    outb(0x3C9, 0x3F);
    
    print("Start des Betriebssystems.",1,1);
    if (get_cpu_mode()) print("CPU ist im Protected Mode.",1,2);
    print("Bootloadername:",1,3);
    print((char*)mbd->boot_loader_name,31,3);
    print("Kernel-Kommandozeile:",1,4);
    print((char*)mbd->cmdline,31,4);
    //int mode = 0x10;  
    
}

