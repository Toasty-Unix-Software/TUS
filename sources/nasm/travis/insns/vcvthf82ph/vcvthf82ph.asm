default rel
	vcvthf82ph xmm6, xmm5
	vcvthf82ph xmm2, xmm2
	vcvthf82ph ymm2, xmm7
	vcvthf82ph ymm3, xmm6
	vcvthf82ph zmm7, ymm0
	vcvthf82ph zmm3, ymm4
	vcvthf82ph xmm10, xmm12
	vcvthf82ph ymm8, xmm13
	vcvthf82ph zmm14, ymm8
	vcvthf82ph xmm16, xmm28
	vcvthf82ph ymm18, xmm31
	vcvthf82ph zmm30, ymm18
	vcvthf82ph xmm7{k2}, qword [0x8a9]
	vcvthf82ph ymm7{k5}, xmm3
	vcvthf82ph zmm2{k4}, ymm4
	vcvthf82ph xmm4{k7}{z}, xmm4
	vcvthf82ph ymm6{k1}{z}, xmm2
	vcvthf82ph zmm6{k4}{z}, ymm1
	vcvthf82ph xmm3, qword [eax+1]
	vcvthf82ph xmm7, qword [eax+64]
	vcvthf82ph ymm4, oword [eax+1]
	vcvthf82ph ymm7, oword [eax+64]
	vcvthf82ph zmm0, yword [eax+1]
	vcvthf82ph zmm5, yword [eax+64]
	vcvthf82ph xmm6, [0xdf2]
	vcvthf82ph ymm6, [0xd63]
	vcvthf82ph zmm6, [0xce0]
