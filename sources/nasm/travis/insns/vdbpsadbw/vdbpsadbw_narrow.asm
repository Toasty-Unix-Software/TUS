	vdbpsadbw xmm1, xmm3, xmm6, 0xe8
	vdbpsadbw xmm1, xmm6, 0xe8
	vdbpsadbw xmm3, xmm7, xmm4, 0x50
	vdbpsadbw ymm6, ymm6, yword [0x1d4], 0x19
	vdbpsadbw ymm6, yword [0x1d4], 0x19
	vdbpsadbw ymm6, ymm7, ymm3, 0xfc
	vdbpsadbw zmm2, zmm7, zmm0, 0xb0
	vdbpsadbw zmm2, zmm0, 0xb0
	vdbpsadbw zmm6, zmm1, zmm0, 0xa0

%ifdef ERROR
	vdbpsadbw xmm14, xmm8, xmm11, 0x9a
	vdbpsadbw ymm8, ymm14, ymm15, 0xf0
	vdbpsadbw zmm11, zmm10, zmm15, 0x95
	vdbpsadbw xmm27, xmm31, xmm19, 0x2e
	vdbpsadbw ymm20, ymm16, ymm28, 0x99
	vdbpsadbw zmm24, zmm28, zmm18, 0xb2
%endif
