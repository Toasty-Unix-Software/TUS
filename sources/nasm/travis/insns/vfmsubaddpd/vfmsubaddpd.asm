default rel
	vfmsubaddpd xmm4, xmm0, xmm2, xmm5
	vfmsubaddpd xmm4, xmm2, xmm5
	vfmsubaddpd xmm7, xmm4, xmm6, xmm0
	vfmsubaddpd ymm4, ymm1, ymm3, ymm4
	vfmsubaddpd ymm4, ymm3, ymm4
	vfmsubaddpd ymm0, ymm3, ymm1, ymm5
	vfmsubaddpd xmm3, xmm7, xmm7, oword [0x924]
	vfmsubaddpd xmm3, xmm7, oword [0x924]
	vfmsubaddpd xmm4, xmm4, xmm1, xmm6
	vfmsubaddpd ymm4, ymm7, ymm1, yword [0xcc3]
	vfmsubaddpd ymm4, ymm1, yword [0xcc3]
	vfmsubaddpd ymm0, ymm6, ymm5, ymm7
	vfmsubaddpd xmm10, xmm14, xmm11, xmm15
	vfmsubaddpd ymm11, ymm12, ymm15, ymm11
	vfmsubaddpd xmm10, xmm11, xmm8, xmm14
	vfmsubaddpd ymm14, ymm11, ymm14, ymm10
	vfmsubaddpd xmm6, xmm5, oword [eax+1], xmm3
	vfmsubaddpd xmm1, xmm6, oword [eax+64], xmm4
	vfmsubaddpd ymm3, ymm7, yword [eax+1], ymm1
	vfmsubaddpd ymm6, ymm1, yword [eax+64], ymm5
	vfmsubaddpd xmm5, xmm4, xmm2, oword [eax+1]
	vfmsubaddpd xmm1, xmm2, xmm0, oword [eax+64]
	vfmsubaddpd ymm7, ymm0, ymm4, yword [eax+1]
	vfmsubaddpd ymm4, ymm4, ymm2, yword [eax+64]
	vfmsubaddpd xmm4, xmm4, [0x54a], xmm4
	vfmsubaddpd ymm0, ymm1, [0x37a], ymm7
	vfmsubaddpd xmm7, xmm3, xmm3, [0xf22]
	vfmsubaddpd ymm6, ymm3, ymm3, [0xf7a]
