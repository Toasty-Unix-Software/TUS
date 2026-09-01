	cmpordpd xmm5, xmm0
	cmpordpd xmm7, xmm2

%ifdef ERROR
	cmpordpd xmm10, xmm12
%endif
