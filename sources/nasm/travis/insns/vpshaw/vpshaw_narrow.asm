	vpshaw xmm5, oword [0xeeb], xmm6
	vpshaw xmm5, xmm6
	vpshaw xmm4, xmm3, xmm1
	vpshaw xmm2, xmm4, oword [0xe41]
	vpshaw xmm2, oword [0xe41]
	vpshaw xmm4, xmm6, xmm0

%ifdef ERROR
	vpshaw xmm10, xmm8, xmm15
	vpshaw xmm10, xmm11, xmm10
%endif
