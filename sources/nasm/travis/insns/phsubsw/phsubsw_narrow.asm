	phsubsw mm4, mm0
	phsubsw mm3, mm5
	phsubsw xmm2, oword [0x7a0]
	phsubsw xmm7, xmm7

%ifdef ERROR
	phsubsw xmm8, xmm8
%endif
