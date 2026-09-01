default rel
	vcvtuqq2ps xmm2, xmm0
	vcvtuqq2ps xmm7, oword [0x4e1]
	vcvtuqq2ps xmm1, ymm7
	vcvtuqq2ps xmm3, ymm4
	vcvtuqq2ps ymm1, zmm0
	vcvtuqq2ps ymm0, zword [0x8ce]
	vcvtuqq2ps xmm10, xmm9
	vcvtuqq2ps xmm9, ymm11
	vcvtuqq2ps ymm13, zmm11
	vcvtuqq2ps xmm20, xmm31
	vcvtuqq2ps xmm21, ymm16
	vcvtuqq2ps ymm19, zmm20
	vcvtuqq2ps xmm0{k3}, oword [0x71b]
	vcvtuqq2ps xmm7{k1}, ymm7
	vcvtuqq2ps ymm7{k7}, zword [0x5f8]
	vcvtuqq2ps xmm0{k5}{z}, xmm1
	vcvtuqq2ps xmm4{k1}{z}, ymm3
	vcvtuqq2ps ymm3{k6}{z}, zmm1
	vcvtuqq2ps ymm6, zmm6, {rz-sae}
	vcvtuqq2ps xmm5, oword [eax+1]
	vcvtuqq2ps xmm1, oword [eax+64]
	vcvtuqq2ps xmm7, yword [eax+1]
	vcvtuqq2ps xmm1, yword [eax+64]
	vcvtuqq2ps ymm6, zword [eax+1]
	vcvtuqq2ps ymm0, zword [eax+64]
	vcvtuqq2ps xmm1, [0x995]
	vcvtuqq2ps xmm6, [0xa64]
	vcvtuqq2ps ymm7, [0x619]
