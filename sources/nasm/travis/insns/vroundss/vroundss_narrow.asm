	vroundss xmm4, xmm6, xmm2, 0x2f
	vroundss xmm4, xmm2, 0x2f
	vroundss xmm5, xmm3, xmm2, 0xe9

%ifdef ERROR
	vroundss xmm11, xmm13, xmm15, 0xb9
%endif
