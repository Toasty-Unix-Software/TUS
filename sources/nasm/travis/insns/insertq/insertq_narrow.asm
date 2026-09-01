	insertq xmm0, xmm7, 0x6d, 0x20
	insertq xmm1, xmm2, 0x7a, 0xc
	insertq xmm2, xmm4
	insertq xmm0, xmm2

%ifdef ERROR
	insertq xmm8, xmm11, 0x3a, 0x6c
	insertq xmm14, xmm13
%endif
