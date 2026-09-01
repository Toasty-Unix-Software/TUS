default rel
	vfnmsub321ps xmm0, xmm5, xmm6
	vfnmsub321ps xmm7, xmm0, xmm5
	vfnmsub321ps ymm7, ymm4, ymm3
	vfnmsub321ps ymm2, ymm0, ymm7
	vfnmsub321ps xmm11, xmm11, xmm9
	vfnmsub321ps ymm12, ymm11, ymm13
	vfnmsub321ps xmm7, xmm4, oword [eax+1]
	vfnmsub321ps xmm5, xmm0, oword [eax+64]
	vfnmsub321ps ymm2, ymm1, yword [eax+1]
	vfnmsub321ps ymm4, ymm5, yword [eax+64]
	vfnmsub321ps xmm7, xmm3, [0x433]
	vfnmsub321ps ymm0, ymm3, [0x40c]
