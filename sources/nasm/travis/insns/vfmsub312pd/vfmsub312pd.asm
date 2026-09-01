default rel
	vfmsub312pd xmm7, xmm7, xmm0
	vfmsub312pd xmm1, xmm2, xmm7
	vfmsub312pd ymm6, ymm6, yword [0x491]
	vfmsub312pd ymm0, ymm7, yword [0x8bd]
	vfmsub312pd xmm10, xmm15, xmm8
	vfmsub312pd ymm8, ymm15, ymm11
	vfmsub312pd xmm1, xmm5, oword [eax+1]
	vfmsub312pd xmm5, xmm5, oword [eax+64]
	vfmsub312pd ymm1, ymm5, yword [eax+1]
	vfmsub312pd ymm0, ymm0, yword [eax+64]
	vfmsub312pd xmm0, xmm5, [0xfcf]
	vfmsub312pd ymm4, ymm2, [0x8eb]
