default rel
	ccmple 4, bl, cl
	ccmple 1, byte [0x1e4], dl
	ccmple 5, di, cx
	ccmple 14, dx, bp
	ccmple 12, ebp, ecx
	ccmple 6, dword [0xda2], ebp
	ccmple 14, rsi, rbp
	ccmple 11, rdi, rcx
	ccmple 3, r9b, r12b
	ccmple 15, r13w, r12w
	ccmple 9, r11d, r13d
	ccmple 15, r8, r9
	ccmple 0, r26b, r25b
	ccmple 3, r21w, r22w
	ccmple 15, r28d, r22d
	ccmple 12, r24, r27
	ccmple 2, byte [eax+1], cl
	ccmple 7, byte [eax+64], bl
	ccmple 1, word [eax+1], bp
	ccmple 11, word [eax+64], bp
	ccmple 9, dword [eax+1], edx
	ccmple 5, dword [eax+64], eax
	ccmple 9, qword [eax+1], rbp
	ccmple 7, qword [eax+64], rsi
	ccmple 12, al, byte [eax+1]
	ccmple 14, cl, byte [eax+64]
	ccmple 12, bx, word [eax+1]
	ccmple 14, si, word [eax+64]
	ccmple 10, ecx, dword [eax+1]
	ccmple 10, edi, dword [eax+64]
	ccmple 9, rdx, qword [eax+1]
	ccmple 15, rax, qword [eax+64]
	ccmple 4, word [eax+1], -96
	ccmple 0, word [eax+64], 48
	ccmple 5, dword [eax+1], 95
	ccmple 0, dword [eax+64], -3
	ccmple 0, qword [eax+1], 95
	ccmple 4, qword [eax+64], -14
	ccmple 8, byte [eax+1], 0x36
	ccmple 0, byte [eax+64], 0xb
	ccmple 9, word [eax+1], 0x4825
	ccmple 15, word [eax+64], 0xb529
	ccmple 12, dword [eax+1], 0x1f03b59c
	ccmple 7, dword [eax+64], 0xe1e574a
	ccmple 6, qword [eax+1], 46608980
	ccmple 5, qword [eax+64], -267493611
	ccmple 9, [0x1e6], dl
	ccmple 10, [0x3a3], bx
	ccmple 14, [0xdd8], edi
	ccmple 2, [0xf76], rsi
	ccmple 0, al, [0x1cc]
	ccmple 12, cx, [0x92c]
	ccmple 14, edx, [0xb2a]
	ccmple 15, rdi, [0xa96]
	ccmple 12, [0xe76], 42
	ccmple 5, [0xf94], 1
	ccmple 2, [0x39a], 83
	ccmple 11, [0x757], 0x16
	ccmple 15, [0xbbb], 0x314
	ccmple 9, [0x40d], 0x3238c6ad
	ccmple 13, [0x7b3], -508976809
