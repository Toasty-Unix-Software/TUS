	sbb byte [0x152], al
	sbb byte [0xc9c], dl
	sbb cx, bx
	sbb cx, si
	sbb edx, ebx
	sbb esi, ebp

%ifdef ERROR
	sbb qword [0xd3b], rcx
	sbb qword [0x514], rax
	sbb r9b, r9b
	sbb r15w, r10w
	sbb r14d, r11d
	sbb r9, r12
	sbb r23b, r16b
	sbb r17w, r24w
	sbb r21d, r20d
	sbb r28, r31
	sbb qword [eax+1], rdi
	sbb qword [eax+64], rdx
	sbb rsi, qword [eax+1]
	sbb rsi, qword [eax+64]
	sbb qword [eax+1], -113
	sbb qword [eax+64], 128
	sbb qword [eax+1], 138481480
	sbb qword [eax+64], 479373787
	sbb rax, rbp, qword [eax+1]
	sbb rcx, rax, qword [eax+64]
	sbb rax, qword [eax+1], rdx
	sbb rdx, qword [eax+64], rsi
	sbb rcx, qword [eax+1], 31
	sbb rbx, qword [eax+64], -22
	sbb rcx, qword [eax+1], -304481604
	sbb rdx, qword [eax+64], 31411479
	sbb [0xf6d], rsi
	sbb rax, [0x5e8]
	sbb [0xd6f], 111
	sbb [0xeed], 503962257
	sbb rdx, rbp, [0x995]
	sbb rdi, [0x541], rsi
	sbb rbp, [0x719], -21
	sbb rax, [0x3cc], 363984859
%endif
