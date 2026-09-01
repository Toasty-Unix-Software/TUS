	vsqrtsd xmm2, xmm6, xmm0
	vsqrtsd xmm2, xmm0
	vsqrtsd xmm7, xmm3, xmm3
	vsqrtsd xmm7, xmm7, xmm7
	vsqrtsd xmm7, xmm7
	vsqrtsd xmm2, xmm4, xmm6

%ifdef ERROR
	vsqrtsd xmm15, xmm9, xmm8
	vsqrtsd xmm12, xmm13, xmm10
	vsqrtsd xmm20, xmm28, xmm19
	vsqrtsd xmm22, xmm26, xmm27
%endif
