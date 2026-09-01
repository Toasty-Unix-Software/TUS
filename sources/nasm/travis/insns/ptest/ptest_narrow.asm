	ptest xmm7, oword [0xf74]
	ptest xmm6, xmm3

%ifdef ERROR
	ptest xmm13, xmm9
%endif
