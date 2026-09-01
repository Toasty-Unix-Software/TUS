	vminmaxps xmm4, xmm7, xmm4, 0x40
	vminmaxps xmm2, xmm6, oword [0x916], 0x5f
	vminmaxps ymm4, ymm3, ymm6, 0xee
	vminmaxps ymm4, ymm6, ymm5, 0x8b
	vminmaxps zmm1, zmm4, zmm5, 0x2a
	vminmaxps zmm0, zmm4, zmm2, 0x63

%ifdef ERROR
	vminmaxps xmm13, xmm15, xmm10, 0x85
	vminmaxps ymm9, ymm10, ymm14, 0x3a
	vminmaxps zmm8, zmm11, zmm11, 0xdb
	vminmaxps xmm25, xmm18, xmm25, 0x67
	vminmaxps ymm29, ymm22, ymm26, 0xa7
	vminmaxps zmm23, zmm23, zmm30, 0x52
%endif
