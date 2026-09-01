	vcvttsd2usi ecx, qword [0x7b2]
	vcvttsd2usi eax, xmm7

%ifdef ERROR
	vcvttsd2usi rcx, xmm5
	vcvttsd2usi rax, qword [0x4d4]
	vcvttsd2usi r9d, xmm12
	vcvttsd2usi r12, xmm13
	vcvttsd2usi r25d, xmm21
	vcvttsd2usi r18, xmm16
	vcvttsd2usi rax, qword [eax+1]
	vcvttsd2usi rdx, qword [eax+64]
	vcvttsd2usi rsi, [0xcde]
%endif
