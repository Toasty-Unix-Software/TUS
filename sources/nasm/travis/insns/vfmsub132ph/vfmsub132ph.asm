default rel
	vfmsub132ph xmm5, xmm0, xmm3
	vfmsub132ph xmm5, xmm3
	vfmsub132ph xmm3, xmm5, xmm7
	vfmsub132ph ymm4, ymm5, ymm3
	vfmsub132ph ymm4, ymm3
	vfmsub132ph ymm1, ymm0, ymm6
	vfmsub132ph zmm3, zmm0, zmm0
	vfmsub132ph zmm3, zmm0
	vfmsub132ph zmm6, zmm1, zmm7
	vfmsub132ph xmm13, xmm9, xmm9
	vfmsub132ph ymm9, ymm10, ymm14
	vfmsub132ph zmm11, zmm13, zmm15
	vfmsub132ph xmm31, xmm27, xmm21
	vfmsub132ph ymm21, ymm18, ymm19
	vfmsub132ph zmm20, zmm19, zmm18
	vfmsub132ph xmm7{k4}, xmm3, xmm3
	vfmsub132ph ymm5{k5}, ymm4, ymm3
	vfmsub132ph zmm5{k4}, zmm4, zword [0xe68]
	vfmsub132ph xmm4{k4}{z}, xmm6, oword [0xfbe]
	vfmsub132ph ymm7{k5}{z}, ymm1, ymm0
	vfmsub132ph zmm3{k1}{z}, zmm4, zmm2
	vfmsub132ph zmm3, zmm3, zmm3, {rn-sae}
	vfmsub132ph xmm0, xmm6, oword [eax+1]
	vfmsub132ph xmm7, xmm6, oword [eax+64]
	vfmsub132ph ymm1, ymm5, yword [eax+1]
	vfmsub132ph ymm7, ymm7, yword [eax+64]
	vfmsub132ph zmm0, zmm6, zword [eax+1]
	vfmsub132ph zmm4, zmm6, zword [eax+64]
	vfmsub132ph xmm3, xmm0, [0x28a]
	vfmsub132ph ymm1, ymm4, [0xe1e]
	vfmsub132ph zmm4, zmm2, [0x4cb]
