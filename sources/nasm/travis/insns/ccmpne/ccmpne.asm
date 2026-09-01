default rel
	ccmpne 9, byte [0x206], bl
	ccmpne 15, al, al
	ccmpne 15, word [0x568], bp
	ccmpne 13, word [0x1a7], cx
	ccmpne 8, esi, ebp
	ccmpne 11, ebp, eax
	ccmpne 7, qword [0x440], rbx
	ccmpne 11, qword [0x8fb], rcx
	ccmpne 14, r9b, r13b
	ccmpne 14, r13w, r15w
	ccmpne 5, r13d, r9d
	ccmpne 8, r12, r14
	ccmpne 13, r21b, r27b
	ccmpne 10, r30w, r25w
	ccmpne 3, r29d, r28d
	ccmpne 6, r21, r19
	ccmpne 8, byte [eax+1], al
	ccmpne 12, byte [eax+64], bl
	ccmpne 11, word [eax+1], bx
	ccmpne 15, word [eax+64], ax
	ccmpne 15, dword [eax+1], ebp
	ccmpne 12, dword [eax+64], edx
	ccmpne 0, qword [eax+1], rbx
	ccmpne 7, qword [eax+64], rdi
	ccmpne 2, cl, byte [eax+1]
	ccmpne 3, dl, byte [eax+64]
	ccmpne 10, si, word [eax+1]
	ccmpne 15, cx, word [eax+64]
	ccmpne 1, esi, dword [eax+1]
	ccmpne 13, ecx, dword [eax+64]
	ccmpne 11, rcx, qword [eax+1]
	ccmpne 5, rbx, qword [eax+64]
	ccmpne 6, word [eax+1], -80
	ccmpne 9, word [eax+64], 31
	ccmpne 3, dword [eax+1], 50
	ccmpne 7, dword [eax+64], 74
	ccmpne 6, qword [eax+1], -44
	ccmpne 3, qword [eax+64], 17
	ccmpne 1, byte [eax+1], 0x3a
	ccmpne 4, byte [eax+64], 0x1
	ccmpne 12, word [eax+1], 0xc297
	ccmpne 9, word [eax+64], 0x2635
	ccmpne 0, dword [eax+1], 0x285ce305
	ccmpne 12, dword [eax+64], 0x4f8f067
	ccmpne 1, qword [eax+1], -526217650
	ccmpne 15, qword [eax+64], 40731337
	ccmpne 5, [0xe2f], al
	ccmpne 5, [0xde2], di
	ccmpne 2, [0xe61], ebp
	ccmpne 6, [0xb72], rcx
	ccmpne 15, al, [0xa0c]
	ccmpne 11, di, [0x656]
	ccmpne 6, ecx, [0xe21]
	ccmpne 5, rsi, [0x47e]
	ccmpne 10, [0x9c7], 77
	ccmpne 13, [0xc12], -78
	ccmpne 1, [0x592], 110
	ccmpne 14, [0x5ea], 0x73
	ccmpne 9, [0xe90], 0xc217
	ccmpne 7, [0xbd2], 0x293b79fb
	ccmpne 1, [0xc2d], 210177463
