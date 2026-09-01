default rel
	vfixupimmpd xmm3, xmm2, xmm7, 0xaa
	vfixupimmpd xmm3, xmm7, 0xaa
	vfixupimmpd xmm0, xmm7, xmm4, 0x61
	vfixupimmpd ymm4, ymm5, ymm7, 0x9f
	vfixupimmpd ymm4, ymm7, 0x9f
	vfixupimmpd ymm3, ymm5, ymm2, 0xef
	vfixupimmpd zmm0, zmm1, zword [0xd97], 0x66
	vfixupimmpd zmm0, zword [0xd97], 0x66
	vfixupimmpd zmm4, zmm3, zmm7, 0xc4
	vfixupimmpd xmm11, xmm13, xmm10, 0x1d
	vfixupimmpd ymm10, ymm9, ymm8, 0x48
	vfixupimmpd zmm8, zmm9, zmm14, 0xbc
	vfixupimmpd xmm31, xmm24, xmm31, 0x58
	vfixupimmpd ymm21, ymm20, ymm23, 0xa6
	vfixupimmpd zmm25, zmm19, zmm17, 0xcd
	vfixupimmpd xmm6{k1}, xmm5, xmm5, 0x24
	vfixupimmpd ymm6{k1}, ymm6, ymm0, 0x42
	vfixupimmpd zmm6{k2}, zmm5, zmm5, 0xe6
	vfixupimmpd xmm1{k3}{z}, xmm3, xmm7, 0x2a
	vfixupimmpd ymm6{k6}{z}, ymm4, ymm2, 0x85
	vfixupimmpd zmm0{k4}{z}, zmm0, zword [0x599], 0xfd
	vfixupimmpd xmm6, xmm6, oword [eax+1], 0xe2
	vfixupimmpd xmm7, xmm4, oword [eax+64], 0x46
	vfixupimmpd ymm6, ymm1, yword [eax+1], 0xe9
	vfixupimmpd ymm5, ymm7, yword [eax+64], 0xa4
	vfixupimmpd zmm2, zmm6, zword [eax+1], 0x43
	vfixupimmpd zmm5, zmm5, zword [eax+64], 0x9d
	vfixupimmpd xmm2, xmm1, [0x1f3], 0x37
	vfixupimmpd ymm3, ymm1, [0x3b6], 0xd7
	vfixupimmpd zmm4, zmm0, [0x589], 0x88
