default rel
	ccmpnle 2, byte [0x810], cl
	ccmpnle 5, byte [0xa07], dl
	ccmpnle 9, bx, ax
	ccmpnle 9, word [0x337], dx
	ccmpnle 9, eax, edx
	ccmpnle 7, ecx, edi
	ccmpnle 3, rcx, rsi
	ccmpnle 9, qword [0x41a], rcx
	ccmpnle 7, r15b, r15b
	ccmpnle 1, r13w, r10w
	ccmpnle 5, r8d, r12d
	ccmpnle 11, r8, r9
	ccmpnle 2, r29b, r20b
	ccmpnle 13, r27w, r16w
	ccmpnle 15, r21d, r27d
	ccmpnle 0, r26, r19
	ccmpnle 8, byte [eax+1], dl
	ccmpnle 7, byte [eax+64], dl
	ccmpnle 4, word [eax+1], ax
	ccmpnle 13, word [eax+64], si
	ccmpnle 9, dword [eax+1], edi
	ccmpnle 9, dword [eax+64], esi
	ccmpnle 5, qword [eax+1], rdx
	ccmpnle 15, qword [eax+64], rcx
	ccmpnle 12, cl, byte [eax+1]
	ccmpnle 8, dl, byte [eax+64]
	ccmpnle 8, ax, word [eax+1]
	ccmpnle 9, bp, word [eax+64]
	ccmpnle 6, edx, dword [eax+1]
	ccmpnle 7, eax, dword [eax+64]
	ccmpnle 10, rsi, qword [eax+1]
	ccmpnle 3, rax, qword [eax+64]
	ccmpnle 5, word [eax+1], -115
	ccmpnle 2, word [eax+64], 71
	ccmpnle 11, dword [eax+1], -118
	ccmpnle 13, dword [eax+64], -62
	ccmpnle 11, qword [eax+1], 112
	ccmpnle 11, qword [eax+64], 93
	ccmpnle 5, byte [eax+1], 0xf0
	ccmpnle 1, byte [eax+64], 0x99
	ccmpnle 4, word [eax+1], 0xd343
	ccmpnle 5, word [eax+64], 0x9e03
	ccmpnle 4, dword [eax+1], 0xc403f21
	ccmpnle 15, dword [eax+64], 0x2612a2f8
	ccmpnle 14, qword [eax+1], -38193403
	ccmpnle 4, qword [eax+64], 48699356
	ccmpnle 0, [0x563], cl
	ccmpnle 4, [0x89f], cx
	ccmpnle 5, [0x10c], ebp
	ccmpnle 10, [0xac3], rcx
	ccmpnle 14, bl, [0xd52]
	ccmpnle 5, di, [0x56c]
	ccmpnle 7, ebp, [0x4fe]
	ccmpnle 6, rcx, [0xa7b]
	ccmpnle 14, [0x6a9], 25
	ccmpnle 15, [0xecf], 99
	ccmpnle 6, [0xa32], 1
	ccmpnle 15, [0xe6a], 0x12
	ccmpnle 12, [0x88b], 0x8ec7
	ccmpnle 14, [0x921], 0x308bc163
	ccmpnle 7, [0xed1], 493243525
