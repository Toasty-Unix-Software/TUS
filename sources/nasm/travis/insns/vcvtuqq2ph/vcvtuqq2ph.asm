default rel
	vcvtuqq2ph xmm1, xmm7
	vcvtuqq2ph xmm4, xmm6
	vcvtuqq2ph xmm1, yword [0x163]
	vcvtuqq2ph xmm3, yword [0x127]
	vcvtuqq2ph xmm6, zmm1
	vcvtuqq2ph xmm6, zmm2
	vcvtuqq2ph xmm8, xmm11
	vcvtuqq2ph xmm15, ymm10
	vcvtuqq2ph xmm10, zmm8
	vcvtuqq2ph xmm17, xmm27
	vcvtuqq2ph xmm19, ymm26
	vcvtuqq2ph xmm17, zmm28
	vcvtuqq2ph xmm1{k7}, xmm1
	vcvtuqq2ph xmm5{k1}, ymm7
	vcvtuqq2ph xmm5{k5}, zmm5
	vcvtuqq2ph xmm4{k6}{z}, xmm4
	vcvtuqq2ph xmm2{k5}{z}, ymm2
	vcvtuqq2ph xmm2{k6}{z}, zmm7
	vcvtuqq2ph xmm2, oword [eax+1]
	vcvtuqq2ph xmm7, oword [eax+64]
	vcvtuqq2ph xmm4, yword [eax+1]
	vcvtuqq2ph xmm7, yword [eax+64]
	vcvtuqq2ph xmm7, zword [eax+1]
	vcvtuqq2ph xmm7, zword [eax+64]
	vcvtuqq2ph xmm2, [0xecb]
	vcvtuqq2ph xmm0, [0x5c8]
	vcvtuqq2ph xmm1, [0x777]
