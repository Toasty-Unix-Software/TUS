	shufpd xmm1, oword [0x9ac], 0x24
	shufpd xmm0, xmm1, 0x90

%ifdef ERROR
	shufpd xmm15, xmm13, 0x70
%endif
