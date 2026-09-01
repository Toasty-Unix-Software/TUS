default rel
	vpshrdw xmm3, xmm7, xmm5, 0xba
	vpshrdw xmm3, xmm5, 0xba
	vpshrdw xmm7, xmm2, xmm3, 0x99
	vpshrdw ymm4, ymm5, ymm1, 0x41
	vpshrdw ymm4, ymm1, 0x41
	vpshrdw ymm1, ymm3, yword [0x167], 0x66
	vpshrdw zmm0, zmm0, zword [0xc2a], 0xd8
	vpshrdw zmm0, zword [0xc2a], 0xd8
	vpshrdw zmm5, zmm2, zmm6, 0x2a
	vpshrdw xmm10, xmm9, xmm12, 0x12
	vpshrdw ymm13, ymm13, ymm9, 0x63
	vpshrdw zmm14, zmm13, zmm9, 0xad
	vpshrdw xmm22, xmm19, xmm25, 0xf4
	vpshrdw ymm30, ymm28, ymm25, 0x4f
	vpshrdw zmm28, zmm21, zmm16, 0xe9
	vpshrdw xmm4{k5}, xmm2, oword [0x7f8], 0xa0
	vpshrdw ymm6{k2}, ymm7, ymm3, 0x4f
	vpshrdw zmm3{k5}, zmm1, zword [0x177], 0x69
	vpshrdw xmm4{k7}{z}, xmm4, xmm1, 0x3f
	vpshrdw ymm4{k2}{z}, ymm4, ymm0, 0x22
	vpshrdw zmm2{k5}{z}, zmm7, zmm5, 0xde
	vpshrdw xmm0, xmm0, oword [eax+1], 0xe8
	vpshrdw xmm2, xmm3, oword [eax+64], 0xbc
	vpshrdw ymm7, ymm3, yword [eax+1], 0xee
	vpshrdw ymm7, ymm2, yword [eax+64], 0x23
	vpshrdw zmm1, zmm4, zword [eax+1], 0x50
	vpshrdw zmm7, zmm3, zword [eax+64], 0x44
	vpshrdw xmm1, xmm1, [0x550], 0x5a
	vpshrdw ymm2, ymm1, [0x2b4], 0x82
	vpshrdw zmm1, zmm0, [0x6bb], 0x3a
