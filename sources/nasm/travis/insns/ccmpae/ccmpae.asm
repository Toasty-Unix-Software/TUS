default rel
	ccmpae 8, byte [0x14d], dl
	ccmpae 11, dl, dl
	ccmpae 3, cx, bx
	ccmpae 5, word [0xa79], bp
	ccmpae 2, dword [0x76d], edi
	ccmpae 2, dword [0x14d], ebx
	ccmpae 2, rdi, rdx
	ccmpae 12, rbx, rcx
	ccmpae 10, r9b, r8b
	ccmpae 11, r14w, r13w
	ccmpae 7, r14d, r9d
	ccmpae 6, r9, r9
	ccmpae 1, r30b, r24b
	ccmpae 3, r27w, r17w
	ccmpae 5, r26d, r24d
	ccmpae 5, r17, r17
	ccmpae 7, byte [eax+1], al
	ccmpae 2, byte [eax+64], dl
	ccmpae 1, word [eax+1], cx
	ccmpae 5, word [eax+64], bx
	ccmpae 3, dword [eax+1], ebp
	ccmpae 10, dword [eax+64], eax
	ccmpae 15, qword [eax+1], rbp
	ccmpae 3, qword [eax+64], rsi
	ccmpae 15, cl, byte [eax+1]
	ccmpae 5, bl, byte [eax+64]
	ccmpae 12, dx, word [eax+1]
	ccmpae 3, si, word [eax+64]
	ccmpae 6, ebp, dword [eax+1]
	ccmpae 15, edi, dword [eax+64]
	ccmpae 11, rcx, qword [eax+1]
	ccmpae 7, rbx, qword [eax+64]
	ccmpae 10, word [eax+1], 63
	ccmpae 15, word [eax+64], 48
	ccmpae 13, dword [eax+1], -122
	ccmpae 5, dword [eax+64], 8
	ccmpae 0, qword [eax+1], -28
	ccmpae 15, qword [eax+64], -58
	ccmpae 0, byte [eax+1], 0x47
	ccmpae 12, byte [eax+64], 0xf1
	ccmpae 4, word [eax+1], 0xb01c
	ccmpae 5, word [eax+64], 0x4d9d
	ccmpae 8, dword [eax+1], 0x4da2541
	ccmpae 3, dword [eax+64], 0x34c95f1f
	ccmpae 14, qword [eax+1], 31016547
	ccmpae 10, qword [eax+64], -436118288
	ccmpae 7, [0x874], bl
	ccmpae 13, [0xa5a], di
	ccmpae 7, [0xb92], ebp
	ccmpae 6, [0x403], rax
	ccmpae 10, al, [0x3b1]
	ccmpae 6, dx, [0xd5c]
	ccmpae 13, edx, [0xd82]
	ccmpae 15, rbp, [0x291]
	ccmpae 5, [0xad9], 77
	ccmpae 12, [0xbbc], 49
	ccmpae 7, [0xfb0], -5
	ccmpae 9, [0x94f], 0x84
	ccmpae 13, [0xa37], 0x7f1e
	ccmpae 11, [0x5f5], 0x39c12128
	ccmpae 8, [0x996], -194416676
