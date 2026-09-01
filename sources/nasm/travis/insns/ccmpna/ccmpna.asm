default rel
	ccmpna 7, byte [0xfa6], bl
	ccmpna 6, byte [0x551], al
	ccmpna 0, word [0xa7a], bx
	ccmpna 13, bp, si
	ccmpna 12, eax, esi
	ccmpna 11, edi, ebp
	ccmpna 10, rbp, rbp
	ccmpna 1, rbx, rcx
	ccmpna 13, r12b, r12b
	ccmpna 7, r9w, r9w
	ccmpna 6, r10d, r15d
	ccmpna 9, r9, r11
	ccmpna 5, r31b, r19b
	ccmpna 7, r29w, r22w
	ccmpna 1, r29d, r31d
	ccmpna 7, r24, r17
	ccmpna 8, byte [eax+1], dl
	ccmpna 4, byte [eax+64], cl
	ccmpna 10, word [eax+1], si
	ccmpna 3, word [eax+64], cx
	ccmpna 0, dword [eax+1], edx
	ccmpna 8, dword [eax+64], ecx
	ccmpna 6, qword [eax+1], rbx
	ccmpna 12, qword [eax+64], rcx
	ccmpna 6, al, byte [eax+1]
	ccmpna 15, al, byte [eax+64]
	ccmpna 15, di, word [eax+1]
	ccmpna 13, ax, word [eax+64]
	ccmpna 4, ebx, dword [eax+1]
	ccmpna 15, ebx, dword [eax+64]
	ccmpna 9, rbp, qword [eax+1]
	ccmpna 8, rcx, qword [eax+64]
	ccmpna 2, word [eax+1], -24
	ccmpna 13, word [eax+64], -2
	ccmpna 15, dword [eax+1], 14
	ccmpna 14, dword [eax+64], -36
	ccmpna 11, qword [eax+1], -36
	ccmpna 2, qword [eax+64], 119
	ccmpna 10, byte [eax+1], 0x34
	ccmpna 12, byte [eax+64], 0xc7
	ccmpna 13, word [eax+1], 0xae41
	ccmpna 7, word [eax+64], 0x72b0
	ccmpna 9, dword [eax+1], 0x3f985298
	ccmpna 3, dword [eax+64], 0xe274868
	ccmpna 0, qword [eax+1], 465072673
	ccmpna 2, qword [eax+64], 121930368
	ccmpna 5, [0x302], al
	ccmpna 4, [0x669], cx
	ccmpna 15, [0x491], ebp
	ccmpna 9, [0x95f], rdx
	ccmpna 15, al, [0xd0a]
	ccmpna 6, bp, [0x1b2]
	ccmpna 9, ebx, [0xc3a]
	ccmpna 8, rbx, [0x1e3]
	ccmpna 13, [0x7c7], -11
	ccmpna 14, [0xecc], -108
	ccmpna 8, [0x269], -103
	ccmpna 12, [0x309], 0x17
	ccmpna 1, [0xeb6], 0x6082
	ccmpna 12, [0x72b], 0x17a336fb
	ccmpna 7, [0xb8f], -356894819
