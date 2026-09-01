	phsubw mm7, mm5
	phsubw mm7, mm7
	phsubw xmm0, xmm2
	phsubw xmm6, oword [0xdce]

%ifdef ERROR
	phsubw xmm9, xmm8
%endif
