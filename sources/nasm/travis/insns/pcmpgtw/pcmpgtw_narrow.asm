	pcmpgtw mm0, mm2
	pcmpgtw mm5, mm2
	pcmpgtw xmm1, oword [0x150]
	pcmpgtw xmm1, xmm0

%ifdef ERROR
	pcmpgtw xmm15, xmm8
%endif
