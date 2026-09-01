	vcvttss2si eax, dword [0xf91]
	vcvttss2si edx, xmm5
	vcvttss2si edx, xmm7
	vcvttss2si ebx, xmm5

%ifdef ERROR
	vcvttss2si rdx, xmm0
	vcvttss2si rdi, dword [0x774]
	vcvttss2si rsi, xmm5
	vcvttss2si rbp, xmm2
	vcvttss2si r8d, xmm8
	vcvttss2si r13, xmm9
	vcvttss2si r14d, xmm9
	vcvttss2si r15, xmm10
	vcvttss2si r18d, xmm26
	vcvttss2si r22, xmm23
	vcvttss2si r22d, xmm28
	vcvttss2si r19, xmm31
	vcvttss2si rdi, dword [eax+1]
	vcvttss2si rax, dword [eax+64]
	vcvttss2si rbx, dword [eax+1]
	vcvttss2si rax, dword [eax+64]
	vcvttss2si rbp, [0xa70]
	vcvttss2si rsi, [0x694]
%endif
