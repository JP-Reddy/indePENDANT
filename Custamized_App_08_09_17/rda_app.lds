

SECTIONS
{
	. = 0x88250000;
	
	_app_start = .;

	.sys_vt . : AT(ADDR(.sys_vt))
	{
		*(.SYS_VVT)
	}

	.app_code . : AT(ADDR(.app_code))
	{
		*(.text)		
	}
	
	.app_rodata . : AT(ADDR(.app_rodata))
	{
		*(.rodata)
		*(.rodata.str1.1)
		*(.rodata.str1.4)
		*(.rodata.*)
	}
	
	_SYS_APP_DATA_FLASH_START = .;
	. = 0x8237f000;
	
	.app_data _SYS_APP_DATA_RAM_START : AT(_SYS_APP_DATA_FLASH_START)
	{
		_SYS_APP_DATA_RAM_START = .;
		*(.data)
		*(.rwdata)
		*(.sdata)
	}
	
	_SYS_APP_DATA_RAM_END = .;

	_SYS_APP_DATA_SIZE = _SYS_APP_DATA_RAM_END - _SYS_APP_DATA_RAM_START;

	_SYS_APP_BSS_FLASH_START = _SYS_APP_DATA_FLASH_START + _SYS_APP_DATA_SIZE;

	_app_end  = _SYS_APP_BSS_FLASH_START;

	_app_size = _app_end - _app_start;


	ASSERT(_app_size < 512 * 1024,"APP CODE OVERFLOW !!")
	
	.app_bss . : AT(_SYS_APP_BSS_FLASH_START)
	{
		_SYS_APP_BSS_RAM_START = .;
		*(.bss)
		*(.sbss)
		*(COMMON)
		*(.scommon)
	}
	
	_SYS_APP_BSS_RAM_END = .;

	_SYS_APP_BSS_SIZE = _SYS_APP_BSS_RAM_END - _SYS_APP_BSS_RAM_START;

	ASSERT(_SYS_APP_DATA_SIZE +  _SYS_APP_BSS_SIZE < 512 * 1024,"APP DATA OVERFLOW !!")
}

