	minpd xmm1, xmm7
	minpd xmm0, oword [0x520]

%ifdef ERROR
	minpd xmm15, xmm13
%endif
