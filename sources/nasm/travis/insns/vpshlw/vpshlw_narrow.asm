	vpshlw xmm4, xmm4, xmm2
	vpshlw xmm4, xmm2
	vpshlw xmm5, oword [0xc4f], xmm0
	vpshlw xmm2, xmm5, xmm1
	vpshlw xmm2, xmm1
	vpshlw xmm7, xmm4, xmm6

%ifdef ERROR
	vpshlw xmm10, xmm9, xmm12
	vpshlw xmm13, xmm9, xmm15
%endif
