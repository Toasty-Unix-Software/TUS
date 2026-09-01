default rel
	vfmsub321pd xmm3, xmm2, xmm6
	vfmsub321pd xmm5, xmm5, oword [0xf19]
	vfmsub321pd ymm6, ymm3, ymm5
	vfmsub321pd ymm3, ymm5, yword [0x207]
	vfmsub321pd xmm11, xmm13, xmm13
	vfmsub321pd ymm12, ymm9, ymm12
	vfmsub321pd xmm1, xmm1, oword [eax+1]
	vfmsub321pd xmm7, xmm0, oword [eax+64]
	vfmsub321pd ymm7, ymm4, yword [eax+1]
	vfmsub321pd ymm1, ymm4, yword [eax+64]
	vfmsub321pd xmm4, xmm6, [0x68e]
	vfmsub321pd ymm4, ymm2, [0x1fe]
