	ccmps 2, byte [0x725], al
	ccmps 4, byte [0xae7], bl
	ccmps 11, bx, bp
	ccmps 2, dx, bp
	ccmps 14, ebp, eax
	ccmps 4, esi, ecx

%ifdef ERROR
	ccmps 8, rbx, rdx
	ccmps 1, rdx, rbp
	ccmps 2, r8b, r10b
	ccmps 2, r13w, r15w
	ccmps 8, r12d, r13d
	ccmps 0, r11, r14
	ccmps 1, r20b, r27b
	ccmps 4, r19w, r30w
	ccmps 8, r27d, r30d
	ccmps 1, r16, r17
	ccmps 6, qword [eax+1], rcx
	ccmps 1, qword [eax+64], rdi
	ccmps 1, rax, qword [eax+1]
	ccmps 2, rsi, qword [eax+64]
	ccmps 3, qword [eax+1], -122
	ccmps 8, qword [eax+64], 38
	ccmps 10, qword [eax+1], -340874139
	ccmps 6, qword [eax+64], 392403653
	ccmps 6, [0x3e5], rbp
	ccmps 11, rsi, [0x73d]
	ccmps 5, [0x4cd], -33
	ccmps 10, [0xc44], 71502190
%endif
