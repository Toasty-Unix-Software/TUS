default rel
	ccmpnbe 10, al, al
	ccmpnbe 10, byte [0xa24], al
	ccmpnbe 2, word [0x850], cx
	ccmpnbe 9, cx, bp
	ccmpnbe 5, edi, edi
	ccmpnbe 4, edx, eax
	ccmpnbe 4, rdx, rcx
	ccmpnbe 13, qword [0xcfd], rsi
	ccmpnbe 0, r8b, r11b
	ccmpnbe 3, r10w, r12w
	ccmpnbe 15, r12d, r13d
	ccmpnbe 9, r10, r12
	ccmpnbe 9, r23b, r28b
	ccmpnbe 11, r21w, r18w
	ccmpnbe 14, r30d, r29d
	ccmpnbe 2, r27, r22
	ccmpnbe 15, byte [eax+1], al
	ccmpnbe 6, byte [eax+64], cl
	ccmpnbe 3, word [eax+1], dx
	ccmpnbe 0, word [eax+64], ax
	ccmpnbe 9, dword [eax+1], edi
	ccmpnbe 15, dword [eax+64], ecx
	ccmpnbe 11, qword [eax+1], rax
	ccmpnbe 7, qword [eax+64], rdi
	ccmpnbe 0, bl, byte [eax+1]
	ccmpnbe 6, bl, byte [eax+64]
	ccmpnbe 1, si, word [eax+1]
	ccmpnbe 0, di, word [eax+64]
	ccmpnbe 7, edi, dword [eax+1]
	ccmpnbe 12, edx, dword [eax+64]
	ccmpnbe 6, rdi, qword [eax+1]
	ccmpnbe 3, rdi, qword [eax+64]
	ccmpnbe 6, word [eax+1], 36
	ccmpnbe 8, word [eax+64], 21
	ccmpnbe 7, dword [eax+1], -126
	ccmpnbe 7, dword [eax+64], 110
	ccmpnbe 11, qword [eax+1], 108
	ccmpnbe 7, qword [eax+64], -35
	ccmpnbe 4, byte [eax+1], 0xd9
	ccmpnbe 0, byte [eax+64], 0x32
	ccmpnbe 14, word [eax+1], 0x9930
	ccmpnbe 13, word [eax+64], 0xae29
	ccmpnbe 11, dword [eax+1], 0x2ad02865
	ccmpnbe 5, dword [eax+64], 0x9f07c73
	ccmpnbe 2, qword [eax+1], -18119360
	ccmpnbe 9, qword [eax+64], -62441155
	ccmpnbe 5, [0xd46], al
	ccmpnbe 0, [0x439], bp
	ccmpnbe 13, [0x420], ebx
	ccmpnbe 15, [0x8bd], rdi
	ccmpnbe 4, bl, [0x983]
	ccmpnbe 2, bp, [0x4bf]
	ccmpnbe 1, ecx, [0x389]
	ccmpnbe 10, rdi, [0x65d]
	ccmpnbe 8, [0x492], 9
	ccmpnbe 12, [0xc3f], -32
	ccmpnbe 1, [0xd93], -80
	ccmpnbe 11, [0x6c9], 0xb
	ccmpnbe 0, [0x53e], 0x9987
	ccmpnbe 9, [0x8c0], 0x283e8861
	ccmpnbe 7, [0x6b6], 246365479
