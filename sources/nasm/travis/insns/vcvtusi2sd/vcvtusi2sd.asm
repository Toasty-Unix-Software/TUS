default rel
	vcvtusi2sd xmm6, xmm7, dword [0xdbb]
	vcvtusi2sd xmm6, dword [0xdbb]
	vcvtusi2sd xmm4, xmm5, dword [0x608]
	vcvtusi2sd xmm3, xmm7, rbp
	vcvtusi2sd xmm3, rbp
	vcvtusi2sd xmm1, xmm0, rcx
	vcvtusi2sd xmm10, xmm14, r12d
	vcvtusi2sd xmm13, xmm10, r8
	vcvtusi2sd xmm19, xmm17, r27d
	vcvtusi2sd xmm16, xmm17, r16
	vcvtusi2sd xmm4, xmm0, ebp, {ru-sae}
	vcvtusi2sd xmm5, xmm4, rdx, {rz-sae}
	vcvtusi2sd xmm1, xmm5, dword [eax+1]
	vcvtusi2sd xmm5, xmm6, dword [eax+64]
	vcvtusi2sd xmm2, xmm1, qword [eax+1]
	vcvtusi2sd xmm3, xmm6, qword [eax+64]
	vcvtusi2sd xmm7, xmm4, [0xfd0]
	vcvtusi2sd xmm5, xmm7, [0xa8a]
