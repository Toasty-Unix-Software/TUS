default rel
	ccmpnc 4, al, cl
	ccmpnc 15, bl, al
	ccmpnc 13, si, si
	ccmpnc 13, word [0xa10], dx
	ccmpnc 4, edx, esi
	ccmpnc 15, dword [0xdaa], esi
	ccmpnc 6, rbp, rax
	ccmpnc 15, rcx, rbp
	ccmpnc 0, r9b, r10b
	ccmpnc 2, r13w, r9w
	ccmpnc 9, r12d, r14d
	ccmpnc 9, r10, r10
	ccmpnc 11, r27b, r29b
	ccmpnc 14, r22w, r21w
	ccmpnc 9, r27d, r28d
	ccmpnc 8, r25, r31
	ccmpnc 0, byte [eax+1], dl
	ccmpnc 1, byte [eax+64], cl
	ccmpnc 5, word [eax+1], dx
	ccmpnc 6, word [eax+64], bx
	ccmpnc 2, dword [eax+1], ebx
	ccmpnc 1, dword [eax+64], ebp
	ccmpnc 6, qword [eax+1], rax
	ccmpnc 13, qword [eax+64], rax
	ccmpnc 13, dl, byte [eax+1]
	ccmpnc 6, bl, byte [eax+64]
	ccmpnc 1, di, word [eax+1]
	ccmpnc 12, cx, word [eax+64]
	ccmpnc 9, eax, dword [eax+1]
	ccmpnc 2, edx, dword [eax+64]
	ccmpnc 4, rdx, qword [eax+1]
	ccmpnc 0, rbx, qword [eax+64]
	ccmpnc 10, word [eax+1], -44
	ccmpnc 13, word [eax+64], 24
	ccmpnc 9, dword [eax+1], -118
	ccmpnc 10, dword [eax+64], 109
	ccmpnc 14, qword [eax+1], 112
	ccmpnc 5, qword [eax+64], -17
	ccmpnc 3, byte [eax+1], 0x78
	ccmpnc 12, byte [eax+64], 0xef
	ccmpnc 11, word [eax+1], 0x67c9
	ccmpnc 12, word [eax+64], 0x8eae
	ccmpnc 11, dword [eax+1], 0x13b0166f
	ccmpnc 15, dword [eax+64], 0x31bf390e
	ccmpnc 1, qword [eax+1], -222496991
	ccmpnc 1, qword [eax+64], -377625458
	ccmpnc 2, [0xd9f], bl
	ccmpnc 4, [0x2b5], bp
	ccmpnc 12, [0x411], ebp
	ccmpnc 14, [0x35f], rdi
	ccmpnc 15, cl, [0xe98]
	ccmpnc 0, si, [0x3b5]
	ccmpnc 1, ebp, [0x3f4]
	ccmpnc 0, rbp, [0xe40]
	ccmpnc 15, [0xb88], 113
	ccmpnc 15, [0xbab], 11
	ccmpnc 3, [0x349], -124
	ccmpnc 5, [0x1ae], 0xa8
	ccmpnc 1, [0xa88], 0xad4c
	ccmpnc 0, [0x9c5], 0x39b37681
	ccmpnc 14, [0xb4d], -374510123
