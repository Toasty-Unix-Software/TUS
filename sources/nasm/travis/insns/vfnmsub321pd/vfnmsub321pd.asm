default rel
	vfnmsub321pd xmm7, xmm5, oword [0x55e]
	vfnmsub321pd xmm2, xmm4, xmm2
	vfnmsub321pd ymm0, ymm3, ymm0
	vfnmsub321pd ymm5, ymm4, ymm5
	vfnmsub321pd xmm12, xmm12, xmm9
	vfnmsub321pd ymm8, ymm13, ymm10
	vfnmsub321pd xmm6, xmm7, oword [eax+1]
	vfnmsub321pd xmm1, xmm4, oword [eax+64]
	vfnmsub321pd ymm5, ymm1, yword [eax+1]
	vfnmsub321pd ymm1, ymm0, yword [eax+64]
	vfnmsub321pd xmm2, xmm4, [0x4f6]
	vfnmsub321pd ymm4, ymm2, [0xc39]
