	vpshldw xmm7, xmm4, oword [0xe49], 0x5d
	vpshldw xmm7, oword [0xe49], 0x5d
	vpshldw xmm1, xmm1, xmm1, 0xfd
	vpshldw ymm4, ymm2, ymm7, 0xaf
	vpshldw ymm4, ymm7, 0xaf
	vpshldw ymm2, ymm3, ymm4, 0x5f
	vpshldw zmm0, zmm1, zmm4, 0x1
	vpshldw zmm0, zmm4, 0x1
	vpshldw zmm1, zmm1, zword [0x7d1], 0x20

%ifdef ERROR
	vpshldw xmm10, xmm9, xmm8, 0xa2
	vpshldw ymm15, ymm9, ymm13, 0x64
	vpshldw zmm12, zmm11, zmm8, 0xa0
	vpshldw xmm24, xmm24, xmm16, 0x64
	vpshldw ymm30, ymm29, ymm29, 0xb4
	vpshldw zmm25, zmm30, zmm16, 0x53
%endif
