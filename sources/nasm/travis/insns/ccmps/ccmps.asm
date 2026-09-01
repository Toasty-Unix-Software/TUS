default rel
	ccmps 2, byte [0x725], al
	ccmps 4, byte [0xae7], bl
	ccmps 11, bx, bp
	ccmps 2, dx, bp
	ccmps 14, ebp, eax
	ccmps 4, esi, ecx
	ccmps 8, rbx, rdx
	ccmps 1, rdx, rbp
	ccmps 2, r8b, r10b
	ccmps 2, r13w, r15w
	ccmps 8, r12d, r13d
	ccmps 0, r11, r14
	ccmps 1, r20b, r27b
	ccmps 4, r19w, r30w
	ccmps 8, r27d, r30d
	ccmps 1, r16, r17
	ccmps 3, byte [eax+1], dl
	ccmps 2, byte [eax+64], dl
	ccmps 12, word [eax+1], di
	ccmps 5, word [eax+64], bp
	ccmps 11, dword [eax+1], ecx
	ccmps 11, dword [eax+64], edx
	ccmps 6, qword [eax+1], rcx
	ccmps 1, qword [eax+64], rdi
	ccmps 5, bl, byte [eax+1]
	ccmps 5, al, byte [eax+64]
	ccmps 13, di, word [eax+1]
	ccmps 4, bx, word [eax+64]
	ccmps 12, esi, dword [eax+1]
	ccmps 11, esi, dword [eax+64]
	ccmps 1, rax, qword [eax+1]
	ccmps 2, rsi, qword [eax+64]
	ccmps 13, word [eax+1], -104
	ccmps 10, word [eax+64], -31
	ccmps 8, dword [eax+1], 106
	ccmps 11, dword [eax+64], -108
	ccmps 3, qword [eax+1], -122
	ccmps 8, qword [eax+64], 38
	ccmps 11, byte [eax+1], 0x64
	ccmps 4, byte [eax+64], 0x6e
	ccmps 9, word [eax+1], 0xafd1
	ccmps 5, word [eax+64], 0xc49a
	ccmps 12, dword [eax+1], 0x2838c5ab
	ccmps 8, dword [eax+64], 0x27d5fd
	ccmps 10, qword [eax+1], -340874139
	ccmps 6, qword [eax+64], 392403653
	ccmps 0, [0xa06], cl
	ccmps 2, [0x26e], ax
	ccmps 2, [0x768], ecx
	ccmps 6, [0x3e5], rbp
	ccmps 5, al, [0xa30]
	ccmps 8, bp, [0x50a]
	ccmps 6, ecx, [0xd08]
	ccmps 11, rsi, [0x73d]
	ccmps 9, [0xfd1], -77
	ccmps 7, [0x7f5], -62
	ccmps 5, [0x4cd], -33
	ccmps 5, [0x5d2], 0x14
	ccmps 1, [0x765], 0xcb14
	ccmps 12, [0xce6], 0x1465d25f
	ccmps 10, [0xc44], 71502190
