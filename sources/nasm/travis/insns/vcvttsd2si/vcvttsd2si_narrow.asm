	vcvttsd2si edx, xmm6
	vcvttsd2si eax, qword [0x854]
	vcvttsd2si ebx, xmm7
	vcvttsd2si eax, xmm4

%ifdef ERROR
	vcvttsd2si rsi, qword [0x49f]
	vcvttsd2si rdx, xmm5
	vcvttsd2si rax, xmm7
	vcvttsd2si rbp, xmm1
	vcvttsd2si r14d, xmm14
	vcvttsd2si r9, xmm11
	vcvttsd2si r15d, xmm15
	vcvttsd2si r15, xmm14
	vcvttsd2si r27d, xmm19
	vcvttsd2si r25, xmm29
	vcvttsd2si r24d, xmm17
	vcvttsd2si r16, xmm25
	vcvttsd2si rbx, qword [eax+1]
	vcvttsd2si rax, qword [eax+64]
	vcvttsd2si rax, qword [eax+1]
	vcvttsd2si rax, qword [eax+64]
	vcvttsd2si rdi, [0x6c8]
	vcvttsd2si rax, [0xc13]
%endif
