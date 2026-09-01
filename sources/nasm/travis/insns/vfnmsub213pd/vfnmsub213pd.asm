default rel
	vfnmsub213pd xmm2, xmm1, oword [0x107]
	vfnmsub213pd xmm1, xmm2, xmm3
	vfnmsub213pd ymm3, ymm3, ymm6
	vfnmsub213pd ymm3, ymm6, yword [0x3fb]
	vfnmsub213pd xmm6, xmm5, oword [0xe9a]
	vfnmsub213pd xmm0, xmm4, xmm6
	vfnmsub213pd ymm1, ymm5, ymm1
	vfnmsub213pd ymm2, ymm0, ymm0
	vfnmsub213pd xmm11, xmm15, xmm15
	vfnmsub213pd ymm11, ymm13, ymm11
	vfnmsub213pd xmm9, xmm12, xmm13
	vfnmsub213pd ymm10, ymm14, ymm11
	vfnmsub213pd xmm27, xmm21, xmm20
	vfnmsub213pd ymm27, ymm18, ymm25
	vfnmsub213pd xmm20, xmm21, xmm31
	vfnmsub213pd ymm18, ymm30, ymm25
	vfnmsub213pd xmm6{k4}, xmm1, xmm0
	vfnmsub213pd ymm2{k2}, ymm0, ymm2
	vfnmsub213pd zmm0{k6}, zmm5, zword [0x190]
	vfnmsub213pd xmm2{k2}{z}, xmm2, oword [0xe7e]
	vfnmsub213pd ymm6{k5}{z}, ymm7, ymm4
	vfnmsub213pd zmm7{k4}{z}, zmm2, zword [0xd59]
	vfnmsub213pd zmm4, zmm7, zmm4, {ru-sae}
	vfnmsub213pd xmm3, xmm0, oword [eax+1]
	vfnmsub213pd xmm6, xmm5, oword [eax+64]
	vfnmsub213pd ymm0, ymm7, yword [eax+1]
	vfnmsub213pd ymm5, ymm3, yword [eax+64]
	vfnmsub213pd xmm4, xmm0, oword [eax+1]
	vfnmsub213pd xmm7, xmm5, oword [eax+64]
	vfnmsub213pd ymm7, ymm3, yword [eax+1]
	vfnmsub213pd ymm4, ymm1, yword [eax+64]
	vfnmsub213pd zmm6, zmm6, zword [eax+1]
	vfnmsub213pd zmm5, zmm0, zword [eax+64]
	vfnmsub213pd xmm5, xmm2, [0x3bd]
	vfnmsub213pd ymm0, ymm2, [0x44a]
	vfnmsub213pd xmm5, xmm2, [0x3c5]
	vfnmsub213pd ymm5, ymm7, [0x319]
	vfnmsub213pd zmm7, zmm0, [0x36b]
