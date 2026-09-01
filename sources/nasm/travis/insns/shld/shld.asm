default rel
	shld si, di, 0x19
	shld bx, bp, 0x35
	shld dword [0xde4], esi, 0xc2
	shld edx, ebx, 0x11
	shld rdi, rax, 0xd1
	shld rbp, rbp, 0x56
	shld dx, cx, cl
	shld word [0xe19], bp, cl
	shld r13w, r10w, 0x57
	shld r13d, r13d, 0xdc
	shld r14, r9, 0x65
	shld r13w, r11w, cl
	shld r21w, r27w, 0x18
	shld r24d, r28d, 0x54
	shld r16, r22, 0xdc
	shld r20w, r31w, cl
	shld word [eax+1], ax, 0xff
	shld word [eax+64], di, 0xe9
	shld dword [eax+1], ecx, 0xc1
	shld dword [eax+64], ecx, 0xf4
	shld qword [eax+1], rbx, 0xa0
	shld qword [eax+64], rbp, 0x7
	shld word [eax+1], bp, cl
	shld word [eax+64], cx, cl
	shld dword [eax+1], ecx, cl
	shld dword [eax+64], edi, cl
	shld qword [eax+1], rdx, cl
	shld qword [eax+64], rbp, cl
	shld ax, word [eax+1], bp, 0x56
	shld cx, word [eax+64], ax, 0x8c
	shld ecx, dword [eax+1], ecx, 0x4d
	shld esi, dword [eax+64], ecx, 0x92
	shld rdx, qword [eax+1], rdx, 0x45
	shld rsi, qword [eax+64], rcx, 0xbf
	shld di, word [eax+1], bp, cl
	shld ax, word [eax+64], ax, cl
	shld ebx, dword [eax+1], eax, cl
	shld eax, dword [eax+64], eax, cl
	shld rbx, qword [eax+1], rbp, cl
	shld rbp, qword [eax+64], rbx, cl
	shld [0xe17], si, 0x8e
	shld [0xb1a], esi, 0x93
	shld [0x9f9], rbx, 0x68
	shld [0x413], si, cl
	shld [0x23d], edi, cl
	shld [0x552], rdi, cl
	shld dx, [0x247], di, 0x92
	shld ecx, [0x1b8], ebp, 0x61
	shld rax, [0x6cb], rbx, 0x52
	shld si, [0x1a9], ax, cl
	shld edi, [0x773], eax, cl
	shld rbx, [0xde0], rbp, cl
