default rel
	vfmaddsub312pd xmm5, xmm4, xmm3
	vfmaddsub312pd xmm3, xmm4, xmm6
	vfmaddsub312pd ymm1, ymm0, ymm5
	vfmaddsub312pd ymm2, ymm5, ymm1
	vfmaddsub312pd xmm10, xmm9, xmm14
	vfmaddsub312pd ymm14, ymm11, ymm15
	vfmaddsub312pd xmm5, xmm2, oword [eax+1]
	vfmaddsub312pd xmm2, xmm7, oword [eax+64]
	vfmaddsub312pd ymm3, ymm2, yword [eax+1]
	vfmaddsub312pd ymm3, ymm5, yword [eax+64]
	vfmaddsub312pd xmm3, xmm5, [0x444]
	vfmaddsub312pd ymm5, ymm4, [0x177]
