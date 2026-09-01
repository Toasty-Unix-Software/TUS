	haddpd xmm0, oword [0xd71]
	haddpd xmm7, xmm6

%ifdef ERROR
	haddpd xmm15, xmm8
%endif
