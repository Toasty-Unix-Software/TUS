default rel
	ccmpno 0, dl, dl
	ccmpno 1, al, bl
	ccmpno 1, bx, ax
	ccmpno 8, word [0xf51], bp
	ccmpno 4, ecx, ecx
	ccmpno 12, edi, eax
	ccmpno 14, qword [0x98f], rsi
	ccmpno 9, qword [0x529], rax
	ccmpno 14, r12b, r10b
	ccmpno 14, r8w, r14w
	ccmpno 0, r15d, r13d
	ccmpno 9, r13, r8
	ccmpno 6, r28b, r29b
	ccmpno 8, r18w, r27w
	ccmpno 8, r17d, r17d
	ccmpno 12, r24, r22
	ccmpno 5, byte [eax+1], cl
	ccmpno 6, byte [eax+64], bl
	ccmpno 4, word [eax+1], dx
	ccmpno 4, word [eax+64], si
	ccmpno 15, dword [eax+1], ebp
	ccmpno 0, dword [eax+64], eax
	ccmpno 8, qword [eax+1], rbp
	ccmpno 0, qword [eax+64], rcx
	ccmpno 3, cl, byte [eax+1]
	ccmpno 7, bl, byte [eax+64]
	ccmpno 15, di, word [eax+1]
	ccmpno 6, si, word [eax+64]
	ccmpno 9, ebx, dword [eax+1]
	ccmpno 13, edi, dword [eax+64]
	ccmpno 6, rax, qword [eax+1]
	ccmpno 15, rbx, qword [eax+64]
	ccmpno 1, word [eax+1], -63
	ccmpno 8, word [eax+64], 6
	ccmpno 3, dword [eax+1], 3
	ccmpno 9, dword [eax+64], -96
	ccmpno 14, qword [eax+1], -21
	ccmpno 13, qword [eax+64], -16
	ccmpno 9, byte [eax+1], 0x55
	ccmpno 4, byte [eax+64], 0xc4
	ccmpno 4, word [eax+1], 0xa879
	ccmpno 11, word [eax+64], 0x4e73
	ccmpno 8, dword [eax+1], 0x24b1ac1b
	ccmpno 13, dword [eax+64], 0x49120ca
	ccmpno 12, qword [eax+1], -396866397
	ccmpno 14, qword [eax+64], -42467842
	ccmpno 14, [0x2db], dl
	ccmpno 10, [0x355], cx
	ccmpno 7, [0x81b], ebp
	ccmpno 10, [0xc0d], rbp
	ccmpno 8, bl, [0xdec]
	ccmpno 3, bx, [0x3a4]
	ccmpno 14, edi, [0xccd]
	ccmpno 3, rbx, [0x151]
	ccmpno 5, [0xfbb], -107
	ccmpno 0, [0xbb6], 62
	ccmpno 4, [0xa13], -71
	ccmpno 3, [0xf81], 0xcc
	ccmpno 7, [0xcf9], 0x862b
	ccmpno 3, [0xcb8], 0x1ca610da
	ccmpno 2, [0x3ea], -361838799
