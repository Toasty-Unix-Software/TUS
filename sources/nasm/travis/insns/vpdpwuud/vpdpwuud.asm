default rel
	vpdpwuud xmm3, xmm6, xmm3
	vpdpwuud xmm6, xmm4, xmm3
	vpdpwuud ymm0, ymm5, yword [0x66e]
	vpdpwuud ymm0, ymm4, yword [0xb71]
	vpdpwuud xmm5, xmm1, xmm0
	vpdpwuud xmm1, xmm1, xmm4
	vpdpwuud ymm0, ymm6, ymm2
	vpdpwuud ymm5, ymm7, ymm2
	vpdpwuud xmm13, xmm9, xmm14
	vpdpwuud ymm12, ymm14, ymm9
	vpdpwuud xmm15, xmm14, xmm15
	vpdpwuud ymm8, ymm14, ymm8
	vpdpwuud xmm31, xmm27, xmm26
	vpdpwuud ymm23, ymm19, ymm30
	vpdpwuud xmm24, xmm22, xmm28
	vpdpwuud ymm23, ymm19, ymm19
	vpdpwuud xmm3{k1}, xmm7, xmm3
	vpdpwuud ymm6{k1}, ymm6, ymm6
	vpdpwuud zmm3{k6}, zmm5, zmm1
	vpdpwuud xmm3{k7}{z}, xmm3, xmm2
	vpdpwuud ymm7{k1}{z}, ymm6, ymm1
	vpdpwuud zmm5{k7}{z}, zmm6, zmm0
	vpdpwuud xmm4, xmm0, oword [eax+1]
	vpdpwuud xmm3, xmm5, oword [eax+64]
	vpdpwuud ymm3, ymm7, yword [eax+1]
	vpdpwuud ymm3, ymm6, yword [eax+64]
	vpdpwuud xmm0, xmm2, oword [eax+1]
	vpdpwuud xmm0, xmm1, oword [eax+64]
	vpdpwuud ymm5, ymm3, yword [eax+1]
	vpdpwuud ymm4, ymm6, yword [eax+64]
	vpdpwuud zmm5, zmm5, zword [eax+1]
	vpdpwuud zmm0, zmm0, zword [eax+64]
	vpdpwuud xmm1, xmm6, [0xde8]
	vpdpwuud ymm4, ymm5, [0x996]
	vpdpwuud xmm2, xmm5, [0x1bd]
	vpdpwuud ymm6, ymm0, [0x933]
	vpdpwuud zmm5, zmm4, [0xf86]
