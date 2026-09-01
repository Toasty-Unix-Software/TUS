default rel
	vpsignd xmm2, xmm3, xmm0
	vpsignd xmm2, xmm0
	vpsignd xmm0, xmm4, xmm3
	vpsignd ymm3, ymm0, ymm2
	vpsignd ymm3, ymm2
	vpsignd ymm2, ymm5, ymm6
	vpsignd xmm13, xmm13, xmm15
	vpsignd ymm14, ymm9, ymm15
	vpsignd xmm0, xmm1, oword [eax+1]
	vpsignd xmm4, xmm7, oword [eax+64]
	vpsignd ymm1, ymm1, yword [eax+1]
	vpsignd ymm0, ymm3, yword [eax+64]
	vpsignd xmm5, xmm3, [0x20f]
	vpsignd ymm6, ymm5, [0x97f]
