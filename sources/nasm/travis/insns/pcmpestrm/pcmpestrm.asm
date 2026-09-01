default rel
	pcmpestrm xmm1, oword [0x1a6], 0x88
	pcmpestrm xmm4, oword [0x2db], 0xbc
	pcmpestrm xmm13, xmm13, 0x3a
	pcmpestrm xmm3, oword [eax+1], 0x9f
	pcmpestrm xmm2, oword [eax+64], 0x9e
	pcmpestrm xmm2, [0xffc], 0xa6
