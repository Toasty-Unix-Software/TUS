	vinsertf64x2 ymm5, ymm2, oword [0x877], 0x25
	vinsertf64x2 ymm5, oword [0x877], 0x25
	vinsertf64x2 ymm6, ymm1, oword [0x613], 0x4e
	vinsertf64x2 zmm2, zmm4, xmm3, 0x9e
	vinsertf64x2 zmm2, xmm3, 0x9e
	vinsertf64x2 zmm2, zmm4, xmm2, 0xf6

%ifdef ERROR
	vinsertf64x2 ymm10, ymm13, xmm10, 0x50
	vinsertf64x2 zmm9, zmm10, xmm12, 0xe0
	vinsertf64x2 ymm26, ymm29, xmm25, 0xb7
	vinsertf64x2 zmm28, zmm21, xmm27, 0x80
%endif
