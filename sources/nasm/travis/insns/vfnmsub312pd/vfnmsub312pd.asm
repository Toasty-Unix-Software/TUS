default rel
	vfnmsub312pd xmm6, xmm6, xmm3
	vfnmsub312pd xmm3, xmm7, xmm5
	vfnmsub312pd ymm3, ymm5, ymm0
	vfnmsub312pd ymm4, ymm7, ymm5
	vfnmsub312pd xmm9, xmm11, xmm8
	vfnmsub312pd ymm15, ymm11, ymm14
	vfnmsub312pd xmm7, xmm7, oword [eax+1]
	vfnmsub312pd xmm5, xmm3, oword [eax+64]
	vfnmsub312pd ymm5, ymm2, yword [eax+1]
	vfnmsub312pd ymm5, ymm1, yword [eax+64]
	vfnmsub312pd xmm7, xmm7, [0x603]
	vfnmsub312pd ymm2, ymm4, [0xced]
