	pavgw mm2, mm0
	pavgw mm3, mm3
	pavgw xmm6, xmm1
	pavgw xmm4, xmm1

%ifdef ERROR
	pavgw xmm12, xmm15
%endif
