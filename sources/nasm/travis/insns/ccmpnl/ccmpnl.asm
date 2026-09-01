default rel
	ccmpnl 11, bl, al
	ccmpnl 4, al, dl
	ccmpnl 12, word [0xdb5], si
	ccmpnl 2, di, bx
	ccmpnl 1, ebx, esi
	ccmpnl 2, edx, esi
	ccmpnl 11, qword [0x6e1], rsi
	ccmpnl 4, qword [0x3ae], rcx
	ccmpnl 7, r14b, r13b
	ccmpnl 8, r14w, r15w
	ccmpnl 7, r10d, r14d
	ccmpnl 5, r9, r13
	ccmpnl 4, r29b, r24b
	ccmpnl 13, r25w, r30w
	ccmpnl 15, r16d, r16d
	ccmpnl 7, r25, r27
	ccmpnl 9, byte [eax+1], al
	ccmpnl 14, byte [eax+64], al
	ccmpnl 10, word [eax+1], si
	ccmpnl 6, word [eax+64], si
	ccmpnl 12, dword [eax+1], edx
	ccmpnl 13, dword [eax+64], edx
	ccmpnl 4, qword [eax+1], rax
	ccmpnl 5, qword [eax+64], rcx
	ccmpnl 15, bl, byte [eax+1]
	ccmpnl 7, cl, byte [eax+64]
	ccmpnl 15, di, word [eax+1]
	ccmpnl 7, si, word [eax+64]
	ccmpnl 11, ecx, dword [eax+1]
	ccmpnl 4, ebx, dword [eax+64]
	ccmpnl 12, rbx, qword [eax+1]
	ccmpnl 2, rsi, qword [eax+64]
	ccmpnl 13, word [eax+1], -47
	ccmpnl 5, word [eax+64], 46
	ccmpnl 5, dword [eax+1], -55
	ccmpnl 6, dword [eax+64], 35
	ccmpnl 13, qword [eax+1], -121
	ccmpnl 0, qword [eax+64], 36
	ccmpnl 9, byte [eax+1], 0x56
	ccmpnl 0, byte [eax+64], 0x15
	ccmpnl 12, word [eax+1], 0x7b59
	ccmpnl 1, word [eax+64], 0x2ea1
	ccmpnl 3, dword [eax+1], 0xa45c05b
	ccmpnl 14, dword [eax+64], 0x21a450b0
	ccmpnl 7, qword [eax+1], 23818694
	ccmpnl 9, qword [eax+64], -307914047
	ccmpnl 8, [0x70f], cl
	ccmpnl 14, [0xf4b], bp
	ccmpnl 12, [0x604], edx
	ccmpnl 15, [0x3c4], rbx
	ccmpnl 0, al, [0xb1a]
	ccmpnl 8, ax, [0x92d]
	ccmpnl 2, edi, [0xdea]
	ccmpnl 11, rdx, [0x6d8]
	ccmpnl 9, [0xd91], 73
	ccmpnl 13, [0xa46], -118
	ccmpnl 9, [0x280], 105
	ccmpnl 1, [0x6eb], 0x52
	ccmpnl 9, [0x94e], 0x1ef0
	ccmpnl 6, [0xf04], 0x2cbcc907
	ccmpnl 1, [0xa0f], 493604984
