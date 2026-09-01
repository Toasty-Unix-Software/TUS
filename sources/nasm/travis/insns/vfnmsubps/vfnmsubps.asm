default rel
	vfnmsubps xmm0, xmm1, xmm2, xmm7
	vfnmsubps xmm0, xmm2, xmm7
	vfnmsubps xmm4, xmm0, xmm0, xmm2
	vfnmsubps ymm5, ymm5, yword [0x47e], ymm2
	vfnmsubps ymm5, yword [0x47e], ymm2
	vfnmsubps ymm1, ymm2, ymm3, ymm4
	vfnmsubps xmm5, xmm1, xmm2, xmm2
	vfnmsubps xmm5, xmm2, xmm2
	vfnmsubps xmm2, xmm1, xmm0, xmm6
	vfnmsubps ymm5, ymm4, ymm3, ymm5
	vfnmsubps ymm5, ymm3, ymm5
	vfnmsubps ymm0, ymm5, ymm6, yword [0xf21]
	vfnmsubps xmm15, xmm15, xmm15, xmm14
	vfnmsubps ymm8, ymm12, ymm11, ymm10
	vfnmsubps xmm8, xmm11, xmm8, xmm8
	vfnmsubps ymm15, ymm9, ymm11, ymm10
	vfnmsubps xmm5, xmm0, oword [eax+1], xmm0
	vfnmsubps xmm5, xmm4, oword [eax+64], xmm3
	vfnmsubps ymm0, ymm7, yword [eax+1], ymm6
	vfnmsubps ymm0, ymm0, yword [eax+64], ymm6
	vfnmsubps xmm3, xmm6, xmm3, oword [eax+1]
	vfnmsubps xmm0, xmm1, xmm3, oword [eax+64]
	vfnmsubps ymm5, ymm4, ymm0, yword [eax+1]
	vfnmsubps ymm6, ymm0, ymm6, yword [eax+64]
	vfnmsubps xmm3, xmm4, [0xe43], xmm2
	vfnmsubps ymm2, ymm0, [0x1cf], ymm6
	vfnmsubps xmm2, xmm1, xmm6, [0x7fa]
	vfnmsubps ymm4, ymm4, ymm1, [0xccb]
