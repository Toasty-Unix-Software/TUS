default rel
	vphsubsw xmm6, xmm1, oword [0x452]
	vphsubsw xmm6, oword [0x452]
	vphsubsw xmm6, xmm0, xmm5
	vphsubsw ymm3, ymm1, ymm6
	vphsubsw ymm3, ymm6
	vphsubsw ymm2, ymm2, ymm6
	vphsubsw xmm10, xmm12, xmm14
	vphsubsw ymm8, ymm15, ymm10
	vphsubsw xmm7, xmm2, oword [eax+1]
	vphsubsw xmm1, xmm7, oword [eax+64]
	vphsubsw ymm1, ymm6, yword [eax+1]
	vphsubsw ymm0, ymm0, yword [eax+64]
	vphsubsw xmm7, xmm4, [0xe74]
	vphsubsw ymm5, ymm6, [0x309]
