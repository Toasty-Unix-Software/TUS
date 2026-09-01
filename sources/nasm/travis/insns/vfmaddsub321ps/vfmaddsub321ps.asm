default rel
	vfmaddsub321ps xmm3, xmm2, xmm6
	vfmaddsub321ps xmm2, xmm2, oword [0x4cc]
	vfmaddsub321ps ymm7, ymm2, ymm6
	vfmaddsub321ps ymm6, ymm2, ymm4
	vfmaddsub321ps xmm12, xmm13, xmm15
	vfmaddsub321ps ymm12, ymm13, ymm15
	vfmaddsub321ps xmm4, xmm3, oword [eax+1]
	vfmaddsub321ps xmm3, xmm6, oword [eax+64]
	vfmaddsub321ps ymm1, ymm3, yword [eax+1]
	vfmaddsub321ps ymm7, ymm7, yword [eax+64]
	vfmaddsub321ps xmm2, xmm2, [0x2a3]
	vfmaddsub321ps ymm1, ymm5, [0xec8]
