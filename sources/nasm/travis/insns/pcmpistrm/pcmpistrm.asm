default rel
	pcmpistrm xmm0, xmm1, 0xbd
	pcmpistrm xmm0, xmm2, 0xd6
	pcmpistrm xmm13, xmm9, 0x18
	pcmpistrm xmm4, oword [eax+1], 0x3c
	pcmpistrm xmm4, oword [eax+64], 0x2a
	pcmpistrm xmm0, [0xef4], 0x5c
