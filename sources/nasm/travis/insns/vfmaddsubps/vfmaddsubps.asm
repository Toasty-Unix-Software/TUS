default rel
	vfmaddsubps xmm0, xmm0, xmm5, xmm5
	vfmaddsubps xmm0, xmm5, xmm5
	vfmaddsubps xmm1, xmm0, xmm0, xmm4
	vfmaddsubps ymm1, ymm6, yword [0xc75], ymm7
	vfmaddsubps ymm1, yword [0xc75], ymm7
	vfmaddsubps ymm3, ymm6, ymm2, ymm4
	vfmaddsubps xmm7, xmm3, xmm3, oword [0xaf3]
	vfmaddsubps xmm7, xmm3, oword [0xaf3]
	vfmaddsubps xmm7, xmm6, xmm5, xmm5
	vfmaddsubps ymm4, ymm6, ymm6, ymm7
	vfmaddsubps ymm4, ymm6, ymm7
	vfmaddsubps ymm6, ymm7, ymm4, ymm4
	vfmaddsubps xmm10, xmm9, xmm12, xmm12
	vfmaddsubps ymm15, ymm8, ymm8, ymm8
	vfmaddsubps xmm12, xmm9, xmm13, xmm14
	vfmaddsubps ymm11, ymm14, ymm14, ymm11
	vfmaddsubps xmm0, xmm7, oword [eax+1], xmm2
	vfmaddsubps xmm2, xmm3, oword [eax+64], xmm5
	vfmaddsubps ymm5, ymm7, yword [eax+1], ymm0
	vfmaddsubps ymm3, ymm7, yword [eax+64], ymm4
	vfmaddsubps xmm5, xmm7, xmm4, oword [eax+1]
	vfmaddsubps xmm1, xmm5, xmm7, oword [eax+64]
	vfmaddsubps ymm5, ymm6, ymm7, yword [eax+1]
	vfmaddsubps ymm7, ymm0, ymm1, yword [eax+64]
	vfmaddsubps xmm7, xmm5, [0x865], xmm0
	vfmaddsubps ymm4, ymm4, [0xff5], ymm1
	vfmaddsubps xmm7, xmm0, xmm3, [0x177]
	vfmaddsubps ymm4, ymm4, ymm1, [0xf8a]
