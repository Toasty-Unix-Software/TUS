default rel
	vfmaddsub123ps xmm1, xmm0, xmm4
	vfmaddsub123ps xmm3, xmm7, xmm6
	vfmaddsub123ps ymm4, ymm7, ymm1
	vfmaddsub123ps ymm7, ymm5, yword [0xee0]
	vfmaddsub123ps xmm14, xmm15, xmm13
	vfmaddsub123ps ymm13, ymm10, ymm11
	vfmaddsub123ps xmm4, xmm4, oword [eax+1]
	vfmaddsub123ps xmm3, xmm4, oword [eax+64]
	vfmaddsub123ps ymm5, ymm2, yword [eax+1]
	vfmaddsub123ps ymm4, ymm6, yword [eax+64]
	vfmaddsub123ps xmm5, xmm1, [0x8b3]
	vfmaddsub123ps ymm5, ymm6, [0x1b9]
