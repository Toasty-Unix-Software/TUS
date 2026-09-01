default rel
	ccmpnae 15, byte [0xc08], dl
	ccmpnae 3, cl, bl
	ccmpnae 5, cx, dx
	ccmpnae 14, dx, si
	ccmpnae 15, ecx, esi
	ccmpnae 6, ebx, ebp
	ccmpnae 2, rax, rbx
	ccmpnae 12, rbx, rax
	ccmpnae 1, r9b, r14b
	ccmpnae 6, r8w, r9w
	ccmpnae 1, r10d, r13d
	ccmpnae 2, r10, r9
	ccmpnae 6, r24b, r25b
	ccmpnae 11, r23w, r28w
	ccmpnae 0, r31d, r28d
	ccmpnae 9, r20, r19
	ccmpnae 7, byte [eax+1], cl
	ccmpnae 8, byte [eax+64], bl
	ccmpnae 0, word [eax+1], cx
	ccmpnae 0, word [eax+64], di
	ccmpnae 4, dword [eax+1], esi
	ccmpnae 13, dword [eax+64], ebp
	ccmpnae 6, qword [eax+1], rbp
	ccmpnae 15, qword [eax+64], rbp
	ccmpnae 2, cl, byte [eax+1]
	ccmpnae 11, al, byte [eax+64]
	ccmpnae 4, cx, word [eax+1]
	ccmpnae 10, si, word [eax+64]
	ccmpnae 6, eax, dword [eax+1]
	ccmpnae 9, edx, dword [eax+64]
	ccmpnae 2, rbp, qword [eax+1]
	ccmpnae 12, rbx, qword [eax+64]
	ccmpnae 12, word [eax+1], -43
	ccmpnae 9, word [eax+64], 109
	ccmpnae 11, dword [eax+1], 110
	ccmpnae 5, dword [eax+64], 41
	ccmpnae 14, qword [eax+1], -113
	ccmpnae 5, qword [eax+64], -79
	ccmpnae 6, byte [eax+1], 0x76
	ccmpnae 6, byte [eax+64], 0xa1
	ccmpnae 7, word [eax+1], 0x41c5
	ccmpnae 8, word [eax+64], 0xa208
	ccmpnae 11, dword [eax+1], 0x2077168
	ccmpnae 10, dword [eax+64], 0x37dba893
	ccmpnae 6, qword [eax+1], 87072369
	ccmpnae 10, qword [eax+64], 67641877
	ccmpnae 10, [0x728], bl
	ccmpnae 15, [0x8a5], bx
	ccmpnae 0, [0xebb], ebx
	ccmpnae 9, [0x3db], rcx
	ccmpnae 13, al, [0xad7]
	ccmpnae 2, bp, [0x129]
	ccmpnae 1, edi, [0xc79]
	ccmpnae 3, rsi, [0xf44]
	ccmpnae 9, [0x120], -13
	ccmpnae 2, [0x6bf], 53
	ccmpnae 0, [0xbad], 22
	ccmpnae 14, [0xc03], 0x9e
	ccmpnae 8, [0x495], 0x3c85
	ccmpnae 3, [0x529], 0x13729640
	ccmpnae 13, [0x3ae], 382090881
