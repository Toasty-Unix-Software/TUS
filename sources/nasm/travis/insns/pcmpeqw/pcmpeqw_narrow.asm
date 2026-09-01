	pcmpeqw mm2, mm1
	pcmpeqw mm4, mm0
	pcmpeqw xmm3, oword [0x972]
	pcmpeqw xmm1, xmm2

%ifdef ERROR
	pcmpeqw xmm13, xmm11
%endif
