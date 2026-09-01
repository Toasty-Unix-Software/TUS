	vscalefps xmm2, xmm7, xmm1
	vscalefps xmm2, xmm1
	vscalefps xmm6, xmm2, oword [0xf66]
	vscalefps ymm3, ymm4, yword [0xe65]
	vscalefps ymm3, yword [0xe65]
	vscalefps ymm6, ymm0, ymm6
	vscalefps zmm5, zmm6, zword [0x959]
	vscalefps zmm5, zword [0x959]
	vscalefps zmm6, zmm4, zmm6

%ifdef ERROR
	vscalefps xmm8, xmm12, xmm8
	vscalefps ymm11, ymm13, ymm11
	vscalefps zmm15, zmm14, zmm14
	vscalefps xmm25, xmm18, xmm20
	vscalefps ymm21, ymm29, ymm27
	vscalefps zmm27, zmm16, zmm29
%endif
