	vpshab xmm1, oword [0x7f1], xmm0
	vpshab xmm1, xmm0
	vpshab xmm4, xmm3, xmm5
	vpshab xmm4, xmm3, xmm3
	vpshab xmm4, xmm3
	vpshab xmm2, xmm0, xmm4

%ifdef ERROR
	vpshab xmm10, xmm10, xmm8
	vpshab xmm10, xmm15, xmm12
%endif
