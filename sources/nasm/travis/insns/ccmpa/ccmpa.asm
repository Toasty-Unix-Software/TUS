default rel
	ccmpa 10, dl, bl
	ccmpa 11, dl, dl
	ccmpa 0, ax, di
	ccmpa 4, bx, ax
	ccmpa 13, dword [0x89e], ebx
	ccmpa 1, ecx, ebp
	ccmpa 14, rsi, rsi
	ccmpa 6, qword [0x3d6], rcx
	ccmpa 4, r13b, r14b
	ccmpa 0, r15w, r12w
	ccmpa 9, r12d, r13d
	ccmpa 2, r12, r13
	ccmpa 13, r28b, r25b
	ccmpa 3, r22w, r16w
	ccmpa 2, r27d, r25d
	ccmpa 9, r29, r24
	ccmpa 12, byte [eax+1], dl
	ccmpa 1, byte [eax+64], dl
	ccmpa 7, word [eax+1], di
	ccmpa 7, word [eax+64], bx
	ccmpa 8, dword [eax+1], esi
	ccmpa 10, dword [eax+64], edx
	ccmpa 11, qword [eax+1], rbp
	ccmpa 9, qword [eax+64], rbx
	ccmpa 5, cl, byte [eax+1]
	ccmpa 6, dl, byte [eax+64]
	ccmpa 2, bp, word [eax+1]
	ccmpa 0, dx, word [eax+64]
	ccmpa 3, edx, dword [eax+1]
	ccmpa 13, edx, dword [eax+64]
	ccmpa 3, rbx, qword [eax+1]
	ccmpa 8, rdx, qword [eax+64]
	ccmpa 4, word [eax+1], -15
	ccmpa 9, word [eax+64], -22
	ccmpa 9, dword [eax+1], 105
	ccmpa 11, dword [eax+64], -46
	ccmpa 3, qword [eax+1], 35
	ccmpa 12, qword [eax+64], -73
	ccmpa 7, byte [eax+1], 0x3
	ccmpa 7, byte [eax+64], 0x14
	ccmpa 13, word [eax+1], 0x2217
	ccmpa 1, word [eax+64], 0xfd2c
	ccmpa 10, dword [eax+1], 0x2d3c990d
	ccmpa 15, dword [eax+64], 0x2d856cc1
	ccmpa 0, qword [eax+1], 477395961
	ccmpa 4, qword [eax+64], 326080965
	ccmpa 2, [0x34c], al
	ccmpa 9, [0xe22], bx
	ccmpa 12, [0xe18], edi
	ccmpa 0, [0x19c], rdx
	ccmpa 5, cl, [0x84b]
	ccmpa 4, ax, [0x57e]
	ccmpa 2, edi, [0xf8b]
	ccmpa 0, rbp, [0xc25]
	ccmpa 11, [0x481], 48
	ccmpa 10, [0x572], -87
	ccmpa 6, [0xd9b], 21
	ccmpa 4, [0x3d4], 0xb5
	ccmpa 6, [0x9d9], 0x173e
	ccmpa 4, [0x89e], 0x18dcf482
	ccmpa 3, [0xf98], -277723076
