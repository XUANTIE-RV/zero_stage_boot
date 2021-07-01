ENTRY(entry)
SECTIONS
{
	. = 0x02000000;
	.text : {
		*(.text)
	}
	. = ALIGN(32);
	.data : {
		*(.data)
	}
	. = ALIGN(0x1000);
	.bss : {
		*(.bss)
	}
}
