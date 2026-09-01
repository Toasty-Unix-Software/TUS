	vpperm xmm0, xmm4, xmm1, oword [0xfe4]
	vpperm xmm0, xmm1, oword [0xfe4]
	vpperm xmm5, xmm7, xmm2, xmm7
	vpperm xmm7, xmm2, xmm4, xmm7
	vpperm xmm7, xmm4, xmm7
	vpperm xmm7, xmm5, oword [0x35c], xmm0

%ifdef ERROR
	vpperm xmm14, xmm11, xmm10, xmm12
	vpperm xmm13, xmm15, xmm8, xmm15
%endif
