	ccmpnc 4, al, cl
	ccmpnc 15, bl, al
	ccmpnc 13, si, si
	ccmpnc 13, word [0xa10], dx
	ccmpnc 4, edx, esi
	ccmpnc 15, dword [0xdaa], esi

%ifdef ERROR
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
	ccmpnc 6, qword [eax+1], rax
	ccmpnc 13, qword [eax+64], rax
	ccmpnc 4, rdx, qword [eax+1]
	ccmpnc 0, rbx, qword [eax+64]
	ccmpnc 14, qword [eax+1], 112
	ccmpnc 5, qword [eax+64], -17
	ccmpnc 1, qword [eax+1], -222496991
	ccmpnc 1, qword [eax+64], -377625458
	ccmpnc 14, [0x35f], rdi
	ccmpnc 0, rbp, [0xe40]
	ccmpnc 3, [0x349], -124
	ccmpnc 14, [0xb4d], -374510123
%endif
