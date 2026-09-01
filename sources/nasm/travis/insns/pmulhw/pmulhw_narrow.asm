	pmulhw mm6, mm1
	pmulhw mm5, mm1
	pmulhw xmm1, xmm6
	pmulhw xmm3, oword [0x302]

%ifdef ERROR
	pmulhw xmm10, xmm15
%endif
