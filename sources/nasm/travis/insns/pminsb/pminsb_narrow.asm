	pminsb xmm2, xmm0
	pminsb xmm4, xmm3

%ifdef ERROR
	pminsb xmm13, xmm13
%endif
