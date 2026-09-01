default rel
	vcvtsi2sd xmm5, xmm1, dword [0x5f6]
	vcvtsi2sd xmm5, dword [0x5f6]
	vcvtsi2sd xmm3, xmm5, esi
	vcvtsi2sd xmm0, xmm3, dword [0x16f]
	vcvtsi2sd xmm0, dword [0x16f]
	vcvtsi2sd xmm1, xmm1, dword [0xf83]
	vcvtsi2sd xmm2, xmm5, rax
	vcvtsi2sd xmm2, rax
	vcvtsi2sd xmm2, xmm2, qword [0xbd7]
	vcvtsi2sd xmm0, xmm7, rbx
	vcvtsi2sd xmm0, rbx
	vcvtsi2sd xmm1, xmm0, rbx
	vcvtsi2sd xmm15, xmm11, r14d
	vcvtsi2sd xmm13, xmm12, dword [0xf6e]
	vcvtsi2sd xmm12, xmm9, r12
	vcvtsi2sd xmm10, xmm14, r10
	vcvtsi2sd xmm28, xmm31, r23d
	vcvtsi2sd xmm27, xmm24, dword [0x6d8]
	vcvtsi2sd xmm28, xmm27, r16
	vcvtsi2sd xmm30, xmm20, r16
	vcvtsi2sd xmm4, xmm1, rcx, {rd-sae}
	vcvtsi2sd xmm6, xmm5, dword [eax+1]
	vcvtsi2sd xmm1, xmm7, dword [eax+64]
	vcvtsi2sd xmm1, xmm6, dword [eax+1]
	vcvtsi2sd xmm5, xmm4, dword [eax+64]
	vcvtsi2sd xmm1, xmm4, qword [eax+1]
	vcvtsi2sd xmm4, xmm6, qword [eax+64]
	vcvtsi2sd xmm2, xmm2, qword [eax+1]
	vcvtsi2sd xmm6, xmm7, qword [eax+64]
	vcvtsi2sd xmm6, xmm3, [0x26e]
	vcvtsi2sd xmm4, xmm7, [0x597]
	vcvtsi2sd xmm1, xmm4, [0xd43]
	vcvtsi2sd xmm4, xmm7, [0xf18]
