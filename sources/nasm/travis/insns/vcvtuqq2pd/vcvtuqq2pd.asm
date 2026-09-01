default rel
	vcvtuqq2pd xmm7, oword [0xfeb]
	vcvtuqq2pd xmm0, xmm6
	vcvtuqq2pd ymm6, ymm1
	vcvtuqq2pd ymm3, ymm4
	vcvtuqq2pd zmm5, zword [0xcc5]
	vcvtuqq2pd zmm4, zmm6
	vcvtuqq2pd xmm8, xmm12
	vcvtuqq2pd ymm9, ymm12
	vcvtuqq2pd zmm15, zmm14
	vcvtuqq2pd xmm23, xmm19
	vcvtuqq2pd ymm19, ymm21
	vcvtuqq2pd zmm27, zmm18
	vcvtuqq2pd xmm4{k4}, oword [0xf10]
	vcvtuqq2pd ymm1{k2}, ymm6
	vcvtuqq2pd zmm6{k1}, zmm4
	vcvtuqq2pd xmm7{k4}{z}, oword [0xca7]
	vcvtuqq2pd ymm5{k5}{z}, yword [0x8e8]
	vcvtuqq2pd zmm2{k4}{z}, zword [0xba8]
	vcvtuqq2pd zmm4, zmm2, {rn-sae}
	vcvtuqq2pd xmm1, oword [eax+1]
	vcvtuqq2pd xmm6, oword [eax+64]
	vcvtuqq2pd ymm5, yword [eax+1]
	vcvtuqq2pd ymm2, yword [eax+64]
	vcvtuqq2pd zmm5, zword [eax+1]
	vcvtuqq2pd zmm0, zword [eax+64]
	vcvtuqq2pd xmm1, [0xe6a]
	vcvtuqq2pd ymm4, [0x3b3]
	vcvtuqq2pd zmm5, [0xf2a]
