default rel
	vpand xmm5, xmm1, oword [0xe0b]
	vpand xmm5, oword [0xe0b]
	vpand xmm1, xmm0, oword [0xf38]
	vpand ymm5, ymm3, ymm4
	vpand ymm5, ymm4
	vpand ymm6, ymm2, ymm3
	vpand xmm10, xmm13, xmm12
	vpand ymm10, ymm13, ymm9
	vpand xmm3, xmm6, oword [eax+1]
	vpand xmm0, xmm0, oword [eax+64]
	vpand ymm0, ymm5, yword [eax+1]
	vpand ymm7, ymm5, yword [eax+64]
	vpand xmm4, xmm6, [0x93f]
	vpand ymm5, ymm3, [0x897]
