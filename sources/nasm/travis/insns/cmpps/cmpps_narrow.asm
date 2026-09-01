	cmpps xmm5, oword [0x7b5], 0x1f
	cmpps xmm7, xmm0, 0x84

%ifdef ERROR
	cmpps xmm12, xmm13, 0x61
%endif
