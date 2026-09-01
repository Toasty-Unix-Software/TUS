default rel
	nop
	nop
	nop bp
	nop cx
	nop ebx
	nop dword [0xe32]
	nop qword [0xa8d]
	nop qword [0xfc1]
	nop r10w
	nop r13d
	nop r9
	nop r16w
	nop r22d
	nop r24
	nop word [eax+1]
	nop word [eax+64]
	nop dword [eax+1]
	nop dword [eax+64]
	nop qword [eax+1]
	nop qword [eax+64]
	nop 0x31, dx, word [eax+1]
	nop 0x36, ax, word [eax+64]
	nop 0x7c, ecx, dword [eax+1]
	nop 0x53, edi, dword [eax+64]
	nop 0x5f, rsi, qword [eax+1]
	nop 0x1d, rdx, qword [eax+64]
	nop [0xecc]
	nop [0xcae]
	nop [0xa81]
	nop 0x4, bx, [0xf55]
	nop 0x3, edi, [0x409]
	nop 0x22, rbp, [0x1ee]
