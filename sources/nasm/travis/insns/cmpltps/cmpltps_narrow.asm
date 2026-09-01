	cmpltps xmm4, xmm6
	cmpltps xmm0, oword [0xab5]

%ifdef ERROR
	cmpltps xmm12, xmm8
%endif
