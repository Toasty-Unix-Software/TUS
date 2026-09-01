default rel
	vfnmaddpd xmm5, xmm7, xmm6, xmm3
	vfnmaddpd xmm5, xmm6, xmm3
	vfnmaddpd xmm2, xmm4, xmm4, xmm6
	vfnmaddpd ymm7, ymm6, yword [0x8b1], ymm7
	vfnmaddpd ymm7, yword [0x8b1], ymm7
	vfnmaddpd ymm7, ymm5, ymm2, ymm0
	vfnmaddpd xmm4, xmm0, xmm6, xmm5
	vfnmaddpd xmm4, xmm6, xmm5
	vfnmaddpd xmm6, xmm2, xmm3, oword [0xd0c]
	vfnmaddpd ymm7, ymm2, ymm5, ymm5
	vfnmaddpd ymm7, ymm5, ymm5
	vfnmaddpd ymm0, ymm2, ymm1, ymm4
	vfnmaddpd xmm11, xmm11, xmm13, xmm15
	vfnmaddpd ymm11, ymm9, ymm8, ymm9
	vfnmaddpd xmm15, xmm11, xmm15, xmm10
	vfnmaddpd ymm12, ymm8, ymm13, ymm12
	vfnmaddpd xmm7, xmm1, oword [eax+1], xmm0
	vfnmaddpd xmm3, xmm7, oword [eax+64], xmm5
	vfnmaddpd ymm4, ymm2, yword [eax+1], ymm0
	vfnmaddpd ymm6, ymm7, yword [eax+64], ymm7
	vfnmaddpd xmm1, xmm4, xmm2, oword [eax+1]
	vfnmaddpd xmm5, xmm3, xmm4, oword [eax+64]
	vfnmaddpd ymm1, ymm3, ymm0, yword [eax+1]
	vfnmaddpd ymm7, ymm3, ymm5, yword [eax+64]
	vfnmaddpd xmm1, xmm7, [0x9fd], xmm3
	vfnmaddpd ymm7, ymm0, [0x890], ymm7
	vfnmaddpd xmm3, xmm4, xmm2, [0x4a2]
	vfnmaddpd ymm1, ymm7, ymm3, [0xf83]
