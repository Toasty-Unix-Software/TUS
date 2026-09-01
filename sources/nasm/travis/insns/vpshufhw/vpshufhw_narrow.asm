	vpshufhw xmm1, oword [0x133], 0xbb
	vpshufhw xmm6, xmm5, 0xa0
	vpshufhw ymm6, yword [0x5d5], 0x2b
	vpshufhw ymm6, ymm5, 0x15
	vpshufhw xmm1, xmm0, 0xf7
	vpshufhw xmm6, xmm2, 0x7
	vpshufhw ymm6, ymm3, 0xa5
	vpshufhw ymm0, ymm4, 0xd0

%ifdef ERROR
	vpshufhw xmm12, xmm13, 0x23
	vpshufhw ymm13, ymm11, 0x11
	vpshufhw xmm9, xmm15, 0xbc
	vpshufhw ymm8, ymm9, 0x53
	vpshufhw xmm24, xmm29, 0x7
	vpshufhw ymm31, ymm28, 0x14
	vpshufhw xmm23, xmm30, 0xcc
	vpshufhw ymm22, ymm17, 0xa7
%endif
