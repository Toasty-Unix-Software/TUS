	vaddps xmm3, xmm1, xmm4
	vaddps xmm3, xmm4
	vaddps xmm3, xmm4, oword [0x9bd]
	vaddps ymm5, ymm5, ymm3
	vaddps ymm5, ymm3
	vaddps ymm7, ymm7, ymm6
	vaddps xmm6, xmm0, oword [0xa06]
	vaddps xmm6, oword [0xa06]
	vaddps xmm0, xmm1, xmm6
	vaddps ymm3, ymm1, ymm4
	vaddps ymm3, ymm4
	vaddps ymm0, ymm0, yword [0x704]

%ifdef ERROR
	vaddps xmm14, xmm8, xmm10
	vaddps ymm15, ymm9, ymm8
	vaddps xmm15, xmm14, xmm12
	vaddps ymm15, ymm13, ymm14
	vaddps xmm18, xmm20, xmm17
	vaddps ymm23, ymm29, ymm31
	vaddps xmm20, xmm26, xmm23
	vaddps ymm27, ymm20, ymm27
%endif
