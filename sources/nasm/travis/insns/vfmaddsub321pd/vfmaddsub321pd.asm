default rel
	vfmaddsub321pd xmm4, xmm1, xmm4
	vfmaddsub321pd xmm6, xmm2, xmm3
	vfmaddsub321pd ymm3, ymm4, ymm5
	vfmaddsub321pd ymm0, ymm5, ymm4
	vfmaddsub321pd xmm8, xmm11, xmm14
	vfmaddsub321pd ymm8, ymm15, ymm9
	vfmaddsub321pd xmm7, xmm6, oword [eax+1]
	vfmaddsub321pd xmm3, xmm5, oword [eax+64]
	vfmaddsub321pd ymm7, ymm1, yword [eax+1]
	vfmaddsub321pd ymm0, ymm6, yword [eax+64]
	vfmaddsub321pd xmm4, xmm5, [0x86e]
	vfmaddsub321pd ymm5, ymm2, [0x661]
