	pcmpeqq xmm7, xmm6
	pcmpeqq xmm0, oword [0x7a2]

%ifdef ERROR
	pcmpeqq xmm10, xmm15
%endif
