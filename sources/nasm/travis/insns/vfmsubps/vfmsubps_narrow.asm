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

%ifdef ERROR
	vfmsubps xmm14, xmm10, xmm14, xmm12
	vfmsubps ymm14, ymm8, ymm14, ymm8
	vfmsubps xmm9, xmm11, xmm12, xmm14
	vfmsubps ymm12, ymm9, ymm11, ymm15
%endif
