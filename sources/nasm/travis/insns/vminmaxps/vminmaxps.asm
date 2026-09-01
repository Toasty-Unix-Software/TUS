default rel
	vminmaxps xmm4, xmm7, xmm4, 0x40
	vminmaxps xmm2, xmm6, oword [0x916], 0x5f
	vminmaxps ymm4, ymm3, ymm6, 0xee
	vminmaxps ymm4, ymm6, ymm5, 0x8b
	vminmaxps zmm1, zmm4, zmm5, 0x2a
	vminmaxps zmm0, zmm4, zmm2, 0x63
	vminmaxps xmm13, xmm15, xmm10, 0x85
	vminmaxps ymm9, ymm10, ymm14, 0x3a
	vminmaxps zmm8, zmm11, zmm11, 0xdb
	vminmaxps xmm25, xmm18, xmm25, 0x67
	vminmaxps ymm29, ymm22, ymm26, 0xa7
	vminmaxps zmm23, zmm23, zmm30, 0x52
	vminmaxps xmm1{k5}, xmm1, xmm0, 0xbe
	vminmaxps ymm6{k1}, ymm6, ymm4, 0x40
	vminmaxps zmm6{k6}, zmm2, zmm3, 0x15
	vminmaxps xmm7{k3}{z}, xmm1, xmm7, 0x21
	vminmaxps ymm5{k4}{z}, ymm0, ymm3, 0xb6
	vminmaxps zmm4{k4}{z}, zmm5, zword [0x4e6], 0xcd
	vminmaxps xmm6, xmm2, oword [eax+1], 0xc0
	vminmaxps xmm1, xmm7, oword [eax+64], 0xe9
	vminmaxps ymm5, ymm6, yword [eax+1], 0x8f
	vminmaxps ymm6, ymm3, yword [eax+64], 0xe0
	vminmaxps zmm0, zmm6, zword [eax+1], 0xee
	vminmaxps zmm1, zmm6, zword [eax+64], 0xe7
	vminmaxps xmm0, xmm6, [0xfda], 0xd8
	vminmaxps ymm7, ymm4, [0x3b4], 0x59
	vminmaxps zmm1, zmm1, [0xe14], 0x9b
