default rel
	ccmpc 9, al, dl
	ccmpc 3, bl, cl
	ccmpc 8, word [0x5e9], cx
	ccmpc 11, ax, si
	ccmpc 13, dword [0x544], edi
	ccmpc 6, eax, ebx
	ccmpc 14, rax, rsi
	ccmpc 15, rdi, rsi
	ccmpc 2, r8b, r11b
	ccmpc 15, r8w, r15w
	ccmpc 8, r9d, r11d
	ccmpc 2, r15, r15
	ccmpc 9, r27b, r29b
	ccmpc 13, r16w, r29w
	ccmpc 8, r30d, r19d
	ccmpc 9, r17, r20
	ccmpc 10, byte [eax+1], cl
	ccmpc 7, byte [eax+64], al
	ccmpc 2, word [eax+1], bx
	ccmpc 10, word [eax+64], dx
	ccmpc 8, dword [eax+1], ebx
	ccmpc 0, dword [eax+64], esi
	ccmpc 14, qword [eax+1], rdx
	ccmpc 5, qword [eax+64], rsi
	ccmpc 1, al, byte [eax+1]
	ccmpc 0, dl, byte [eax+64]
	ccmpc 14, bx, word [eax+1]
	ccmpc 15, bp, word [eax+64]
	ccmpc 11, edi, dword [eax+1]
	ccmpc 12, ebx, dword [eax+64]
	ccmpc 10, rcx, qword [eax+1]
	ccmpc 7, rdi, qword [eax+64]
	ccmpc 8, word [eax+1], -58
	ccmpc 6, word [eax+64], 91
	ccmpc 1, dword [eax+1], -7
	ccmpc 2, dword [eax+64], 6
	ccmpc 7, qword [eax+1], 47
	ccmpc 8, qword [eax+64], 33
	ccmpc 6, byte [eax+1], 0x74
	ccmpc 1, byte [eax+64], 0x6b
	ccmpc 0, word [eax+1], 0x8e8e
	ccmpc 2, word [eax+64], 0xfb7
	ccmpc 13, dword [eax+1], 0xfdf8a1a
	ccmpc 11, dword [eax+64], 0x3043f972
	ccmpc 8, qword [eax+1], 53357401
	ccmpc 11, qword [eax+64], -142029383
	ccmpc 2, [0x9d9], al
	ccmpc 13, [0x49f], cx
	ccmpc 9, [0xb21], edx
	ccmpc 14, [0xe32], rdi
	ccmpc 4, dl, [0x8fd]
	ccmpc 9, dx, [0xb2b]
	ccmpc 3, esi, [0xdb5]
	ccmpc 10, rax, [0x273]
	ccmpc 15, [0x2fc], -113
	ccmpc 7, [0x536], -78
	ccmpc 8, [0xaf0], -115
	ccmpc 2, [0xefb], 0x75
	ccmpc 14, [0xd9b], 0x8d13
	ccmpc 10, [0x9e0], 0x8795896
	ccmpc 3, [0xbac], 104916897
