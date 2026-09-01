default rel
	vfmaddpd xmm6, xmm0, oword [0x3e7], xmm4
	vfmaddpd xmm6, oword [0x3e7], xmm4
	vfmaddpd xmm1, xmm1, xmm6, xmm3
	vfmaddpd ymm3, ymm7, yword [0x543], ymm4
	vfmaddpd ymm3, yword [0x543], ymm4
	vfmaddpd ymm6, ymm7, ymm5, ymm2
	vfmaddpd xmm7, xmm7, xmm6, oword [0x3b5]
	vfmaddpd xmm7, xmm6, oword [0x3b5]
	vfmaddpd xmm6, xmm7, xmm7, xmm7
	vfmaddpd ymm2, ymm1, ymm1, ymm6
	vfmaddpd ymm2, ymm1, ymm6
	vfmaddpd ymm0, ymm7, ymm5, ymm5
	vfmaddpd xmm12, xmm15, xmm14, xmm12
	vfmaddpd ymm9, ymm14, ymm8, ymm12
	vfmaddpd xmm14, xmm13, xmm8, xmm9
	vfmaddpd ymm13, ymm10, ymm14, ymm12
	vfmaddpd xmm1, xmm1, oword [eax+1], xmm6
	vfmaddpd xmm5, xmm2, oword [eax+64], xmm6
	vfmaddpd ymm0, ymm3, yword [eax+1], ymm1
	vfmaddpd ymm3, ymm6, yword [eax+64], ymm4
	vfmaddpd xmm2, xmm3, xmm4, oword [eax+1]
	vfmaddpd xmm5, xmm0, xmm5, oword [eax+64]
	vfmaddpd ymm5, ymm6, ymm5, yword [eax+1]
	vfmaddpd ymm2, ymm0, ymm0, yword [eax+64]
	vfmaddpd xmm0, xmm2, [0x9df], xmm3
	vfmaddpd ymm7, ymm1, [0xae2], ymm7
	vfmaddpd xmm2, xmm5, xmm7, [0x6fd]
	vfmaddpd ymm4, ymm3, ymm5, [0x227]
