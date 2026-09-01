default rel
	vfmaddsub312ps xmm7, xmm7, xmm1
	vfmaddsub312ps xmm6, xmm7, xmm5
	vfmaddsub312ps ymm4, ymm5, yword [0xcba]
	vfmaddsub312ps ymm3, ymm5, ymm2
	vfmaddsub312ps xmm13, xmm14, xmm13
	vfmaddsub312ps ymm14, ymm10, ymm9
	vfmaddsub312ps xmm1, xmm5, oword [eax+1]
	vfmaddsub312ps xmm1, xmm6, oword [eax+64]
	vfmaddsub312ps ymm1, ymm5, yword [eax+1]
	vfmaddsub312ps ymm1, ymm5, yword [eax+64]
	vfmaddsub312ps xmm3, xmm4, [0x8ec]
	vfmaddsub312ps ymm3, ymm4, [0xe78]
