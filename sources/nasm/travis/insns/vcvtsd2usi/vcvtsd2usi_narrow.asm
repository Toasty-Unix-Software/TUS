	vcvtsd2usi edi, xmm6
	vcvtsd2usi ebp, xmm3

%ifdef ERROR
	vcvtsd2usi rdx, xmm3
	vcvtsd2usi rax, xmm5
	vcvtsd2usi r12d, xmm14
	vcvtsd2usi r10, xmm13
	vcvtsd2usi r24d, xmm23
	vcvtsd2usi r28, xmm30
	vcvtsd2usi rdi, qword [eax+1]
	vcvtsd2usi rsi, qword [eax+64]
	vcvtsd2usi rbp, [0x686]
%endif
