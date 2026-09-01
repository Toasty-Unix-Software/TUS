default rel
	fcom dword [0x80a]
	fcom dword [0xc1f]
	fcom qword [0xccd]
	fcom qword [0x780]
	fcom st2
	fcom st1
	fcom st0, st7
	fcom st0, st6
	fcom dword [eax+1]
	fcom dword [eax+64]
	fcom qword [eax+1]
	fcom qword [eax+64]
	fcom [0x960]
	fcom [0x87c]
