default rel
	vcvttss2sis eax, xmm4
	vcvttss2sis edi, xmm1
	vcvttss2sis rdi, xmm0
	vcvttss2sis rcx, xmm4
	vcvttss2sis r12d, xmm15
	vcvttss2sis r15, xmm13
	vcvttss2sis r23d, xmm16
	vcvttss2sis r29, xmm23
	vcvttss2sis ebp, xmm5, {sae}
	vcvttss2sis rsi, xmm2, {sae}
	vcvttss2sis edx, dword [eax+1]
	vcvttss2sis ecx, dword [eax+64]
	vcvttss2sis rdi, dword [eax+1]
	vcvttss2sis rsi, dword [eax+64]
	vcvttss2sis ecx, [0xbbf]
	vcvttss2sis rcx, [0x2b6]
