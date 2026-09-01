	pmuldq xmm7, xmm6
	pmuldq xmm0, oword [0x779]

%ifdef ERROR
	pmuldq xmm15, xmm11
%endif
