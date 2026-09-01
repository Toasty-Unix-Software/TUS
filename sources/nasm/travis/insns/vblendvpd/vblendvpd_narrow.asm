	vblendvpd xmm6, xmm2, xmm6, xmm3
	vblendvpd xmm6, xmm6, xmm3
	vblendvpd xmm2, xmm1, oword [0x682], xmm3
	vblendvpd ymm0, ymm6, ymm3, ymm1
	vblendvpd ymm0, ymm3, ymm1
	vblendvpd ymm4, ymm0, ymm0, ymm5

%ifdef ERROR
	vblendvpd xmm14, xmm8, xmm13, xmm11
	vblendvpd ymm12, ymm11, ymm10, ymm8
%endif
