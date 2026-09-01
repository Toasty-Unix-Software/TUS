	vmulsd xmm7, xmm2, xmm2
	vmulsd xmm7, xmm2
	vmulsd xmm1, xmm4, xmm4
	vmulsd xmm7, xmm6, xmm2
	vmulsd xmm7, xmm2
	vmulsd xmm5, xmm6, xmm0

%ifdef ERROR
	vmulsd xmm10, xmm10, xmm12
	vmulsd xmm15, xmm13, xmm15
	vmulsd xmm18, xmm16, xmm28
	vmulsd xmm16, xmm24, xmm25
%endif
