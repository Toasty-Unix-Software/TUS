default rel
	dec si
	dec dx
	dec ecx
	dec ebp
	dec cl
	dec al
	dec dx
	dec bx
	dec r13w
	dec r13d
	dec r9b
	dec r10w
	dec r20w
	dec r28d
	dec r19b
	dec r26w
	dec byte [eax+1]
	dec byte [eax+64]
	dec word [eax+1]
	dec word [eax+64]
	dec dword [eax+1]
	dec dword [eax+64]
	dec qword [eax+1]
	dec qword [eax+64]
	dec cl, byte [eax+1]
	dec cl, byte [eax+64]
	dec di, word [eax+1]
	dec bx, word [eax+64]
	dec ecx, dword [eax+1]
	dec edx, dword [eax+64]
	dec rbp, qword [eax+1]
	dec rbx, qword [eax+64]
	dec [0x86c]
	dec [0xa99]
	dec [0xf05]
	dec [0x7e2]
	dec bl, [0x920]
	dec ax, [0xefc]
	dec esi, [0x626]
	dec rcx, [0x671]
