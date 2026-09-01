	vgetmantsh xmm5, xmm4, xmm7, 0x22
	vgetmantsh xmm3, xmm7, xmm4, 0xa5

%ifdef ERROR
	vgetmantsh xmm13, xmm15, xmm9, 0x5f
	vgetmantsh xmm30, xmm29, xmm28, 0x46
%endif
