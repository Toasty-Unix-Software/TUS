	vcvttss2usi ebp, xmm0
	vcvttss2usi edi, dword [0x2d6]

%ifdef ERROR
	vcvttss2usi rsi, xmm0
	vcvttss2usi rcx, dword [0x4ba]
	vcvttss2usi r8d, xmm8
	vcvttss2usi r8, xmm11
	vcvttss2usi r30d, xmm28
	vcvttss2usi r28, xmm18
	vcvttss2usi rax, dword [eax+1]
	vcvttss2usi rdi, dword [eax+64]
	vcvttss2usi rbp, [0x63b]
%endif
