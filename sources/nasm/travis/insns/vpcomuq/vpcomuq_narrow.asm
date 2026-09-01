	vpcomuq xmm3, xmm6, xmm4, 0xfb
	vpcomuq xmm3, xmm4, 0xfb
	vpcomuq xmm2, xmm2, xmm7, 0xba

%ifdef ERROR
	vpcomuq xmm12, xmm11, xmm8, 0x4
%endif
