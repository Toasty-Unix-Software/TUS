default rel
	ccmpf 2, al, al
	ccmpf 15, bl, dl
	ccmpf 15, cx, bx
	ccmpf 3, si, bp
	ccmpf 0, dword [0x4ee], ebx
	ccmpf 4, edi, ebp
	ccmpf 6, rbx, rsi
	ccmpf 1, rdx, rbp
	ccmpf 7, r14b, r9b
	ccmpf 12, r13w, r10w
	ccmpf 7, r8d, r8d
	ccmpf 6, r14, r11
	ccmpf 9, r26b, r28b
	ccmpf 4, r21w, r18w
	ccmpf 0, r21d, r20d
	ccmpf 14, r17, r22
	ccmpf 2, byte [eax+1], dl
	ccmpf 2, byte [eax+64], al
	ccmpf 8, word [eax+1], dx
	ccmpf 14, word [eax+64], dx
	ccmpf 5, dword [eax+1], ecx
	ccmpf 14, dword [eax+64], ebx
	ccmpf 0, qword [eax+1], rbp
	ccmpf 14, qword [eax+64], rdx
	ccmpf 2, dl, byte [eax+1]
	ccmpf 13, bl, byte [eax+64]
	ccmpf 10, cx, word [eax+1]
	ccmpf 11, ax, word [eax+64]
	ccmpf 0, edx, dword [eax+1]
	ccmpf 5, eax, dword [eax+64]
	ccmpf 9, rax, qword [eax+1]
	ccmpf 1, rdx, qword [eax+64]
	ccmpf 11, word [eax+1], 86
	ccmpf 9, word [eax+64], -99
	ccmpf 4, dword [eax+1], 119
	ccmpf 8, dword [eax+64], -91
	ccmpf 10, qword [eax+1], -87
	ccmpf 14, qword [eax+64], 88
	ccmpf 15, byte [eax+1], 0x12
	ccmpf 5, byte [eax+64], 0x96
	ccmpf 9, word [eax+1], 0xe763
	ccmpf 9, word [eax+64], 0x9fc6
	ccmpf 10, dword [eax+1], 0x3e01d64b
	ccmpf 7, dword [eax+64], 0x263e5334
	ccmpf 9, qword [eax+1], -87097222
	ccmpf 8, qword [eax+64], 286679442
	ccmpf 6, [0xa31], bl
	ccmpf 11, [0x726], bx
	ccmpf 11, [0x9c6], ecx
	ccmpf 13, [0x9a0], rbp
	ccmpf 2, dl, [0xe2a]
	ccmpf 2, di, [0x7b4]
	ccmpf 5, ebx, [0xe12]
	ccmpf 1, rbp, [0xdf1]
	ccmpf 14, [0xc7d], 85
	ccmpf 15, [0x438], 11
	ccmpf 5, [0xc29], -106
	ccmpf 2, [0xb7e], 0x78
	ccmpf 7, [0x170], 0xa478
	ccmpf 6, [0xaa6], 0x17a56c73
	ccmpf 15, [0x100], -432323728
