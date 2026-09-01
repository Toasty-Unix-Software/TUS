	vpshrdq xmm4, xmm0, xmm4, 0xf6
	vpshrdq xmm4, xmm4, 0xf6
	vpshrdq xmm3, xmm4, xmm7, 0x6c
	vpshrdq ymm0, ymm1, ymm2, 0x50
	vpshrdq ymm0, ymm2, 0x50
	vpshrdq ymm7, ymm5, ymm7, 0x20
	vpshrdq zmm3, zmm2, zword [0x93e], 0x39
	vpshrdq zmm3, zword [0x93e], 0x39
	vpshrdq zmm3, zmm0, zmm5, 0x61

%ifdef ERROR
	vpshrdq xmm15, xmm11, xmm8, 0x44
	vpshrdq ymm12, ymm9, ymm8, 0xd4
	vpshrdq zmm12, zmm8, zmm15, 0x6d
	vpshrdq xmm29, xmm23, xmm23, 0xcd
	vpshrdq ymm20, ymm27, ymm23, 0xf4
	vpshrdq zmm17, zmm24, zmm18, 0xc6
%endif
