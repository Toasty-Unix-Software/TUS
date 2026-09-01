default rel
	vpandnd xmm5, xmm3, xmm2
	vpandnd xmm5, xmm2
	vpandnd xmm7, xmm4, oword [0xeb6]
	vpandnd ymm5, ymm3, ymm6
	vpandnd ymm5, ymm6
	vpandnd ymm5, ymm2, ymm7
	vpandnd zmm4, zmm0, zword [0x7bc]
	vpandnd zmm4, zword [0x7bc]
	vpandnd zmm0, zmm1, zword [0x292]
	vpandnd xmm14, xmm11, xmm13
	vpandnd ymm14, ymm11, ymm10
	vpandnd zmm9, zmm10, zmm14
	vpandnd xmm24, xmm17, xmm31
	vpandnd ymm19, ymm28, ymm21
	vpandnd zmm17, zmm18, zmm16
	vpandnd xmm4{k3}, xmm1, xmm7
	vpandnd ymm7{k3}, ymm2, yword [0x988]
	vpandnd zmm1{k7}, zmm7, zmm1
	vpandnd xmm5{k5}{z}, xmm1, oword [0xfd6]
	vpandnd ymm3{k7}{z}, ymm3, ymm2
	vpandnd zmm3{k4}{z}, zmm0, zmm6
	vpandnd xmm3, xmm2, oword [eax+1]
	vpandnd xmm1, xmm5, oword [eax+64]
	vpandnd ymm6, ymm2, yword [eax+1]
	vpandnd ymm4, ymm4, yword [eax+64]
	vpandnd zmm7, zmm0, zword [eax+1]
	vpandnd zmm7, zmm0, zword [eax+64]
	vpandnd xmm4, xmm4, [0x123]
	vpandnd ymm3, ymm4, [0x9b7]
	vpandnd zmm5, zmm0, [0x4c4]
