	ccmpnae 15, byte [0xc08], dl
	ccmpnae 3, cl, bl
	ccmpnae 5, cx, dx
	ccmpnae 14, dx, si
	ccmpnae 15, ecx, esi
	ccmpnae 6, ebx, ebp

%ifdef ERROR
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
	ccmpnae 6, qword [eax+1], rbp
	ccmpnae 15, qword [eax+64], rbp
	ccmpnae 2, rbp, qword [eax+1]
	ccmpnae 12, rbx, qword [eax+64]
	ccmpnae 14, qword [eax+1], -113
	ccmpnae 5, qword [eax+64], -79
	ccmpnae 6, qword [eax+1], 87072369
	ccmpnae 10, qword [eax+64], 67641877
	ccmpnae 9, [0x3db], rcx
	ccmpnae 3, rsi, [0xf44]
	ccmpnae 0, [0xbad], 22
	ccmpnae 13, [0x3ae], 382090881
%endif
