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

%ifdef ERROR
	vcvtsi2sd xmm15, xmm11, r14d
	vcvtsi2sd xmm13, xmm12, dword [0xf6e]
	vcvtsi2sd xmm12, xmm9, r12
	vcvtsi2sd xmm10, xmm14, r10
	vcvtsi2sd xmm28, xmm31, r23d
	vcvtsi2sd xmm27, xmm24, dword [0x6d8]
	vcvtsi2sd xmm28, xmm27, r16
	vcvtsi2sd xmm30, xmm20, r16
%endif
