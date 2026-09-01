	shld si, di, 0x19
	shld bx, bp, 0x35
	shld dword [0xde4], esi, 0xc2
	shld edx, ebx, 0x11
	shld dx, cx, cl
	shld word [0xe19], bp, cl

%ifdef ERROR
	shld rdi, rax, 0xd1
	shld rbp, rbp, 0x56
	shld r13w, r10w, 0x57
	shld r13d, r13d, 0xdc
	shld r14, r9, 0x65
	shld r13w, r11w, cl
	shld r21w, r27w, 0x18
	shld r24d, r28d, 0x54
	shld r16, r22, 0xdc
	shld r20w, r31w, cl
	shld qword [eax+1], rbx, 0xa0
	shld qword [eax+64], rbp, 0x7
	shld qword [eax+1], rdx, cl
	shld qword [eax+64], rbp, cl
	shld rdx, qword [eax+1], rdx, 0x45
	shld rsi, qword [eax+64], rcx, 0xbf
	shld rbx, qword [eax+1], rbp, cl
	shld rbp, qword [eax+64], rbx, cl
	shld [0x9f9], rbx, 0x68
	shld [0x552], rdi, cl
	shld rax, [0x6cb], rbx, 0x52
	shld rbx, [0xde0], rbp, cl
%endif
