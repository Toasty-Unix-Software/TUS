default rel
	vinsertf64x2 ymm5, ymm2, oword [0x877], 0x25
	vinsertf64x2 ymm5, oword [0x877], 0x25
	vinsertf64x2 ymm6, ymm1, oword [0x613], 0x4e
	vinsertf64x2 zmm2, zmm4, xmm3, 0x9e
	vinsertf64x2 zmm2, xmm3, 0x9e
	vinsertf64x2 zmm2, zmm4, xmm2, 0xf6
	vinsertf64x2 ymm10, ymm13, xmm10, 0x50
	vinsertf64x2 zmm9, zmm10, xmm12, 0xe0
	vinsertf64x2 ymm26, ymm29, xmm25, 0xb7
	vinsertf64x2 zmm28, zmm21, xmm27, 0x80
	vinsertf64x2 ymm7{k7}, ymm2, xmm6, 0x3c
	vinsertf64x2 zmm6{k5}, zmm5, xmm1, 0x38
	vinsertf64x2 ymm1{k1}{z}, ymm0, xmm6, 0x64
	vinsertf64x2 zmm4{k2}{z}, zmm5, xmm2, 0x4f
	vinsertf64x2 ymm6, ymm7, oword [eax+1], 0x41
	vinsertf64x2 ymm3, ymm2, oword [eax+64], 0xd8
	vinsertf64x2 zmm4, zmm1, oword [eax+1], 0xa
	vinsertf64x2 zmm0, zmm6, oword [eax+64], 0xd0
	vinsertf64x2 ymm6, ymm2, [0x412], 0xec
	vinsertf64x2 zmm0, zmm6, [0x76e], 0x3d
