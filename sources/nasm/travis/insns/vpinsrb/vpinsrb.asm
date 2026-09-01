default rel
	vpinsrb xmm1, xmm0, byte [0x3f4], 0x5
	vpinsrb xmm1, byte [0x3f4], 0x5
	vpinsrb xmm5, xmm4, byte [0x5b8], 0xa4
	vpinsrb xmm7, xmm5, cl, 0x88
	vpinsrb xmm7, cl, 0x88
	vpinsrb xmm1, xmm4, byte [0x2e4], 0x9e
	vpinsrb xmm6, xmm0, edx, 0x2a
	vpinsrb xmm6, edx, 0x2a
	vpinsrb xmm5, xmm6, ecx, 0x6f
	vpinsrb xmm8, xmm13, byte [0x6ec], 0xe3
	vpinsrb xmm14, xmm12, r8b, 0x1f
	vpinsrb xmm15, xmm14, r14d, 0x9f
	vpinsrb xmm2, xmm3, byte [eax+1], 0x24
	vpinsrb xmm3, xmm3, byte [eax+64], 0xa4
	vpinsrb xmm7, xmm6, byte [eax+1], 0xaf
	vpinsrb xmm5, xmm4, byte [eax+64], 0xfc
	vpinsrb xmm1, xmm3, [0x924], 0xc2
	vpinsrb xmm7, xmm4, [0xf3f], 0x77
