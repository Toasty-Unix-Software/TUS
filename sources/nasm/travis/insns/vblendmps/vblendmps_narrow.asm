	vblendmps xmm6, xmm2, xmm3
	vblendmps xmm3, xmm7, xmm1
	vblendmps ymm2, ymm6, ymm2
	vblendmps ymm3, ymm3, ymm0
	vblendmps zmm1, zmm0, zmm4
	vblendmps zmm1, zmm6, zmm3

%ifdef ERROR
	vblendmps xmm8, xmm12, xmm14
	vblendmps ymm11, ymm12, ymm12
	vblendmps zmm15, zmm8, zmm13
	vblendmps xmm21, xmm25, xmm26
	vblendmps ymm31, ymm25, ymm22
	vblendmps zmm22, zmm27, zmm24
%endif
