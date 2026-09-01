	cmpneqps xmm7, oword [0xf47]
	cmpneqps xmm3, oword [0x1f1]

%ifdef ERROR
	cmpneqps xmm15, xmm8
%endif
