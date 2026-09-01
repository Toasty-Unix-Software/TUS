default rel
	vphsubw xmm1, xmm4, xmm7
	vphsubw xmm1, xmm7
	vphsubw xmm2, xmm2, xmm2
	vphsubw ymm3, ymm0, yword [0x81a]
	vphsubw ymm3, yword [0x81a]
	vphsubw ymm5, ymm6, yword [0xb4b]
	vphsubw xmm8, xmm15, xmm14
	vphsubw ymm11, ymm12, ymm8
	vphsubw xmm2, xmm2, oword [eax+1]
	vphsubw xmm7, xmm7, oword [eax+64]
	vphsubw ymm4, ymm3, yword [eax+1]
	vphsubw ymm0, ymm0, yword [eax+64]
	vphsubw xmm0, xmm4, [0xc71]
	vphsubw ymm0, ymm6, [0xf01]
