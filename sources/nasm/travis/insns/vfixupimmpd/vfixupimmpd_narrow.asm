	vfixupimmpd xmm3, xmm2, xmm7, 0xaa
	vfixupimmpd xmm3, xmm7, 0xaa
	vfixupimmpd xmm0, xmm7, xmm4, 0x61
	vfixupimmpd ymm4, ymm5, ymm7, 0x9f
	vfixupimmpd ymm4, ymm7, 0x9f
	vfixupimmpd ymm3, ymm5, ymm2, 0xef
	vfixupimmpd zmm0, zmm1, zword [0xd97], 0x66
	vfixupimmpd zmm0, zword [0xd97], 0x66
	vfixupimmpd zmm4, zmm3, zmm7, 0xc4

%ifdef ERROR
	vfixupimmpd xmm11, xmm13, xmm10, 0x1d
	vfixupimmpd ymm10, ymm9, ymm8, 0x48
	vfixupimmpd zmm8, zmm9, zmm14, 0xbc
	vfixupimmpd xmm31, xmm24, xmm31, 0x58
	vfixupimmpd ymm21, ymm20, ymm23, 0xa6
	vfixupimmpd zmm25, zmm19, zmm17, 0xcd
%endif
