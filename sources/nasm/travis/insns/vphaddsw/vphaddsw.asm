default rel
	vphaddsw xmm7, xmm2, xmm4
	vphaddsw xmm7, xmm4
	vphaddsw xmm7, xmm3, xmm6
	vphaddsw ymm5, ymm0, yword [0xb75]
	vphaddsw ymm5, yword [0xb75]
	vphaddsw ymm7, ymm5, ymm4
	vphaddsw xmm13, xmm12, xmm10
	vphaddsw ymm14, ymm13, ymm15
	vphaddsw xmm7, xmm0, oword [eax+1]
	vphaddsw xmm5, xmm1, oword [eax+64]
	vphaddsw ymm5, ymm3, yword [eax+1]
	vphaddsw ymm3, ymm0, yword [eax+64]
	vphaddsw xmm5, xmm7, [0xaf8]
	vphaddsw ymm6, ymm0, [0x5b7]
