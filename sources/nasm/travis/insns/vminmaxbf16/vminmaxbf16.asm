default rel
	vminmaxbf16 xmm0, xmm3, xmm2, 0xa6
	vminmaxbf16 xmm3, xmm2, xmm6, 0x6e
	vminmaxbf16 ymm1, ymm1, yword [0x4ba], 0xf0
	vminmaxbf16 ymm3, ymm0, ymm1, 0x99
	vminmaxbf16 zmm3, zmm2, zmm4, 0x55
	vminmaxbf16 zmm3, zmm1, zmm4, 0x35
	vminmaxbf16 xmm12, xmm9, xmm10, 0x8e
	vminmaxbf16 ymm9, ymm10, ymm15, 0x53
	vminmaxbf16 zmm11, zmm10, zmm9, 0x72
	vminmaxbf16 xmm24, xmm30, xmm20, 0x79
	vminmaxbf16 ymm25, ymm29, ymm28, 0x10
	vminmaxbf16 zmm29, zmm16, zmm23, 0x4a
	vminmaxbf16 xmm5{k1}, xmm4, oword [0x221], 0x29
	vminmaxbf16 ymm4{k4}, ymm5, yword [0xb7a], 0xc7
	vminmaxbf16 zmm0{k1}, zmm6, zword [0x15c], 0x80
	vminmaxbf16 xmm2{k7}{z}, xmm0, xmm2, 0xa0
	vminmaxbf16 ymm0{k7}{z}, ymm2, ymm7, 0xa6
	vminmaxbf16 zmm2{k6}{z}, zmm4, zmm6, 0x8b
	vminmaxbf16 xmm1, xmm4, oword [eax+1], 0xe5
	vminmaxbf16 xmm0, xmm7, oword [eax+64], 0xc2
	vminmaxbf16 ymm0, ymm4, yword [eax+1], 0xb0
	vminmaxbf16 ymm5, ymm2, yword [eax+64], 0xe6
	vminmaxbf16 zmm5, zmm4, zword [eax+1], 0xf6
	vminmaxbf16 zmm3, zmm4, zword [eax+64], 0x80
	vminmaxbf16 xmm0, xmm3, [0x1c2], 0x4
	vminmaxbf16 ymm3, ymm2, [0x864], 0xde
	vminmaxbf16 zmm5, zmm7, [0x868], 0x85
