default rel
	vcvtdq2pd xmm4, xmm0
	vcvtdq2pd xmm5, qword [0x2d3]
	vcvtdq2pd ymm7, oword [0x246]
	vcvtdq2pd ymm3, xmm4
	vcvtdq2pd xmm1, xmm3
	vcvtdq2pd xmm6, xmm6
	vcvtdq2pd ymm4, xmm6
	vcvtdq2pd ymm1, xmm7
	vcvtdq2pd xmm15, xmm8
	vcvtdq2pd ymm8, xmm10
	vcvtdq2pd xmm14, xmm8
	vcvtdq2pd ymm11, xmm10
	vcvtdq2pd xmm22, xmm16
	vcvtdq2pd ymm27, xmm21
	vcvtdq2pd xmm18, xmm28
	vcvtdq2pd ymm23, xmm25
	vcvtdq2pd xmm4{k4}, xmm3
	vcvtdq2pd ymm7{k4}, xmm7
	vcvtdq2pd zmm4{k2}, ymm3
	vcvtdq2pd xmm4{k2}{z}, qword [0x145]
	vcvtdq2pd ymm6{k1}{z}, xmm5
	vcvtdq2pd zmm1{k2}{z}, ymm2
	vcvtdq2pd zmm7, ymm5, {rn-sae}
	vcvtdq2pd xmm2, qword [eax+1]
	vcvtdq2pd xmm1, qword [eax+64]
	vcvtdq2pd ymm7, oword [eax+1]
	vcvtdq2pd ymm2, oword [eax+64]
	vcvtdq2pd xmm3, qword [eax+1]
	vcvtdq2pd xmm1, qword [eax+64]
	vcvtdq2pd ymm6, oword [eax+1]
	vcvtdq2pd ymm7, oword [eax+64]
	vcvtdq2pd zmm2, yword [eax+1]
	vcvtdq2pd zmm3, yword [eax+64]
	vcvtdq2pd xmm0, [0x3e3]
	vcvtdq2pd ymm5, [0xc9c]
	vcvtdq2pd xmm0, [0x6fe]
	vcvtdq2pd ymm4, [0x31d]
	vcvtdq2pd zmm4, [0x1b8]
