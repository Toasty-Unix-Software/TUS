	psubsb mm1, mm0
	psubsb mm7, mm5
	psubsb xmm0, xmm2
	psubsb xmm7, oword [0xb27]

%ifdef ERROR
	psubsb xmm12, xmm12
%endif
