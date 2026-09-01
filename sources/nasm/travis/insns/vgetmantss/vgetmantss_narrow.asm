	vgetmantss xmm5, xmm7, xmm2, 0x96
	vgetmantss xmm0, xmm2, xmm0, 0x2

%ifdef ERROR
	vgetmantss xmm9, xmm15, xmm9, 0x6c
	vgetmantss xmm25, xmm29, xmm28, 0xe3
%endif
