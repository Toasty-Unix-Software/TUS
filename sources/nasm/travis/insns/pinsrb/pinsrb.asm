default rel
	pinsrb xmm3, [0x6ba], 0x3a
	pinsrb xmm4, [0xf70], 0xe
	pinsrb xmm7, dl, 0x9
	pinsrb xmm0, dl, 0x26
	pinsrb xmm1, ebp, 0xd7
	pinsrb xmm4, esi, 0x1a
	pinsrb xmm13, [0x1bf], 0xc5
	pinsrb xmm15, r13b, 0x91
	pinsrb xmm11, r12d, 0x3a
	pinsrb xmm4, [eax+1], 0x1a
	pinsrb xmm1, [eax+64], 0x3d
	pinsrb xmm1, byte [eax+1], 0x52
	pinsrb xmm7, byte [eax+64], 0x34
	pinsrb xmm0, [0x5f7], 0xc7
