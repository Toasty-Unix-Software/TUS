default rel
	ctestb 5, dl, dl
	ctestb 15, al, bl
	ctestb 3, di, ax
	ctestb 13, word [0x733], bx
	ctestb 11, ebp, ebx
	ctestb 15, ecx, eax
	ctestb 14, qword [0xc65], rax
	ctestb 3, rax, rsi
	ctestb 10, r8b, r10b
	ctestb 15, r9w, r14w
	ctestb 15, r8d, r10d
	ctestb 5, r8, r12
	ctestb 7, r30b, r21b
	ctestb 12, r24w, r25w
	ctestb 10, r16d, r19d
	ctestb 0, r29, r28
	ctestb 15, byte [eax+1], dl
	ctestb 1, byte [eax+64], dl
	ctestb 4, word [eax+1], cx
	ctestb 6, word [eax+64], si
	ctestb 10, dword [eax+1], ecx
	ctestb 8, dword [eax+64], eax
	ctestb 13, qword [eax+1], rdx
	ctestb 11, qword [eax+64], rdx
	ctestb 2, byte [eax+1], 0x96
	ctestb 5, byte [eax+64], 0x77
	ctestb 3, word [eax+1], 0xe156
	ctestb 12, word [eax+64], 0xf016
	ctestb 14, dword [eax+1], 0x3c718fd4
	ctestb 15, dword [eax+64], 0x176d02a2
	ctestb 4, qword [eax+1], 138111268
	ctestb 9, qword [eax+64], -385562069
	ctestb 6, [0xba4], cl
	ctestb 8, [0xa63], cx
	ctestb 5, [0xe1f], eax
	ctestb 11, [0xcaf], rsi
	ctestb 0, [0xd73], 0x73
	ctestb 2, [0xca0], 0x5568
	ctestb 15, [0xb29], 0xa3a3d86
	ctestb 15, [0x68c], 453872912
