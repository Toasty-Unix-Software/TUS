	pcmpestrm xmm1, oword [0x1a6], 0x88
	pcmpestrm xmm4, oword [0x2db], 0xbc

%ifdef ERROR
	pcmpestrm xmm13, xmm13, 0x3a
%endif
