OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(kern_entry)

SECTIONS
{
	. = 0xC0100000;
	PROVIDE(kern_start = .);
	.text : AT(0x100000) {
		*(multiboot_header)
		*(.text)
	}
	.data : {
		*(.data)
	}
	.rodata : {
		*(.rodata)
	}
	.bss : {
		*(.bss)
	}
	PROVIDE(kern_end = .);
}
