	vcvtss2si esi, xmm0
	vcvtss2si eax, xmm4
	vcvtss2si ecx, xmm7
	vcvtss2si edx, xmm0

%ifdef ERROR
	vcvtss2si rbx, xmm4
	vcvtss2si rsi, dword [0xe10]
	vcvtss2si rbx, xmm7
	vcvtss2si rdi, xmm0
	vcvtss2si r12d, xmm13
	vcvtss2si r10, xmm13
	vcvtss2si r8d, xmm8
	vcvtss2si r11, xmm12
	vcvtss2si r18d, xmm19
	vcvtss2si r29, xmm25
	vcvtss2si r24d, xmm18
	vcvtss2si r19, xmm27
	vcvtss2si rdi, dword [eax+1]
	vcvtss2si rax, dword [eax+64]
	vcvtss2si rdx, dword [eax+1]
	vcvtss2si rdi, dword [eax+64]
	vcvtss2si rcx, [0xcf3]
	vcvtss2si rax, [0xd1c]
%endif
