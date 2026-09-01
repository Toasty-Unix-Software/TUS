default rel
	vextractps ecx, xmm1, 0x59
	vextractps ebp, xmm5, 0x1
	vextractps esi, xmm0, 0xf0
	vextractps eax, xmm4, 0x3e
	vextractps rbp, xmm1, 0x27
	vextractps rsi, xmm4, 0x50
	vextractps dword [0xbf8], xmm3, 0xb8
	vextractps dword [0x172], xmm1, 0xe1
	vextractps r15d, xmm12, 0x1a
	vextractps r14d, xmm9, 0xba
	vextractps r15, xmm14, 0x43
	vextractps dword [0x5a2], xmm14, 0x35
	vextractps r28d, xmm24, 0x30
	vextractps r17d, xmm23, 0x66
	vextractps r25, xmm27, 0xd6
	vextractps dword [0x728], xmm27, 0x63
	vextractps dword [eax+1], xmm7, 0x1e
	vextractps dword [eax+64], xmm6, 0xe
	vextractps dword [eax+1], xmm4, 0x2c
	vextractps dword [eax+64], xmm4, 0x1b
	vextractps [0x245], xmm2, 0xc5
	vextractps [0x67b], xmm6, 0xa2
