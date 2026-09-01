default rel
	vptest xmm6, xmm1
	vptest xmm5, xmm0
	vptest ymm7, ymm5
	vptest ymm5, ymm4
	vptest xmm13, xmm15
	vptest ymm11, ymm10
	vptest xmm2, oword [eax+1]
	vptest xmm6, oword [eax+64]
	vptest ymm1, yword [eax+1]
	vptest ymm4, yword [eax+64]
	vptest xmm2, [0xa9d]
	vptest ymm3, [0x9b5]
