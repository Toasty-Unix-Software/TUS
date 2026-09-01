	pmaxsb xmm3, oword [0x6ae]
	pmaxsb xmm3, xmm2

%ifdef ERROR
	pmaxsb xmm8, xmm13
%endif
