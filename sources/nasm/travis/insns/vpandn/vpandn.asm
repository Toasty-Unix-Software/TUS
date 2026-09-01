default rel
	vpandn xmm6, xmm1, xmm6
	vpandn xmm6, xmm6
	vpandn xmm2, xmm0, oword [0xc6c]
	vpandn ymm6, ymm1, ymm6
	vpandn ymm6, ymm6
	vpandn ymm2, ymm3, yword [0xa9c]
	vpandn xmm9, xmm9, xmm14
	vpandn ymm10, ymm13, ymm13
	vpandn xmm6, xmm1, oword [eax+1]
	vpandn xmm4, xmm0, oword [eax+64]
	vpandn ymm4, ymm4, yword [eax+1]
	vpandn ymm7, ymm0, yword [eax+64]
	vpandn xmm5, xmm0, [0x679]
	vpandn ymm2, ymm1, [0xd0c]
