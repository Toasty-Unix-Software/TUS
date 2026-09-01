	ccmpnle 2, byte [0x810], cl
	ccmpnle 5, byte [0xa07], dl
	ccmpnle 9, bx, ax
	ccmpnle 9, word [0x337], dx
	ccmpnle 9, eax, edx
	ccmpnle 7, ecx, edi

%ifdef ERROR
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
	ccmpnle 5, qword [eax+1], rdx
	ccmpnle 15, qword [eax+64], rcx
	ccmpnle 10, rsi, qword [eax+1]
	ccmpnle 3, rax, qword [eax+64]
	ccmpnle 11, qword [eax+1], 112
	ccmpnle 11, qword [eax+64], 93
	ccmpnle 14, qword [eax+1], -38193403
	ccmpnle 4, qword [eax+64], 48699356
	ccmpnle 10, [0xac3], rcx
	ccmpnle 6, rcx, [0xa7b]
	ccmpnle 6, [0xa32], 1
	ccmpnle 7, [0xed1], 493243525
%endif
