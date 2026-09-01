default rel
	ccmpl 8, dl, al
	ccmpl 15, bl, dl
	ccmpl 4, si, bx
	ccmpl 15, word [0x50c], bx
	ccmpl 10, dword [0xabe], esi
	ccmpl 10, edx, edx
	ccmpl 11, rbp, rdx
	ccmpl 1, qword [0xad5], rbp
	ccmpl 5, r13b, r14b
	ccmpl 0, r13w, r15w
	ccmpl 1, r12d, r15d
	ccmpl 3, r9, r14
	ccmpl 11, r17b, r28b
	ccmpl 10, r19w, r17w
	ccmpl 2, r21d, r26d
	ccmpl 5, r25, r28
	ccmpl 13, byte [eax+1], bl
	ccmpl 5, byte [eax+64], al
	ccmpl 9, word [eax+1], si
	ccmpl 13, word [eax+64], di
	ccmpl 8, dword [eax+1], ecx
	ccmpl 0, dword [eax+64], ecx
	ccmpl 6, qword [eax+1], rdx
	ccmpl 6, qword [eax+64], rdi
	ccmpl 2, cl, byte [eax+1]
	ccmpl 0, al, byte [eax+64]
	ccmpl 7, dx, word [eax+1]
	ccmpl 13, cx, word [eax+64]
	ccmpl 12, ecx, dword [eax+1]
	ccmpl 9, ebx, dword [eax+64]
	ccmpl 5, rdi, qword [eax+1]
	ccmpl 11, rsi, qword [eax+64]
	ccmpl 8, word [eax+1], -12
	ccmpl 7, word [eax+64], -67
	ccmpl 9, dword [eax+1], -4
	ccmpl 13, dword [eax+64], -35
	ccmpl 9, qword [eax+1], 63
	ccmpl 2, qword [eax+64], -36
	ccmpl 6, byte [eax+1], 0x90
	ccmpl 15, byte [eax+64], 0xc3
	ccmpl 7, word [eax+1], 0xbee
	ccmpl 5, word [eax+64], 0xd5f3
	ccmpl 10, dword [eax+1], 0xb098b48
	ccmpl 13, dword [eax+64], 0x29b46fa9
	ccmpl 11, qword [eax+1], -381212624
	ccmpl 9, qword [eax+64], 500905960
	ccmpl 15, [0x243], cl
	ccmpl 6, [0xdc3], cx
	ccmpl 14, [0xfe9], ecx
	ccmpl 13, [0xafe], rbx
	ccmpl 11, al, [0xd6b]
	ccmpl 10, ax, [0x2ad]
	ccmpl 13, ebx, [0xa14]
	ccmpl 8, rax, [0x704]
	ccmpl 11, [0x7c2], -4
	ccmpl 13, [0xf0c], 98
	ccmpl 5, [0x50a], -31
	ccmpl 1, [0xcd7], 0x8
	ccmpl 11, [0x8cd], 0xe036
	ccmpl 15, [0x224], 0x21cd079e
	ccmpl 9, [0x94b], 514720756
