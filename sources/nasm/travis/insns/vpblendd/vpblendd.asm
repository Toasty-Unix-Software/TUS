default rel
	vpblendd xmm6, xmm6, oword [0xd08], 0x41
	vpblendd xmm6, oword [0xd08], 0x41
	vpblendd xmm0, xmm1, oword [0x1f1], 0x1
	vpblendd ymm3, ymm2, ymm3, 0x1f
	vpblendd ymm3, ymm3, 0x1f
	vpblendd ymm3, ymm6, ymm6, 0x8d
	vpblendd xmm13, xmm10, xmm14, 0xcd
	vpblendd ymm9, ymm8, ymm9, 0x9f
	vpblendd xmm7, xmm6, oword [eax+1], 0x33
	vpblendd xmm5, xmm2, oword [eax+64], 0x68
	vpblendd ymm6, ymm6, yword [eax+1], 0x4b
	vpblendd ymm4, ymm2, yword [eax+64], 0x34
	vpblendd xmm5, xmm4, [0xe85], 0xc9
	vpblendd ymm5, ymm6, [0x2f1], 0xfe
