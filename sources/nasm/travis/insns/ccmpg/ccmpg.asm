default rel
	ccmpg 3, byte [0x4d4], bl
	ccmpg 3, dl, dl
	ccmpg 11, di, di
	ccmpg 1, cx, di
	ccmpg 4, edx, ebx
	ccmpg 4, edi, ebp
	ccmpg 0, rbx, rsi
	ccmpg 15, rdi, rsi
	ccmpg 6, r12b, r9b
	ccmpg 6, r11w, r14w
	ccmpg 0, r14d, r9d
	ccmpg 0, r9, r8
	ccmpg 15, r16b, r28b
	ccmpg 6, r30w, r21w
	ccmpg 9, r31d, r24d
	ccmpg 1, r28, r27
	ccmpg 3, byte [eax+1], bl
	ccmpg 9, byte [eax+64], dl
	ccmpg 5, word [eax+1], cx
	ccmpg 8, word [eax+64], di
	ccmpg 0, dword [eax+1], eax
	ccmpg 3, dword [eax+64], ebp
	ccmpg 13, qword [eax+1], rdx
	ccmpg 3, qword [eax+64], rdi
	ccmpg 14, cl, byte [eax+1]
	ccmpg 4, al, byte [eax+64]
	ccmpg 1, bp, word [eax+1]
	ccmpg 2, cx, word [eax+64]
	ccmpg 10, ecx, dword [eax+1]
	ccmpg 12, eax, dword [eax+64]
	ccmpg 1, rax, qword [eax+1]
	ccmpg 2, rsi, qword [eax+64]
	ccmpg 2, word [eax+1], 0
	ccmpg 8, word [eax+64], 70
	ccmpg 15, dword [eax+1], 89
	ccmpg 4, dword [eax+64], -32
	ccmpg 5, qword [eax+1], 87
	ccmpg 8, qword [eax+64], 0
	ccmpg 4, byte [eax+1], 0xf1
	ccmpg 15, byte [eax+64], 0x2
	ccmpg 2, word [eax+1], 0x8202
	ccmpg 4, word [eax+64], 0xf90b
	ccmpg 13, dword [eax+1], 0x361f4f33
	ccmpg 8, dword [eax+64], 0x3aa3b19a
	ccmpg 7, qword [eax+1], 429263645
	ccmpg 9, qword [eax+64], 220487927
	ccmpg 1, [0xa74], al
	ccmpg 15, [0x7dd], di
	ccmpg 9, [0x954], eax
	ccmpg 1, [0x68a], rdx
	ccmpg 3, cl, [0x240]
	ccmpg 4, bx, [0xabb]
	ccmpg 5, ebp, [0x4a7]
	ccmpg 3, rbp, [0x49e]
	ccmpg 4, [0x708], 66
	ccmpg 14, [0xc92], -79
	ccmpg 15, [0xd83], 92
	ccmpg 2, [0x98b], 0xbc
	ccmpg 14, [0x3d1], 0xd37a
	ccmpg 6, [0xd58], 0x1395ba99
	ccmpg 15, [0x696], 6647136
