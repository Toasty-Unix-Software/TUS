	hsubps xmm4, oword [0xa5b]
	hsubps xmm7, oword [0x70a]

%ifdef ERROR
	hsubps xmm11, xmm15
%endif
