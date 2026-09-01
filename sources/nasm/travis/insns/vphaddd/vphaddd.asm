default rel
	vphaddd xmm7, xmm2, xmm6
	vphaddd xmm7, xmm6
	vphaddd xmm5, xmm2, oword [0xe3a]
	vphaddd ymm1, ymm0, ymm6
	vphaddd ymm1, ymm6
	vphaddd ymm4, ymm7, yword [0x6f1]
	vphaddd xmm11, xmm14, xmm14
	vphaddd ymm8, ymm12, ymm10
	vphaddd xmm5, xmm7, oword [eax+1]
	vphaddd xmm4, xmm1, oword [eax+64]
	vphaddd ymm2, ymm1, yword [eax+1]
	vphaddd ymm4, ymm1, yword [eax+64]
	vphaddd xmm2, xmm0, [0x5e9]
	vphaddd ymm2, ymm4, [0xad4]
