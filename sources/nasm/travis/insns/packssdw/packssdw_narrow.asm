	packssdw mm7, mm4
	packssdw mm6, mm0
	packssdw xmm1, xmm4
	packssdw xmm6, xmm2

%ifdef ERROR
	packssdw xmm13, xmm13
%endif
