default rel
	vaesenc xmm6, xmm4, xmm2
	vaesenc xmm6, xmm2
	vaesenc xmm3, xmm7, xmm1
	vaesenc ymm1, ymm4, ymm0
	vaesenc ymm1, ymm0
	vaesenc ymm5, ymm0, ymm7
	vaesenc zmm0, zmm4, zmm2
	vaesenc zmm0, zmm2
	vaesenc zmm6, zmm1, zmm0
	vaesenc xmm12, xmm11, xmm15
	vaesenc ymm9, ymm8, ymm10
	vaesenc zmm14, zmm15, zmm11
	vaesenc xmm24, xmm17, xmm16
	vaesenc ymm19, ymm19, ymm27
	vaesenc zmm27, zmm31, zmm22
	vaesenc xmm3, xmm7, oword [eax+1]
	vaesenc xmm1, xmm7, oword [eax+64]
	vaesenc ymm1, ymm2, yword [eax+1]
	vaesenc ymm5, ymm2, yword [eax+64]
	vaesenc zmm2, zmm7, zword [eax+1]
	vaesenc zmm1, zmm0, zword [eax+64]
	vaesenc xmm3, xmm4, [0xd49]
	vaesenc ymm1, ymm6, [0xf54]
	vaesenc zmm1, zmm6, [0xc27]
