	vpshld xmm4, xmm4, xmm0
	vpshld xmm4, xmm0
	vpshld xmm6, xmm3, xmm3
	vpshld xmm2, xmm0, oword [0x980]
	vpshld xmm2, oword [0x980]
	vpshld xmm7, xmm5, xmm6

%ifdef ERROR
	vpshld xmm14, xmm14, xmm14
	vpshld xmm9, xmm10, xmm13
%endif
