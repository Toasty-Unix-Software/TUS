	subps xmm2, xmm4
	subps xmm0, oword [0xa14]

%ifdef ERROR
	subps xmm15, xmm8
%endif
