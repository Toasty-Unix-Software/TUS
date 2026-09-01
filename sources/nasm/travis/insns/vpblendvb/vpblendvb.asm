default rel
	vpblendvb xmm7, xmm5, xmm4, xmm6
	vpblendvb xmm7, xmm4, xmm6
	vpblendvb xmm1, xmm2, oword [0xa4c], xmm2
	vpblendvb ymm2, ymm6, ymm0, ymm4
	vpblendvb ymm2, ymm0, ymm4
	vpblendvb ymm3, ymm0, ymm4, ymm3
	vpblendvb xmm12, xmm15, xmm14, xmm8
	vpblendvb ymm8, ymm14, ymm8, ymm11
	vpblendvb xmm5, xmm3, oword [eax+1], xmm2
	vpblendvb xmm3, xmm2, oword [eax+64], xmm1
	vpblendvb ymm4, ymm1, yword [eax+1], ymm2
	vpblendvb ymm0, ymm1, yword [eax+64], ymm5
	vpblendvb xmm4, xmm5, [0x5ac], xmm1
	vpblendvb ymm5, ymm7, [0x85d], ymm7
