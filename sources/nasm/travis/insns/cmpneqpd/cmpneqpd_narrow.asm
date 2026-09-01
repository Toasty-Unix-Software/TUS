	cmpneqpd xmm1, xmm1
	cmpneqpd xmm0, xmm2

%ifdef ERROR
	cmpneqpd xmm15, xmm8
%endif
