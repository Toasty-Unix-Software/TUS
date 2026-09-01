	psrlw mm1, mm4
	psrlw mm1, mm2
	psrlw mm1, 0x79
	psrlw mm3, 0x40
	psrlw xmm6, xmm1
	psrlw xmm7, xmm4
	psrlw xmm6, 0x35
	psrlw xmm3, 0x55

%ifdef ERROR
	psrlw xmm13, xmm14
	psrlw xmm15, 0xd
%endif
