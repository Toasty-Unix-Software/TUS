default rel
	vcvtsi2sh xmm0, xmm5, eax
	vcvtsi2sh xmm0, eax
	vcvtsi2sh xmm1, xmm7, eax
	vcvtsi2sh xmm1, xmm4, rcx
	vcvtsi2sh xmm1, rcx
	vcvtsi2sh xmm5, xmm7, rdx
	vcvtsi2sh xmm12, xmm12, r9d
	vcvtsi2sh xmm11, xmm15, r8
	vcvtsi2sh xmm26, xmm19, r22d
	vcvtsi2sh xmm18, xmm20, r22
	vcvtsi2sh xmm1, xmm2, ebp, {ru-sae}
	vcvtsi2sh xmm6, xmm6, rax, {rn-sae}
	vcvtsi2sh xmm2, xmm6, dword [eax+1]
	vcvtsi2sh xmm6, xmm5, dword [eax+64]
	vcvtsi2sh xmm3, xmm3, qword [eax+1]
	vcvtsi2sh xmm0, xmm3, qword [eax+64]
	vcvtsi2sh xmm2, xmm2, [0x10c]
	vcvtsi2sh xmm6, xmm0, [0xf81]
