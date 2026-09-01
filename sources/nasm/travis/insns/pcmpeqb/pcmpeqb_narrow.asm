	pcmpeqb mm2, mm0
	pcmpeqb mm6, mm4
	pcmpeqb xmm0, oword [0x459]
	pcmpeqb xmm0, xmm0

%ifdef ERROR
	pcmpeqb xmm8, xmm12
%endif
