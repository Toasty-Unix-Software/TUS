	vinserti64x2 ymm5, ymm2, xmm0, 0xe9
	vinserti64x2 ymm5, xmm0, 0xe9
	vinserti64x2 ymm7, ymm4, xmm1, 0xdc
	vinserti64x2 zmm7, zmm5, xmm6, 0x9b
	vinserti64x2 zmm7, xmm6, 0x9b
	vinserti64x2 zmm0, zmm6, xmm6, 0x60

%ifdef ERROR
	vinserti64x2 ymm15, ymm13, xmm12, 0x69
	vinserti64x2 zmm15, zmm8, xmm8, 0xd6
	vinserti64x2 ymm23, ymm25, xmm16, 0xd4
	vinserti64x2 zmm21, zmm19, xmm20, 0x93
%endif
