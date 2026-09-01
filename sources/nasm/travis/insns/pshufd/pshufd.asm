default rel
	pshufd xmm2, xmm4, 0x31
	pshufd xmm2, xmm7, 0x5e
	pshufd xmm3, [0x4e6], 0x65
	pshufd xmm7, [0x3ef], 0x1d
	pshufd xmm14, xmm10, 0x1b
	pshufd xmm8, [0xd04], 0x2f
	pshufd xmm6, [eax+1], 0x16
	pshufd xmm0, [eax+64], 0x74
