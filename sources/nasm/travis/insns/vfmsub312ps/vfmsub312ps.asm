default rel
	vfmsub312ps xmm7, xmm1, xmm7
	vfmsub312ps xmm4, xmm4, xmm3
	vfmsub312ps ymm6, ymm6, ymm4
	vfmsub312ps ymm4, ymm7, yword [0x36e]
	vfmsub312ps xmm8, xmm14, xmm15
	vfmsub312ps ymm11, ymm10, ymm13
	vfmsub312ps xmm0, xmm6, oword [eax+1]
	vfmsub312ps xmm3, xmm5, oword [eax+64]
	vfmsub312ps ymm5, ymm2, yword [eax+1]
	vfmsub312ps ymm5, ymm1, yword [eax+64]
	vfmsub312ps xmm6, xmm5, [0xd90]
	vfmsub312ps ymm5, ymm2, [0x6fb]
