default rel
	vfmsub321ps xmm5, xmm5, oword [0xaed]
	vfmsub321ps xmm7, xmm1, xmm7
	vfmsub321ps ymm7, ymm5, ymm5
	vfmsub321ps ymm0, ymm7, ymm5
	vfmsub321ps xmm15, xmm8, xmm11
	vfmsub321ps ymm8, ymm10, ymm13
	vfmsub321ps xmm5, xmm7, oword [eax+1]
	vfmsub321ps xmm3, xmm3, oword [eax+64]
	vfmsub321ps ymm5, ymm4, yword [eax+1]
	vfmsub321ps ymm7, ymm4, yword [eax+64]
	vfmsub321ps xmm3, xmm3, [0xe06]
	vfmsub321ps ymm7, ymm7, [0xffb]
