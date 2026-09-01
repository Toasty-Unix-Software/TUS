	pabsw mm5, mm7
	pabsw mm7, mm6
	pabsw xmm7, oword [0x186]
	pabsw xmm2, xmm2

%ifdef ERROR
	pabsw xmm13, xmm8
%endif
