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

%ifdef ERROR
	vfmaddsubps xmm10, xmm9, xmm12, xmm12
	vfmaddsubps ymm15, ymm8, ymm8, ymm8
	vfmaddsubps xmm12, xmm9, xmm13, xmm14
	vfmaddsubps ymm11, ymm14, ymm14, ymm11
%endif
