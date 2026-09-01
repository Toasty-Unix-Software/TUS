	vsqrtps xmm4, oword [0x615]
	vsqrtps xmm4, xmm2
	vsqrtps ymm7, ymm4
	vsqrtps ymm0, ymm6
	vsqrtps xmm6, xmm5
	vsqrtps xmm2, xmm4
	vsqrtps ymm2, ymm0
	vsqrtps ymm0, ymm4

%ifdef ERROR
	vsqrtps xmm10, xmm8
	vsqrtps ymm8, ymm12
	vsqrtps xmm9, xmm10
	vsqrtps ymm15, ymm11
	vsqrtps xmm23, xmm20
	vsqrtps ymm30, ymm31
	vsqrtps xmm27, xmm18
	vsqrtps ymm23, ymm27
%endif
