default rel
	vphsubd xmm6, xmm3, xmm6
	vphsubd xmm6, xmm6
	vphsubd xmm1, xmm6, xmm6
	vphsubd ymm5, ymm6, yword [0xf15]
	vphsubd ymm5, yword [0xf15]
	vphsubd ymm3, ymm2, ymm6
	vphsubd xmm12, xmm13, xmm12
	vphsubd ymm8, ymm12, ymm12
	vphsubd xmm0, xmm5, oword [eax+1]
	vphsubd xmm6, xmm5, oword [eax+64]
	vphsubd ymm2, ymm6, yword [eax+1]
	vphsubd ymm4, ymm4, yword [eax+64]
	vphsubd xmm2, xmm6, [0x3e6]
	vphsubd ymm5, ymm4, [0xdd7]
