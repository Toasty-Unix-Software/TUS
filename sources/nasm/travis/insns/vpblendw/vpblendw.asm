default rel
	vpblendw xmm1, xmm7, oword [0xa85], 0x26
	vpblendw xmm1, oword [0xa85], 0x26
	vpblendw xmm6, xmm0, oword [0xe7d], 0xf9
	vpblendw ymm4, ymm3, ymm5, 0xf3
	vpblendw ymm4, ymm5, 0xf3
	vpblendw ymm3, ymm6, ymm1, 0x34
	vpblendw xmm15, xmm12, xmm8, 0x8b
	vpblendw ymm9, ymm11, ymm15, 0xaa
	vpblendw xmm1, xmm4, oword [eax+1], 0xfd
	vpblendw xmm7, xmm5, oword [eax+64], 0x2d
	vpblendw ymm6, ymm5, yword [eax+1], 0xb9
	vpblendw ymm1, ymm1, yword [eax+64], 0xaf
	vpblendw xmm3, xmm7, [0x34c], 0xb6
	vpblendw ymm7, ymm4, [0x10a], 0x7b
