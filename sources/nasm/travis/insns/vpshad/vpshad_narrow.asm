	vpshad xmm4, oword [0x556], xmm7
	vpshad xmm4, xmm7
	vpshad xmm3, oword [0x667], xmm3
	vpshad xmm6, xmm2, xmm1
	vpshad xmm6, xmm1
	vpshad xmm0, xmm7, xmm2

%ifdef ERROR
	vpshad xmm13, xmm14, xmm13
	vpshad xmm9, xmm10, xmm9
%endif
