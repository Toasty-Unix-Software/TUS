	pmulld xmm5, oword [0x302]
	pmulld xmm5, oword [0x7e2]

%ifdef ERROR
	pmulld xmm12, xmm15
%endif
