default rel
	vfnmsub132ph xmm5, xmm3, xmm7
	vfnmsub132ph xmm5, xmm7
	vfnmsub132ph xmm1, xmm6, xmm3
	vfnmsub132ph ymm0, ymm2, ymm2
	vfnmsub132ph ymm0, ymm2
	vfnmsub132ph ymm5, ymm7, ymm2
	vfnmsub132ph zmm2, zmm4, zword [0x89f]
	vfnmsub132ph zmm2, zword [0x89f]
	vfnmsub132ph zmm4, zmm7, zword [0x150]
	vfnmsub132ph xmm15, xmm9, xmm13
	vfnmsub132ph ymm11, ymm15, ymm10
	vfnmsub132ph zmm11, zmm10, zmm15
	vfnmsub132ph xmm19, xmm20, xmm25
	vfnmsub132ph ymm19, ymm31, ymm17
	vfnmsub132ph zmm18, zmm27, zmm27
	vfnmsub132ph xmm1{k4}, xmm7, oword [0xfd5]
	vfnmsub132ph ymm0{k4}, ymm4, yword [0x163]
	vfnmsub132ph zmm4{k3}, zmm5, zword [0x508]
	vfnmsub132ph xmm6{k5}{z}, xmm5, oword [0x5bc]
	vfnmsub132ph ymm1{k1}{z}, ymm7, ymm4
	vfnmsub132ph zmm4{k2}{z}, zmm1, zmm2
	vfnmsub132ph zmm6, zmm5, zmm2, {rn-sae}
	vfnmsub132ph xmm1, xmm3, oword [eax+1]
	vfnmsub132ph xmm2, xmm1, oword [eax+64]
	vfnmsub132ph ymm6, ymm1, yword [eax+1]
	vfnmsub132ph ymm6, ymm7, yword [eax+64]
	vfnmsub132ph zmm1, zmm2, zword [eax+1]
	vfnmsub132ph zmm2, zmm7, zword [eax+64]
	vfnmsub132ph xmm5, xmm7, [0x6fd]
	vfnmsub132ph ymm4, ymm5, [0x1ef]
	vfnmsub132ph zmm6, zmm0, [0xa24]
