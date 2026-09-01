default rel
	vcvtss2usi eax, dword [0xb7c]
	vcvtss2usi ebx, xmm7
	vcvtss2usi rdx, xmm2
	vcvtss2usi rdx, dword [0x29e]
	vcvtss2usi r15d, xmm8
	vcvtss2usi r13, xmm15
	vcvtss2usi r22d, xmm26
	vcvtss2usi r17, xmm22
	vcvtss2usi edi, xmm1, {rd-sae}
	vcvtss2usi rsi, xmm1, {rz-sae}
	vcvtss2usi esi, dword [eax+1]
	vcvtss2usi eax, dword [eax+64]
	vcvtss2usi rsi, dword [eax+1]
	vcvtss2usi rsi, dword [eax+64]
	vcvtss2usi esi, [0xcbb]
	vcvtss2usi rsi, [0xf82]
