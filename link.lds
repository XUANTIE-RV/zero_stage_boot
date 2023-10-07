ENTRY(entry)
SECTIONS
{
	. = 0x60000000;
	PROVIDE(_fw_start = .);
	.text : {
		PROVIDE(_text_start = .);
		*(.text)
		PROVIDE(_text_end = .);
	}
	. = ALIGN(8);
	.data : {
		PROVIDE(_data_start = .);
		*(.data)
		. = ALIGN(8);
		PROVIDE(_data_end = .);
	}
	. = ALIGN(0x1000);
	.bss : {
		PROVIDE(_bss_start = .);
		*(.bss)
		. = ALIGN(8);
		PROVIDE(_bss_end = .);
	}
	. = ALIGN(0x1000);
	PROVIDE(_fw_end = .);
}
