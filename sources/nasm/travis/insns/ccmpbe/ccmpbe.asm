default rel
	ccmpbe 6, dl, bl
	ccmpbe 5, byte [0x3b3], bl
	ccmpbe 13, word [0xb57], bp
	ccmpbe 0, word [0xdd8], di
	ccmpbe 10, esi, edi
	ccmpbe 0, ebx, ecx
	ccmpbe 12, rdx, rbx
	ccmpbe 12, rdx, rdi
	ccmpbe 9, r8b, r12b
	ccmpbe 2, r15w, r9w
	ccmpbe 11, r15d, r12d
	ccmpbe 12, r13, r8
	ccmpbe 3, r28b, r19b
	ccmpbe 6, r18w, r27w
	ccmpbe 4, r22d, r23d
	ccmpbe 2, r17, r23
	ccmpbe 9, byte [eax+1], cl
	ccmpbe 14, byte [eax+64], dl
	ccmpbe 12, word [eax+1], bp
	ccmpbe 0, word [eax+64], cx
	ccmpbe 3, dword [eax+1], ecx
	ccmpbe 9, dword [eax+64], ebx
	ccmpbe 1, qword [eax+1], rdx
	ccmpbe 12, qword [eax+64], rbx
	ccmpbe 9, bl, byte [eax+1]
	ccmpbe 4, al, byte [eax+64]
	ccmpbe 6, bp, word [eax+1]
	ccmpbe 7, dx, word [eax+64]
	ccmpbe 14, ebx, dword [eax+1]
	ccmpbe 15, edx, dword [eax+64]
	ccmpbe 13, rax, qword [eax+1]
	ccmpbe 10, rdx, qword [eax+64]
	ccmpbe 8, word [eax+1], -15
	ccmpbe 9, word [eax+64], 79
	ccmpbe 5, dword [eax+1], -21
	ccmpbe 6, dword [eax+64], -37
	ccmpbe 2, qword [eax+1], 12
	ccmpbe 8, qword [eax+64], 43
	ccmpbe 2, byte [eax+1], 0x2
	ccmpbe 1, byte [eax+64], 0x4f
	ccmpbe 15, word [eax+1], 0xe713
	ccmpbe 0, word [eax+64], 0x371d
	ccmpbe 1, dword [eax+1], 0x12dcbb05
	ccmpbe 12, dword [eax+64], 0x1d611d6f
	ccmpbe 9, qword [eax+1], -207183652
	ccmpbe 12, qword [eax+64], -332313569
	ccmpbe 4, [0x561], dl
	ccmpbe 14, [0xbd1], si
	ccmpbe 13, [0xf6f], edi
	ccmpbe 5, [0x11a], rcx
	ccmpbe 13, al, [0xb3f]
	ccmpbe 6, di, [0x4c6]
	ccmpbe 10, edi, [0x74c]
	ccmpbe 11, rsi, [0x509]
	ccmpbe 4, [0xa94], 22
	ccmpbe 3, [0xec1], -115
	ccmpbe 14, [0x557], 122
	ccmpbe 3, [0x2ed], 0xca
	ccmpbe 6, [0x56e], 0xb87d
	ccmpbe 8, [0x2d1], 0xf6e4304
	ccmpbe 9, [0xebc], -470759931
