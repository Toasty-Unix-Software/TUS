default rel
	vcvtqq2ph xmm3, xmm1
	vcvtqq2ph xmm3, xmm2
	vcvtqq2ph xmm7, yword [0x26b]
	vcvtqq2ph xmm2, yword [0xcba]
	vcvtqq2ph xmm5, zmm6
	vcvtqq2ph xmm6, zmm7
	vcvtqq2ph xmm11, xmm11
	vcvtqq2ph xmm12, ymm15
	vcvtqq2ph xmm11, zmm10
	vcvtqq2ph xmm19, xmm26
	vcvtqq2ph xmm27, ymm17
	vcvtqq2ph xmm30, zmm21
	vcvtqq2ph xmm3{k6}, xmm0
	vcvtqq2ph xmm5{k2}, ymm7
	vcvtqq2ph xmm6{k1}, zmm0
	vcvtqq2ph xmm0{k4}{z}, xmm1
	vcvtqq2ph xmm4{k2}{z}, ymm1
	vcvtqq2ph xmm5{k6}{z}, zmm0
	vcvtqq2ph xmm0, zmm1, {rn-sae}
	vcvtqq2ph xmm2, oword [eax+1]
	vcvtqq2ph xmm2, oword [eax+64]
	vcvtqq2ph xmm6, yword [eax+1]
	vcvtqq2ph xmm4, yword [eax+64]
	vcvtqq2ph xmm5, zword [eax+1]
	vcvtqq2ph xmm3, zword [eax+64]
	vcvtqq2ph xmm6, [0xfc5]
	vcvtqq2ph xmm0, [0xa78]
	vcvtqq2ph xmm6, [0x9c3]
