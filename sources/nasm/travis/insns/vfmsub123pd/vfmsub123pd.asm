default rel
	vfmsub123pd xmm6, xmm6, xmm1
	vfmsub123pd xmm5, xmm4, oword [0x922]
	vfmsub123pd ymm6, ymm5, ymm2
	vfmsub123pd ymm5, ymm7, yword [0xb2a]
	vfmsub123pd xmm11, xmm15, xmm8
	vfmsub123pd ymm8, ymm10, ymm8
	vfmsub123pd xmm5, xmm7, oword [eax+1]
	vfmsub123pd xmm1, xmm7, oword [eax+64]
	vfmsub123pd ymm1, ymm6, yword [eax+1]
	vfmsub123pd ymm0, ymm3, yword [eax+64]
	vfmsub123pd xmm6, xmm1, [0x675]
	vfmsub123pd ymm3, ymm6, [0xb0f]
