	vxorps xmm7, xmm1, xmm6
	vxorps xmm7, xmm6
	vxorps xmm6, xmm7, xmm6
	vxorps ymm7, ymm0, ymm0
	vxorps ymm7, ymm0
	vxorps ymm6, ymm0, ymm6
	vxorps xmm4, xmm7, xmm0
	vxorps xmm4, xmm0
	vxorps xmm5, xmm3, xmm5
	vxorps ymm0, ymm4, ymm5
	vxorps ymm0, ymm5
	vxorps ymm5, ymm7, ymm0

%ifdef ERROR
	vxorps xmm10, xmm8, xmm12
	vxorps ymm14, ymm12, ymm11
	vxorps xmm9, xmm14, xmm12
	vxorps ymm13, ymm11, ymm15
	vxorps xmm24, xmm17, xmm31
	vxorps ymm23, ymm27, ymm26
	vxorps xmm25, xmm26, xmm16
	vxorps ymm26, ymm28, ymm21
%endif
