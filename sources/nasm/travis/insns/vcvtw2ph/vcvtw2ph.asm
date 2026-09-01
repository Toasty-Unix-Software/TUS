default rel
	vcvtw2ph xmm4, xmm6
	vcvtw2ph xmm3, oword [0xcc1]
	vcvtw2ph ymm0, ymm5
	vcvtw2ph ymm7, ymm7
	vcvtw2ph zmm1, zmm3
	vcvtw2ph zmm2, zword [0xe49]
	vcvtw2ph xmm13, xmm9
	vcvtw2ph ymm11, ymm14
	vcvtw2ph zmm13, zmm11
	vcvtw2ph xmm17, xmm17
	vcvtw2ph ymm17, ymm16
	vcvtw2ph zmm20, zmm28
	vcvtw2ph xmm7{k3}, xmm6
	vcvtw2ph ymm6{k2}, yword [0x9b4]
	vcvtw2ph zmm4{k1}, zmm0
	vcvtw2ph xmm3{k4}{z}, xmm5
	vcvtw2ph ymm4{k2}{z}, yword [0x9cd]
	vcvtw2ph zmm5{k1}{z}, zmm2
	vcvtw2ph zmm3, zmm6, {rd-sae}
	vcvtw2ph xmm6, oword [eax+1]
	vcvtw2ph xmm2, oword [eax+64]
	vcvtw2ph ymm6, yword [eax+1]
	vcvtw2ph ymm5, yword [eax+64]
	vcvtw2ph zmm0, zword [eax+1]
	vcvtw2ph zmm6, zword [eax+64]
	vcvtw2ph xmm0, [0xc1c]
	vcvtw2ph ymm1, [0xf58]
	vcvtw2ph zmm7, [0x652]
