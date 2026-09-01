default rel
	vfmsub123ps xmm2, xmm5, oword [0x3c8]
	vfmsub123ps xmm1, xmm3, xmm1
	vfmsub123ps ymm3, ymm3, ymm2
	vfmsub123ps ymm4, ymm4, ymm5
	vfmsub123ps xmm12, xmm11, xmm10
	vfmsub123ps ymm13, ymm15, ymm12
	vfmsub123ps xmm7, xmm0, oword [eax+1]
	vfmsub123ps xmm7, xmm6, oword [eax+64]
	vfmsub123ps ymm7, ymm4, yword [eax+1]
	vfmsub123ps ymm1, ymm0, yword [eax+64]
	vfmsub123ps xmm2, xmm2, [0xc81]
	vfmsub123ps ymm1, ymm2, [0x41b]
