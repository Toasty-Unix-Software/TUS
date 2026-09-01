default rel
	vcvtusi2sh xmm7, xmm0, ebx
	vcvtusi2sh xmm7, ebx
	vcvtusi2sh xmm5, xmm2, edi
	vcvtusi2sh xmm2, xmm5, rbp
	vcvtusi2sh xmm2, rbp
	vcvtusi2sh xmm1, xmm3, qword [0x83f]
	vcvtusi2sh xmm8, xmm10, r11d
	vcvtusi2sh xmm13, xmm10, r12
	vcvtusi2sh xmm19, xmm22, r24d
	vcvtusi2sh xmm25, xmm16, r19
	vcvtusi2sh xmm4, xmm4, ecx, {rz-sae}
	vcvtusi2sh xmm2, xmm1, rdi, {rn-sae}
	vcvtusi2sh xmm1, xmm6, dword [eax+1]
	vcvtusi2sh xmm7, xmm5, dword [eax+64]
	vcvtusi2sh xmm2, xmm6, qword [eax+1]
	vcvtusi2sh xmm4, xmm2, qword [eax+64]
	vcvtusi2sh xmm3, xmm7, [0xba8]
	vcvtusi2sh xmm6, xmm0, [0x2b2]
