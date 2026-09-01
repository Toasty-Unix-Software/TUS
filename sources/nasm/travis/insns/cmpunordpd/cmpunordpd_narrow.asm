	cmpunordpd xmm1, xmm1
	cmpunordpd xmm7, xmm1

%ifdef ERROR
	cmpunordpd xmm11, xmm15
%endif
