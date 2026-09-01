default rel
	vfmsubps xmm6, xmm3, xmm4, xmm4
	vfmsubps xmm6, xmm4, xmm4
	vfmsubps xmm2, xmm0, xmm6, xmm7
	vfmsubps ymm0, ymm5, yword [0x60f], ymm1
	vfmsubps ymm0, yword [0x60f], ymm1
	vfmsubps ymm1, ymm3, ymm3, ymm3
	vfmsubps xmm0, xmm4, xmm3, xmm6
	vfmsubps xmm0, xmm3, xmm6
	vfmsubps xmm4, xmm3, xmm2, xmm3
	vfmsubps ymm5, ymm7, ymm3, ymm4
	vfmsubps ymm5, ymm3, ymm4
	vfmsubps ymm5, ymm0, ymm1, ymm1
	vfmsubps xmm14, xmm10, xmm14, xmm12
	vfmsubps ymm14, ymm8, ymm14, ymm8
	vfmsubps xmm9, xmm11, xmm12, xmm14
	vfmsubps ymm12, ymm9, ymm11, ymm15
	vfmsubps xmm4, xmm1, oword [eax+1], xmm3
	vfmsubps xmm1, xmm0, oword [eax+64], xmm1
	vfmsubps ymm2, ymm2, yword [eax+1], ymm3
	vfmsubps ymm3, ymm2, yword [eax+64], ymm7
	vfmsubps xmm0, xmm2, xmm6, oword [eax+1]
	vfmsubps xmm7, xmm5, xmm4, oword [eax+64]
	vfmsubps ymm3, ymm2, ymm2, yword [eax+1]
	vfmsubps ymm3, ymm3, ymm3, yword [eax+64]
	vfmsubps xmm6, xmm6, [0x3c3], xmm3
	vfmsubps ymm6, ymm1, [0x946], ymm0
	vfmsubps xmm0, xmm0, xmm5, [0x524]
	vfmsubps ymm2, ymm1, ymm5, [0x38c]
